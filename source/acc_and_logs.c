#include "def.h"

void acc(BYTE cmd)//command(читать, записать, сбросить)
{
	if (sizeof(ACCOUNTS) != 256) {cmd = 109;}
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
			warning_led(3);
		break;
	}
}

BYTE acces_ip(BYTE n_port)//True-1,F-0;
{
	BYTE ch = 0;
	if		(SRAV(4, &cfg_1.access[n_port].ip[0], eth_sock[n_port+1].ip_addr)) {ch++;}
	else if (SRAV(4, &cfg_1.access[n_port].ip[1], eth_sock[n_port+1].ip_addr)) {ch++;}
	else if (SRAV(4, &cfg_1.access[n_port].ip[2], eth_sock[n_port+1].ip_addr)) {ch++;}
	else if (SRAV(4, &cfg_1.access[n_port].ip[3], eth_sock[n_port+1].ip_addr)) {ch++;}
	return ch;
}

void log_ch( )
{
	WORD ch;
	flash_read(LOGS_ADDR, &ch, 2);
	if(ch != 0xFF) {return;}
	
	LOG_DATA data;
	data.ptr = 0;
	data.cell[0].inform.indx = 0;
	data.cell[0].times = 0x00000109;
	data.cell[0].ip = 0x00010009;
	data.cell[0].inform.operat_code = 109;
	memset(&data.rsv[0] , 1, 7);
	
	log_clear();
	flash_write(LOGS_ADDR,(BYTE*)&data.ptr, 256);
	
	return(CFG_OK);
}

void log_safe(BYTE actv_user_id, DWORD ip, DWORD times, BYTE operat_code)
{
	LOG_DATA data;
	BYTE indx;
	flash_read(LOGS_ADDR, &data.ptr, 2048);
	indx = data.cell[data.ptr].inform.indx;
	data.ptr++;
	if (data.ptr > 170)	{data.ptr = 1;}
	data.cell[data.ptr].ip = ip;
	data.cell[data.ptr].times = times;
	data.cell[data.ptr].inform.indx = indx + 1;
	data.cell[data.ptr].inform.operat_code = operat_code;
	
	log_clear();
	flash_write(LOGS_ADDR, (BYTE*)&data, sizeof(LOG_DATA));
	
	return(CFG_OK);
}

void log_clear()
{
	for (BYTE i = 0; i < 8; i++)	
	{
		if(!flash_empty(LOGS_ADDR + (i * 256), 256))	
		{flash_erase_page(LOGS_ADDR + (i * 256));}	
	}
}
