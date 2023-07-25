#include "def.h"

BYTE cbuf[512];
BYTE n_port=1;

#define CM2_WHO_ARE_YOU 0x01
#define UID_WHO_ARE_YOU 0x8001

void cmd_process(void)
{
cmd_common_process ();

cmd_usart_process ();
}


void cmd_common_process (void)
{
	WORD  size = 0;
	WORD  addr = 0;
	WORD  ixo  = 0;
	WORD  cnt  = 0;
    WORD  wn   = 0;
	WORD  cs   = 0;
	
	if(u_port[0].r_status==FALSE) {return;}	
	u_port[0].r_status=FALSE;
	
	//size=size_data_sock(0);
	size=((u_port[0].len[0]<<8) | (u_port[0].len[1]));
		
	memcpy(cbuf,(BYTE*)&u_port[0].data,size);

	
	
	if(size						  <    5)	{ return; }
	if(size                       >  1024)	{ return; }
		
	if(crc16_ccit(cbuf,size)   !=   0)		{ return; }	
	
	addr  = *(__packed WORD*)(cbuf+0);      wn=+sizeof(WORD);

    if(addr !=  0){return;}
	wn++;//cmd

	switch(cbuf[2])
	{
		case 0x01:                
									if(cbuf[wn]==0x01)
										{
										   
										cbuf[wn]  = CM2_WHO_ARE_YOU;									wn += sizeof(BYTE);
										cbuf[wn]  = (BYTE)UID_WHO_ARE_YOU;								wn += sizeof(BYTE);
										cbuf[wn]  = (BYTE)(UID_WHO_ARE_YOU>>8);							wn += sizeof(BYTE);
										memcpy(&cbuf[wn],MODEL,strlen(MODEL));							wn+=strlen(MODEL);
										memcpy(&cbuf[wn],((BYTE*)&cfg.com_network.mac_addr),6);			wn+=6;
										memcpy(&cbuf[wn],VERSION,strlen(VERSION));						wn+=strlen(VERSION);
										}
		break;										
		//......................................................................
		case 0x07:  				
										if(size != 7) { return; }             // CMD=0x07 Read CFG
		
										//ixo=*(__packed WORD*)(cbuf+wn);									wn+=sizeof(WORD);
									    ixo=cbuf[3]<<8 | cbuf[4];											wn+=sizeof(WORD);									
										cnt= sizeof(CFG) - ixo;

										if(ixo > sizeof(CFG)) { break ;  }
										if(cnt > 512       ) { cnt=512;  }

										cbuf[wn]=cnt>>8; cbuf[wn+1]=cnt & 0x00FF;							wn+=sizeof(WORD);
										
										//*(__packed WORD*)(cbuf[wn])=cnt;									wn+=sizeof(WORD);
										
										memcpy(&cbuf[wn],((BYTE*)&cfg)+ixo,cnt);						wn+=cnt;
		break;

		//......................................................................
		case 0x10:						if(size <  6) { return; }	
										wn+=iap_process(&cbuf[wn],size-5);
		break;

		//......................................................................
		case 0x17:						if(size  <  7) { return; }
		
										ixo=cbuf[3]<<8 | cbuf[4];	 wn+=sizeof(WORD);            // CMD=0x17 Send CFG
										cnt=sizeof(CFG) - ixo;		 wn+=sizeof(WORD);
		
										if(cnt  ==  0) { break; }
										if(ixo  ==  0)
										{
											memset(&cfg_tmp,0x00,sizeof(CFG));
										}
		
										if(ixo       >= sizeof(CFG)) { break; }
										if((cnt+ixo) >  sizeof(CFG)) { break; }
		
										memcpy(((BYTE*)&cfg_tmp)+ixo,cbuf+wn                  ,cnt);
										memcpy(cbuf+wn                  ,((BYTE*)&cfg_tmp)+ixo,cnt);
										wn+=cnt;
		break;
		//......................................................................
		case 0x26:						if(size  !=  5) { return; }
				
										cfg_drop();
										reset=1;
		break;
		//......................................................................
		case 0x27:						if(size  !=  5) { return; } 
				
										if(crc16_ccit((BYTE*)&cfg_tmp,sizeof(CFG)) != 0)
										{
											break;
										}
		
										memcpy(&cfg,&cfg_tmp,sizeof(CFG));
										wn =+cfg_save();			
										reset=1;
		break;
		//......................................................................
		default :	   return;
	}
	
	cbuf[2]|=0x80;

	cs= crc16_ccit(cbuf,wn);  
	
	cbuf[wn] = (BYTE)(cs & 0x00ff);         wn++;
	cbuf[wn] = (BYTE)((cs & 0xff00) >> 8);  wn++;
	

	memcpy((BYTE*)&u_port[0].data,cbuf,wn);
	
	

	u_port[0].len[0]=((wn & 0xFF00)>>8);
	u_port[0].len[1]=(wn & 0x00FF);
	u_port[0].w_status=1;

}

void cmd_usart_process (void)
{	
	if(n_port==5){n_port=1;}
	usart_process(n_port);
	n_port++;
 }