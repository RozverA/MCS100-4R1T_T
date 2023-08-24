#include "def.h"

BYTE cbuf[600];
BYTE n_port=1;

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
						memcpy(&cbuf[wn],((BYTE*)&cfg.com_network.mac_addr),6);			wn+=6;
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
		
					cnt= sizeof(CFG);
					memcpy(&cbuf[wn],((BYTE*)&cfg),cnt);						wn+=cnt;
		break;

		//......................................................................
		case 0x10:	if(size <  6) { return; }
					wn+=iap_process(&cbuf[wn],size-5);
		break;

		//......................................................................
		case 0x17:	if(size  <  5) { return; }
		
					memcpy(((BYTE*)&cfg_tmp),cbuf+wn                  ,sizeof(CFG));
					memcpy(cbuf+wn                  ,((BYTE*)&cfg_tmp),sizeof(CFG));
					wn+=sizeof(CFG);
		break;
		//......................................................................
		case 0x27:	if(size  !=  5) { return; }
		
					if(crc16_ccit((BYTE*)&cfg_tmp,sizeof(CFG)) != 0)
					{
						break;
					}
		
					memcpy(&cfg,&cfg_tmp,sizeof(CFG));
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