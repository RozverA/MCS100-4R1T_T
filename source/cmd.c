#include "def.h"


BYTE cbuf[300];
volatile BYTE log_stat = 0;

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
	WORD  ixo  = 0;
	DWORD ip_addr;

	
	if(eth_sock[0].r_status==FALSE) {return;}
	eth_sock[0].r_status=FALSE;

	size=((eth_sock[0].len[0]<<8) | (eth_sock[0].len[1]));
	
	if(size  >    sizeof(cbuf))	{size=sizeof(cbuf);}
	if(size	 <    5			  )	{ return; }
	
	memcpy(cbuf,(BYTE*)&eth_sock[0].data,size);

	if(crc16_ccit(cbuf,size)   !=   0)		{ return; }
	
	addr  = *(__packed WORD*)(cbuf+0);      wn=+sizeof(WORD);

	if(addr !=  0){return;}
	wn++;//cmd
//logging
	if (!SRAV(4, &eth_sock[0].ip_addr[0], &ip_addr)){log_stat = 0;}
	if ((cbuf[2] != 0x20) && (!log_stat)) {return 0;}
	
	
	switch(cbuf[2])
	{
		case 0x01:	if(size != 6) { return; }             // CMD=0x07 Read CFG		
		
					if(cbuf[wn]==0x01)
					{
						cbuf[wn]  = (BYTE)UID_WHO_ARE_YOU;								wn += sizeof(BYTE);
						cbuf[wn]  = (BYTE)(UID_WHO_ARE_YOU>>8);							wn += sizeof(BYTE);
						memcpy(&cbuf[wn],MODEL,sizeof(MODEL));							wn +=sizeof(MODEL);
						memcpy(&cbuf[wn],VERSION,sizeof(VERSION));						wn +=sizeof(VERSION);
						break;
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
		//......................................................................
		case 0x07:	if(size != 7)			{ return; }										// CMD=0x07 Read CFG_1	
		
					ixo=cbuf[3] | (cbuf[4]<<8);							wn+=sizeof(WORD);
					cnt= sizeof(CFG_1) - ixo;
					if(ixo > sizeof(CFG_1)	) { break;  }
					if(cnt > 256			) { cnt=256;  }
					cbuf[wn]=(cnt & 0x00FF);							wn+=sizeof(BYTE);
					cbuf[wn]=(cnt & 0xFF00)>>8;							wn+=sizeof(BYTE);

					memcpy(&cbuf[wn],((BYTE*)&cfg_1)+ixo,cnt);				wn+=cnt;
		break;

		//......................................................................
		case 0x08:	if(size != 7) { return; }												// CMD=0x08 Read CFG 2
		
					ixo=cbuf[3] | (cbuf[4]<<8);							wn+=sizeof(WORD);
					cnt= sizeof(CFG_2) - ixo;
					if(ixo > sizeof(CFG_2)	) { break;  }
					if(cnt > 256			) { cnt=256;  }
					cbuf[wn]=(cnt & 0x00FF);							wn+=sizeof(BYTE);
					cbuf[wn]=(cnt & 0xFF00)>>8;							wn+=sizeof(BYTE);

					
					memcpy(&cbuf[wn],((BYTE*)&cfg_2)+ixo,cnt);				wn+=cnt;
		break;

		//......................................................................

		case 0x10:	if(size <  6) { return; }
					wn+=iap_process(&cbuf[wn],size-5);
		break;

		//......................................................................
		case 0x17:	if(size  <  7) { return; }
		
					ixo = cbuf[3] | (cbuf[4]<<8);							wn+=sizeof(WORD);
					cnt = cbuf[5] | (cbuf[6]<<8);							wn+=sizeof(WORD);
					
					if(cnt  ==  0) { break; }
					if(ixo  ==  0) {memset(&cfg_1_tmp,0x00,sizeof(CFG_1));}
					
					if(ixo       >= sizeof(CFG_1)) { break; }
					if((cnt+ixo) >  sizeof(CFG_1)) { break; }
					
					memcpy(((BYTE*)&cfg_1_tmp)+ixo,cbuf+wn                  ,cnt);
					memcpy(cbuf+wn                  ,((BYTE*)&cfg_1_tmp)+ixo,cnt);
					wn+=cnt;
		break;
		//......................................................................
		case 0x20:	if(size <  7) { return; }
					if (!cbuf[3])	{return;}										cnt = 4;
					if			(!SRAV(32, cbuf[cnt], accnts.user.login[0]))		{wn++;break;}
					else if		(!SRAV(32, cbuf[cnt], accnts.admin.login[0]))		{wn++;break;}

					cnt += 32;														
					if			(!SRAV(32, cbuf[cnt], accnts.user.password[0]))		{wn++;break;}
					if			(!SRAV(32, cbuf[cnt], accnts.admin.password[0]))		{wn++;break;}
					log_stat = 1;
					memcpy(&ip_addr, eth_sock[0].ip_addr[0], DW_LEN);					
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
		case 0x29:	if(size  !=  5) { return; }
		
					reset=1; wn++;
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
	static BYTE n_port=1;
	
	for(n_port=1;n_port<5;n_port++)
	{
		if(cfg_1.sock_rs485[n_port-1].en==FALSE) {continue;}
		usart_process(n_port);
	}
	
	
}

void usart_process (BYTE n_port)
{
	WORD size=0;
	WORD crc;
	
	switch(port[n_port-1].stage)
	{
		case RS485_WRITE:
			//ETH message check
			if (!eth_sock[n_port].r_status){return;}									//check read stat
			port[n_port-1].time_port = port[n_port-1].tout_port*10;						//
		
			size = eth_sock[n_port].len[0] << 8 | eth_sock[n_port].len[1];				//give size
		
			if(size>USART_BUF_SIZE)	{eth_sock[n_port].r_status = 0; return;}	
				
			switch(cfg_1.sock_rs485[n_port - 1].pl)
			{
				case MBUS:		
					crc = crc16_mbus(eth_sock[n_port].data+MBAP_HDR_LEN, size - MBAP_HDR_LEN);
					eth_sock[n_port].data[size] = crc; 
					eth_sock[n_port].data[size + 1] = crc >> 8;
					usart_write(n_port - 1, eth_sock[n_port].data + MBAP_HDR_LEN, size - MBAP_HDR_LEN + 2);
				break;
				default://GATE
					usart_write(n_port - 1, eth_sock[n_port].data, size);
				break;
			}
			port[n_port-1].stage = RS485_READ;
			port[n_port-1].rn = 0;

		return;
		case RS485_READ://UP
			size = usart_read(n_port - 1, port[n_port-1].rbuf, USART_BUF_SIZE);   //give mess size
			if (size != 0)
			{
				switch (cfg_1.sock_rs485[n_port - 1].pl)
				{
					case MBUS:
					
						memcpy(eth_sock[n_port].data + MBAP_HDR_LEN, port[n_port-1].rbuf, size);
						size += MBAP_HDR_LEN - 2;
						eth_sock[n_port].data[5]=size-MBAP_HDR_LEN;
					break;
					default://GATE
						memcpy(eth_sock[n_port].data, port[n_port-1].rbuf, size); 
					break;
				}
			
				eth_sock[n_port].len[0]		= (size & 0xFF00) >> 8;
				eth_sock[n_port].len[1]		=  size & 0x00FF;
				eth_sock[n_port].w_status	= 1;
				port[n_port-1].dt			= (port[n_port-1].tout_port*10)-port[n_port-1].time_port;
				port[n_port-1].stage		= RS485_WRITE;
				eth_sock[n_port].r_status	= 0;
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