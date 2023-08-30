#include "def.h"


BYTE cbuf[600];
BYTE n_port=1;
WORD u_size = 0;	//for usart_read
MBAP_HDR hdr_struct;

#define CM2_WHO_ARE_YOU 0x01
#define UID_WHO_ARE_YOU 0x8001


#define CM2_STATUS_PACK 0x02
#define UID_STATUS_PACK 0x8002

void cmd_process(void)
{
	cmd_common_process ();
	cmd_usart_process ();
}


void cmd_common_process (void)
{
	WORD  size = 0;
	WORD  addr = 0;
	WORD  cnt  = 0;
	WORD  wn   = 0;
	WORD  cs   = 0;
	BYTE  i		= 0;
	
	if(eth_sock[0].r_status==FALSE) {return;}
	eth_sock[0].r_status=FALSE;

	size=((eth_sock[0].len[0]<<8) | (eth_sock[0].len[1]));
	
	if(size>sizeof(cbuf))	{size=sizeof(cbuf);}
	
	memcpy(cbuf,(BYTE*)&eth_sock[0].data,size);

	if(size						  <    5)	{ return; }
	if(size                       >  1024)	{ return; }
	
	if(crc16_ccit(cbuf,size)   !=   0)		{ return; }
	
	addr  = *(__packed WORD*)(cbuf+0);      wn=+sizeof(WORD);

	if(addr !=  0){return;}
	wn++;//cmd

	switch(cbuf[2])
	{
		case 0x01:	if(size != 6) { return; }             // CMD=0x07 Read CFG		
		
					if(cbuf[wn]==0x01)
					{
						cbuf[wn]  = (BYTE)UID_WHO_ARE_YOU;								wn += sizeof(BYTE);
						cbuf[wn]  = (BYTE)(UID_WHO_ARE_YOU>>8);							wn += sizeof(BYTE);
						memcpy(&cbuf[wn],MODEL,strlen(MODEL));							wn+=strlen(MODEL);
						//memcpy(&cbuf[wn],((BYTE*)&cfg.com_network.mac_addr),6);			wn+=6;
						memcpy(&cbuf[wn],VERSION,strlen(VERSION));						wn+=strlen(VERSION);
					}
		
				if(cbuf[wn]==0x02)
				{
					cbuf[wn]  = (BYTE)UID_STATUS_PACK;								wn += sizeof(BYTE);
					cbuf[wn]  = (BYTE)(UID_STATUS_PACK>>8);							wn += sizeof(BYTE); //uid device
					memcpy(&cbuf[wn],(BYTE*)&TTL,4);								wn += sizeof(DWORD);//TTL
			
					for (i = 0; i < 4; i++)
						{
						memcpy(&cbuf[wn],(BYTE*)&eth_sock[i+1].counters.rx,4);		wn += sizeof(DWORD);
						memcpy(&cbuf[wn],(BYTE*)&eth_sock[i+1].counters.tx,4);		wn += sizeof(DWORD);
						memcpy(&cbuf[wn],(BYTE*)&port[i].counters.rx,4);			wn += sizeof(DWORD);
						memcpy(&cbuf[wn],(BYTE*)&port[i].counters.tx,4);			wn += sizeof(DWORD);
						memcpy(&cbuf[wn],(BYTE*)&port[i].dt,2);						wn += sizeof(WORD);//dt port0
						}			
				}
		break;
		//......................................................................
		case 0x07:	if(size != 5) { return; }             // CMD=0x07 Read CFG
		
					cnt= sizeof(CFG_1);
					memcpy(&cbuf[wn],((BYTE*)&cfg_1),cnt);						wn+=cnt;
		break;

		//......................................................................
		case 0x10:	if(size <  6) { return; }
					wn+=iap_process(&cbuf[wn],size-5);
		break;

		//......................................................................
		case 0x17:	if(size  <  5) { return; }
		
					memcpy(((BYTE*)&cfg_1_tmp),cbuf+wn                  ,sizeof(CFG_1));
					memcpy(cbuf+wn                  ,((BYTE*)&cfg_1_tmp),sizeof(CFG_1));
					wn+=sizeof(CFG_1);
		break;
		//......................................................................
		case 0x27:	if(size  !=  5) { return; }
		
					if(crc16_ccit((BYTE*)&cfg_1_tmp,sizeof(CFG_1)) != 0)
					{
						break;
					}
		
					memcpy(&cfg_1,&cfg_1_tmp,sizeof(CFG_1));
					wn |=+cfg_save();
					reset=1;
		break;
		//......................................................................
		
		default :	   return;
	}
	
	cbuf[2]|=0x80;

	cs= crc16_ccit(cbuf,wn);
	
	cbuf[wn] = (BYTE)(cs & 0x00ff);         wn++;
	cbuf[wn] = (BYTE)((cs & 0xff00) >> 8);  wn++;
	
	memcpy((BYTE*)&eth_sock[0].data,cbuf,wn);
	
	eth_sock[0].len[0]=((wn & 0xFF00)>>8);
	eth_sock[0].len[1]=(wn & 0x00FF);
	eth_sock[0].w_status=1;

}

