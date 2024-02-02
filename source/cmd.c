#include "def.h"


BYTE			cbuf[300];
DWORD			ip_addrs;
volatile BYTE	log_stat		= 0;
BYTE			actv_user_id	= 0;
DWORD			rsv_time		= 0;
BYTE			wr_flag_cfg = 0;
BYTE			wr_flag_acc = 0;

#define CM2_WHO_ARE_YOU 0x01
#define UID_WHO_ARE_YOU 0x8001

#define send_error()		{cbuf[wn] = 0; wn++; break;}
#define send_error_EX()		{cbuf[wn++] = 0;cbuf[wn++] = 0;cbuf[wn++] = 0;cbuf[wn++] = 0;cbuf[wn++] = 0;break;}
#define send_admin()		{cbuf[wn] = 1; wn++;}
#define send_user()			{cbuf[wn] = 2; wn++;}
#define send_numb(numb)		{cbuf[wn] = numb; wn++;}
#define check_admin()		{if(actv_user_id != ADMIN_LOGIN) {send_error();}}
#define save_log(n)			{if (rsv_time != 0 ) {log_safe(actv_user_id, ip_addrs, rsv_time, n);}}

#define SEC					10000 

#define CM2_STATUS_PACK 0x02
#define UID_STATUS_PACK 0x8002

void cmd_process(void)
{
	cmd_common_process ();
	cmd_usart_process ();
}


