#include "def.h"


CFG cfg;
CFG cfg_tmp;



void cfg_init(void)
//--------------------------------------------------------------------------
{
	memset(&cfg    ,0x00,sizeof(CFG));
	memset(&cfg_tmp,0x00,sizeof(CFG));
}

WORD cfg_read(void)
//--------------------------------------------------------------------------
{
	WORD size = 0;

	size = flash_read(CFG_ADDR,&cfg,sizeof(CFG));

	if(size != sizeof(CFG)) {return(CFG_ERR);}

	if(crc16_ccit((BYTE*)&cfg,sizeof(CFG))!=0){return(CFG_ERR);}

	return(CFG_OK);
}

void cfg_default(void)
//--------------------------------------------------------------------------
{
	WORD  i=0;

	cfg.guid = 0xFAC6097A;
	cfg.serial_n_L=0x04030201;
	cfg.serial_n_H=0x08070605;
	
	cfg.in_numb=102;
	
	//............................................................................

	cfg.com_network.src_port    = 5000;
	
	//mac 5A:5B:5C:5D:17:17 /SHAR
	cfg.com_network.mac_addr[0]=0x5A;
	cfg.com_network.mac_addr[1]=0x5b;
	cfg.com_network.mac_addr[2]=0x5c;
	cfg.com_network.mac_addr[3]=0x5d;
	cfg.com_network.mac_addr[4]=0x17;
	cfg.com_network.mac_addr[5]=0x17;
	
	//ip 192.168.1.102 /SIPR
	cfg.com_network.ip_addr[0]=0xC0;//192
	cfg.com_network.ip_addr[1]=0xA8;//168
	cfg.com_network.ip_addr[2]=0x01;//1
	cfg.com_network.ip_addr[3]=0x66;//102

	//mask 255.255.255.0 /SUBR
	cfg.com_network.ip_mask[0]=0xFF;
	cfg.com_network.ip_mask[1]=0xFF;
	cfg.com_network.ip_mask[2]=0xFF;
	cfg.com_network.ip_mask[3]=0x00;
	
	//gateway 192.168.1.1 /GAR
	cfg.com_network.ip_gate[0]=0xC0;
	cfg.com_network.ip_gate[1]=0xA8;
	cfg.com_network.ip_gate[2]=0x01;
	cfg.com_network.ip_gate[3]=0x01;

	

	//...........................................................................
	for(i=0;i<4;++i)
	{
		cfg.sock_rs485[i].mode		= UDP_MODE;
		cfg.sock_rs485[i].en		= TRUE;
		
		cfg.sock_rs485[i].src_port	=5001+i;
		
		cfg.sock_rs485[i].baud		=38400;
		cfg.sock_rs485[i].bsize		=8;
		cfg.sock_rs485[i].parity	=0;
		cfg.sock_rs485[i].stop		=0;
		cfg.sock_rs485[i].tout		=100;

	}

	cfg.cs=crc16_ccit(((BYTE *)&cfg),sizeof(CFG)-2);
	//..............................................................................
}

void cfg_check()
//--------------------------------------------------------------------------
{
	BYTE i = 0;
	
	for(i=0;i<4;i++)
	{
		port[i].tout_port=cfg.sock_rs485[i].tout;
		if ((port[i].tout_port < 50) || (port[i].tout_port > 1000))  {port[i].tout_port= 100;}
	}
	
	return;
}

WORD cfg_save(void)
//--------------------------------------------------------------------------
{
	if(!flash_empty(CFG_ADDR,512))
	{
		flash_erase_page(CFG_ADDR);
		flash_erase_page(CFG_ADDR+256);
	}
	
	flash_write(CFG_ADDR,(BYTE*)&cfg,sizeof(CFG));
			
	return(CFG_OK);
}