void cmd_usart_process (void)
{
	if(n_port==5){n_port=1;}
	usart_process(n_port);
	n_port++;
}

WORD cut_hdr(BYTE* messege)
{
	hdr_struct.transaction_n = *(messege) << 8; messege++;
	hdr_struct.transaction_n += *(messege); messege++;
	hdr_struct.protocol = *(messege) << 8; messege++;
	hdr_struct.protocol += *(messege); messege++;
	hdr_struct.len = *(messege) << 8; messege++;
	hdr_struct.len += *(messege); messege++;
	
	if(!hdr_struct.protocol)	{return hdr_struct.len;}
	else						{return 0;}
}

WORD add_hdr(BYTE* buf, WORD mess_size)
{
	*buf = hdr_struct.transaction_n >> 8; buf++;
	*buf = hdr_struct.transaction_n; buf++;
	*buf = hdr_struct.protocol >> 8; buf++;
	*buf = hdr_struct.protocol; buf++;
	*buf = mess_size >> 8; buf++;
	*buf = mess_size; buf++;
}


void usart_process (BYTE n_port)
{
	WORD size=0;
	static WORD r_cnt=0;
	static WORD w_cnt=0;
	static BYTE udp_mode=0;
	BYTE buff[USART_BUF_SIZE];
	WORD crc;
	
	if(cfg_1.sock_rs485[n_port-1].en==FALSE) {return;}
	
	switch(port[n_port-1].stage)
	{
		case RS485_WRITE:
		//ETH message check
		if (!eth_sock[n_port].r_status){return;}									//check read stat
		port[n_port-1].time_port = port[n_port-1].tout_port*10;						//
		
		size = eth_sock[n_port].len[0] << 8 | eth_sock[n_port].len[1];				//give size
		if (cfg_1.sock_rs485[n_port-1].mode == TCP_GATE)	{size += SKIP_HDR;}		//cut header (for TCP)
		if (cfg_1.sock_rs485[n_port-1].mode == TCP_IP)		{size += SKIP_HDR;}		//cut header (for TCP)
			
//HC block
		if (cfg_1.sock_rs485[n_port - 1].mode < 1)
		{		
			cut_hdr(eth_sock[n_port].data);
			size -= MBAP_HDR_LEN;								//
			memcpy(&buff[0], &eth_sock[n_port].data[6], size);	//copy data without header in buffer
			crc = crc16_mbus(buff, size);						//calculate crc
			buff[size] = crc; buff[size + 1] = crc >> 8;		//add crc on buffer
			usart_write(n_port - 1, &buff, size + 2);			//write data on 
			udp_mode = 1;										//need delete after add cfg udp_ip
		}
		else {usart_write(n_port - 1, eth_sock[n_port].data, size); udp_mode = 0;}
//HC block	

		w_cnt++;
		port[n_port-1].stage = RS485_READ;
		port[n_port-1].rn = 0;

		return;
		case RS485_READ://UP
		u_size = usart_read(n_port - 1, port[n_port-1].rbuf, USART_BUF_SIZE);   //give mess size
		if (u_size != 0)
		{
			//if (cfg_1.sock_rs485[n_port - 1].mode < 1){		}
			if(udp_mode == 1)
			{
				add_hdr(&eth_sock[n_port].data[0], u_size - 1);													//write header and minus size check sum
				u_size	+= MBAP_HDR_LEN;																		//if replase in memcpy, all breaking
				memcpy(&eth_sock[n_port].data[6], port[n_port-1].rbuf, u_size);									//copy data in eth data
			} else {memcpy(eth_sock[n_port].data, port[n_port-1].rbuf, u_size); }								//gate mode
			
			eth_sock[n_port].len[0]		= (u_size & 0xFF00) >> 8;
			eth_sock[n_port].len[1]		=  u_size & 0x00FF; //write mess size in port_udp
			eth_sock[n_port].w_status	= 1;
			port[n_port-1].dt			= (port[n_port-1].tout_port*10)-port[n_port-1].time_port;
			port[n_port-1].stage		= RS485_WRITE;
			eth_sock[n_port].r_status = 0;
			r_cnt++;
			return;
		}
		if (port[n_port-1].time_port==0)
		{
			port[n_port-1].stage = RS485_WRITE;
			port[n_port-1].dt 	 = 0;
			eth_sock[n_port].r_status = 0;
		}
		return;
		default:
		port[n_port-1].stage = RS485_WRITE;
		break;
	}
}