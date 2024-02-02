#include "def.h"

#define NAME "MCS100-4R1T"
CFG_1 cfg_1;
CFG_1 cfg_1_tmp;
CFG_2 cfg_2;

void cfg_init(void)
//--------------------------------------------------------------------------
{
	memset(&cfg_1    ,0x00,sizeof(CFG_1));
	memset(&cfg_1_tmp,0x00,sizeof(CFG_1));
}

WORD cfg_read(void)
//--------------------------------------------------------------------------
{
	WORD size = 0;

	size = flash_read(CFG_1_ADDR,&cfg_1,sizeof(CFG_1));

	if(size != sizeof(CFG_1)) {return(CFG_ERR);}

	if(crc16_ccit((BYTE*)&cfg_1,sizeof(CFG_1))!=0){return(CFG_ERR);}

	return(CFG_OK);
}

WORD cfg_2_read(void)
//--------------------------------------------------------------------------
{
	WORD size = 0;

	size = flash_read(CFG_2_ADDR,&cfg_2,sizeof(CFG_2));

	if(size != sizeof(CFG_2)) {return(CFG_ERR);}

	if(crc16_ccit((BYTE*)&cfg_2,sizeof(CFG_2))!=0){return(CFG_ERR);}

	return(CFG_OK);
}

void cfg_default(void)
//--------------------------------------------------------------------------
{
	WORD  i=0;

	cfg_1.guid = 0xFAC6097A;
	
	cfg_1.in_numb=102;
	
	//............................................................................

	cfg_1.com_network.src_port    = 4000;
	
	//ip 192.168.1.102 /SIPR
	cfg_1.com_network.ip_addr[0]=0xC0;//192
	cfg_1.com_network.ip_addr[1]=0xA8;//168
	cfg_1.com_network.ip_addr[2]=0x01;//1
	cfg_1.com_network.ip_addr[3]=0x65;//101

	//mask 255.255.255.0 /SUBR
	cfg_1.com_network.ip_mask[0]=0xFF;
	cfg_1.com_network.ip_mask[1]=0xFF;
	cfg_1.com_network.ip_mask[2]=0xFF;
	cfg_1.com_network.ip_mask[3]=0x00;
	
	//gateway 192.168.1.1 /GAR
	cfg_1.com_network.ip_gate[0]=0xC0;
	cfg_1.com_network.ip_gate[1]=0xA8;
	cfg_1.com_network.ip_gate[2]=0x01;
	cfg_1.com_network.ip_gate[3]=0x01;
	
	memcpy(cfg_1.name, NAME, sizeof(NAME));
	cfg_1.name[sizeof(NAME) + 1] = 0x00;
	
	//...........................................................................
	for(i=0;i<4;++i)
	{
		cfg_1.sock_rs485[i].mode		= UDP;
		cfg_1.sock_rs485[i].pl			= GATE;
		cfg_1.sock_rs485[i].en			= FALSE;
		
		cfg_1.sock_rs485[i].src_port	=4001+i;
		
		cfg_1.sock_rs485[i].baud		=38400;
		cfg_1.sock_rs485[i].bsize		=8;
		cfg_1.sock_rs485[i].parity		=0;
		cfg_1.sock_rs485[i].stop		=0;
		cfg_1.sock_rs485[i].tout		=100;
		
		memset(cfg_1.access[i].ip, 0, 4*4);
		cfg_1.access[i].en = 1;

	}
	cfg_1.cs=crc16_ccit(((BYTE *)&cfg_1),sizeof(CFG_1)-2);
	//..............................................................................
	cfg_save();
	log_clear();
	acc(DROP);
}

void cfg_check()
//--------------------------------------------------------------------------
{
	BYTE i = 0;
	
	for(i=0;i<4;i++)
	{
		port[i].tout_port=cfg_1.sock_rs485[i].tout;
		if ((port[i].tout_port < 50) || (port[i].tout_port > 2000))  {port[i].tout_port= 100;}
	}
	
	return;
}

WORD cfg_save(void)
//--------------------------------------------------------------------------
{
	if(!flash_empty(CFG_1_ADDR,256))
	{
		flash_erase_page(CFG_1_ADDR);
	}
	
	flash_write(CFG_1_ADDR,(BYTE*)&cfg_1,sizeof(CFG_1));
			
	return(CFG_OK);
}