void cmd_common_process (void)
{
	WORD	size	= 0;
	WORD	addr	= 0;
	WORD	cnt		= 0;
	WORD	wn		= 0;
	WORD	cs		= 0;
	BYTE	i		= 0;
	WORD	ixo		= 0;
	BYTE	ch		= 0;
	
	if (log_stat == 1)	
	{
		if (loggin_timeout > (3010000)) 
		{log_stat = 0;}
	}
			
	if(eth_sock[0].r_status==FALSE) {return;}
	eth_sock[0].r_status=FALSE;

	size=((eth_sock[0].len[0]<<8) | (eth_sock[0].len[1]));
	
	if(size	 <    5			  )	{ return; }
	
	if(size  >    sizeof(cbuf))	{size=sizeof(cbuf);}
	memcpy(cbuf,(BYTE*)&eth_sock[0].data,size);

	if(crc16_ccit(cbuf,size)   !=   0)		{ return; }
	
	addr  = *(__packed WORD*)(cbuf+0);      wn=+sizeof(WORD);

	if(addr !=  0){return;}
	wn++;//cmd
//logging
	if (!SRAV(4, &eth_sock[0].ip_addr[0], &ip_addrs)) {log_stat = 0;}

	if ( ( (cbuf[2] != 0x00) && (cbuf[2] != 0x20) && (cbuf[2] != 0x10) ) && (!log_stat) ) 
	{return 0;}
	
	
	
	
	switch(cbuf[2])
	{
		case 0x00:	if(size  !=  9) { return; }
					rsv_time = num_aus_byte(DW_LEN, &cbuf[3], R_SIDE);
					loggin_timeout = 0;
					send_admin();
		break;
		case 0x01:	if(size != 6) { return; }             // CMD=0x07 Read CFG	net	
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
					check_admin();	
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
					check_admin();	
		
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
					wr_flag_cfg = 1;
		break;
		//......................................................................
		case 0x20:	if(size <  7)		{ return; }								//login

					cnt = 3;
					decrypted(&cbuf[cnt]);

					for (i = 0; i < 2; i++ ) { if (SRAV(32, &cbuf[3], &accnts.accnt[i].login[0])) 	{ch = i + 1; break;	} } //compare login
					if (!ch) {send_error_EX();}																				//if login not searched

					cnt += 32;
					if (!SRAV(32, &cbuf[cnt], &accnts.accnt[ch-1].password[0])) {send_error_EX();}									//if password no match
																
					if (ch == ADMIN_LOGIN) {save_log(ADMIN_LOGIN); send_admin();} else {send_user();}
					actv_user_id = ch;
					log_stat = 1;	
					loggin_timeout = 0;																						//login status up
					memcpy((BYTE*)&ip_addrs, &eth_sock[0].ip_addr[0], DW_LEN);														//write crc firmware
					num_to_byte(crc_fw, DW_LEN, &cbuf[wn], L_SIDE);			wn += 4;
		break;
		//......................................................................
		case 0x21:	if(size < 5)		{ return; }								//login send
 					check_admin();
					cnt = 3;
					
					memcpy(&cbuf[3], &accnts.accnt[0].login[0], USART_BUF_SIZE);
					crypted(&cbuf[cnt]);		cnt += 64;
					crypted(&cbuf[cnt]);		cnt += 64;
					crypted(&cbuf[cnt]);		cnt += 64;
					crypted(&cbuf[cnt]);		cnt += 64;
					wn += 256;
		break;
		//......................................................................
		case 0x22:	if(size <  5)		{ return; }								//login change
					check_admin();
					cnt = 3;
					decrypted(&cbuf[cnt]);		cnt += 64;	
					decrypted(&cbuf[cnt]);		cnt += 64;	
					decrypted(&cbuf[cnt]);		cnt += 64;	
					decrypted(&cbuf[cnt]);		cnt += 64;
					
					memcpy(&accnts.accnt[0].login[0], &cbuf[3], USART_BUF_SIZE);
					wr_flag_acc = 1;

		break;
		
		//......................................................................
		case 0x23:	if(size <  5)		{ return; }								//read logs
 					check_admin();
					cnt = 3; wn++;
					flash_read(LOGS_ADDR + (256 * cbuf[3]), &cbuf[4], 256);		wn +=256;
		break;
		//......................................................................
		case 0x27:	if(size  !=  5) { return; }									//change config
					check_admin();	
					
					if (wr_flag_cfg)
					{	
						if((crc16_ccit((BYTE*)&cfg_1_tmp,sizeof(CFG_1)) != 0))		{send_error();}		
						memcpy(&cfg_1,&cfg_1_tmp,sizeof(CFG_1));
						save_log(EEPROM);
						wn |=+cfg_save();
					}
					
					if (wr_flag_acc)
					{
						acc(WRITE);
						save_log(ACC_CHANGE);
					}
					
					
					reset=1;
		break;
		//......................................................................
		case 0x29:	if(size  !=  5) { return; }									//reboot
					check_admin();
					reset=1; wn++;
					send_admin();
		break;
		//......................................................................
		
		default :	   return;
	}
	
	cbuf[2]|=0x80;

	cs= crc16_ccit(cbuf,wn);
	
	cbuf[wn] = (BYTE)(cs & 0x00ff);         wn++;
	cbuf[wn] = (BYTE)((cs & 0xff00) >> 8);  wn++;
	
	if (wn > MAX_SIZE_BUF_SPI) {return;}
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
			if (!eth_sock[n_port].r_status){return;}										//check read stat
			
			if(!cfg_1.access[n_port - 1].en) {	if (!acces_ip(n_port - 1)) {return;} 	}	//check access ip
			
 			port[n_port-1].time_port = port[n_port-1].tout_port*10;							//launch timeout
		
			size = eth_sock[n_port].len[0] << 8 | eth_sock[n_port].len[1];				//give size
		
			if(size>USART_BUF_SIZE)	{eth_sock[n_port].r_status = 0; return;}			//check overload
			
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
						if (size > USART_BUF_SIZE) {return;}
						memcpy(eth_sock[n_port].data + MBAP_HDR_LEN, port[n_port-1].rbuf, size);
						size += MBAP_HDR_LEN - 2;
						eth_sock[n_port].data[5]=size-MBAP_HDR_LEN;
					break;
					default://GATE
						if (size > USART_BUF_SIZE) {return;}
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