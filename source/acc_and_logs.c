#include "def.h"

void acc(BYTE cmd)//command(читать, записать, сбросить)
{
	if (sizeof(ACCOUNTS) != 256) {warning_led(3);return;}
		
	switch(cmd)
	{
		case READ:
			memcpy(&accnts.accnt[0].login[0], ACCOUNT_PLACE, sizeof(ACCOUNTS));
		break;
		case WRITE:
			if(!flash_empty(ACCOUNT_PLACE,256))	{flash_erase_page(ACCOUNT_PLACE);}
			flash_write(ACCOUNT_PLACE, &accnts.accnt[0].login[0], sizeof(ACCOUNTS));
		break;
		case DROP:

			memset(&accnts.accnt[0].login[0], 0, 64);
			memcpy(&accnts.accnt[0].login[0], ADMIN, sizeof(ADMIN));
			memcpy(&accnts.accnt[0].password[0], ADMIN, sizeof(ADMIN));

			memset(&accnts.accnt[1].login[0], 0, 64);
			memcpy(&accnts.accnt[1].login[0], USER, sizeof(USER));
			memcpy(&accnts.accnt[1].password[0], USER, sizeof(USER));
			
			accnts.cs = crc16_ccit(&accnts.accnt[0].login, sizeof(ACCOUNTS));
			
			if(!flash_empty(ACCOUNT_PLACE,256))	{flash_erase_page(ACCOUNT_PLACE);}
			flash_write(ACCOUNT_PLACE, &accnts.accnt[0].login[0] , sizeof(ACCOUNTS));
		break;
		default:			
		break;
	}
}

BYTE acces_ip(BYTE n_port)//True-1,F-0;
{
	if		(SRAV(4, &cfg_1.access[n_port].ip[0], eth_sock[n_port+1].ip_addr)) {return 1;}
	if		(SRAV(4, &cfg_1.access[n_port].ip[1], eth_sock[n_port+1].ip_addr)) {return 1;}
	if		(SRAV(4, &cfg_1.access[n_port].ip[2], eth_sock[n_port+1].ip_addr)) {return 1;}
	if		(SRAV(4, &cfg_1.access[n_port].ip[3], eth_sock[n_port+1].ip_addr)) {return 1;}
	return 0;
}

void log_ch( )
{
	if (!flash_empty(LOGS_ADDR, 1024))/*1024 * 4(DWORD)*/{return;}
	
	LOG_DATA data;
	data.cell[0].ip				= 0x00000000;
	data.cell[0].times			= 0x00000000;
	data.cell[0].indx			= 0;
	data.cell[0].operat_code	= START;
	data.cell[0].version		= VERSION_B;
	flash_write(LOGS_ADDR ,&data.cell[0] , 16);
}

void log_safe(BYTE actv_user_id, DWORD ip, DWORD times, WORD operat_code)
{
	LOG_DATA data;
	flash_read(LOGS_ADDR, &data, 4096);
	BYTE ptr = 1;
	BYTE last_ptr = 0;
	BYTE* dst;
	
	while(1)	
	{
		if (ptr == CELLS)											{ptr = 0; break;}//OVF
		if ((data.cell[last_ptr].indx + 1) != data.cell[ptr].indx)  { break;}
		last_ptr++;	
		ptr++;	
	}
	data.cell[ptr].ip			= ip;
	data.cell[ptr].times		= times;
	data.cell[ptr].indx			= data.cell[last_ptr].indx + 1;
	data.cell[ptr].operat_code	= operat_code;
	data.cell[ptr].version		= VERSION_B;
	
	dst = LOGS_ADDR + (ptr * CELL);
		
	if ((ptr % 16) == 0) 
	{
		flash_erase_row(LOGS_ADDR + ((ptr/CELL) * BLOCK));
	} 
	flash_write(dst, &data.cell[ptr].ip, 16);
}

void log_clear()
{
	for (BYTE i = 0; i < 16; i++)	
	{
		if(!flash_empty(LOGS_ADDR + (i * 256), 256))	
		{flash_erase_page(LOGS_ADDR + (i * 256));}	
	}
}
