#include "def.h"

ETH_HDR eth_sock[MAX_SOCKETS_CNT];

void eth_init(void)
{
	w5500_init_reg();
	
	//common socket for settings
	if(socket_init(SOCKET_0,cfg_1.com_network.src_port,UDP)) {err_dword.common_sock = 1;}

	//port socket
	for(BYTE i=0; i<4; i++)
	{
		if(cfg_1.sock_rs485[i].en==FALSE) {i++; continue;} 
		if(socket_init(i+1,cfg_1.sock_rs485[i].src_port,cfg_1.sock_rs485[i].mode)) {err_dword.socket += (1 << i);}
	}
}

void eth_process(void)
{
	static BYTE eth_st=0;
	WORD rtrn = 0;
	switch(eth_st)
	{
		case CHECK:				
			rtrn = check_data_wr_process();	
			if(rtrn != NO_SOKET)								
			{
				eth_st					= WRITE_PROCESS;
				w5500_mode.mode_op		= MODE_OP_WRITE_UDP;
				w5500_mode.numb_socket	= rtrn;
				switch(rtrn)
				{
					case COMMON_SOCK_VAL:																						return;
					default:				if(cfg_1.sock_rs485[rtrn-1].mode == TCP) {w5500_mode.mode_op=MODE_OP_WRITE_TCP;}	return;
				} 
			}

			check_sockets_process((BYTE*)&w5500_mode);//select sockets for read			
			eth_st = READ_PROCESS;
		break;
		case READ_PROCESS:
			rtrn = w5500_process (w5500_mode.mode_op,w5500_mode.numb_socket);
			if(rtrn == 2)	{eth_st=0;break;}
			if(rtrn)		{eth_parse(w5500_mode.numb_socket,rtrn); eth_st=0;}
		break;
		case WRITE_PROCESS:
			rtrn = w5500_process(w5500_mode.mode_op,w5500_mode.numb_socket);
			if(rtrn)		{eth_st=0;}
		break;
		default:
			eth_st = CHECK;
		break;

	}
}

void check_sockets_process (BYTE *buf)
{
	static BYTE index = MAX_SOCKETS_VAL;
	
	index++;
	if(index > MAX_SOCKETS_VAL){index=0;}
	
	switch (index)
	{
		case COMMON_SOCK_VAL:
			w5500_mode.numb_socket	= SOCKET_0;
			w5500_mode.mode_op		= MODE_OP_READ_UDP;
		return;
		default:
			w5500_mode.numb_socket = index;
			if (cfg_1.sock_rs485[index-1].mode == TCP)	
			{w5500_mode.mode_op = MODE_OP_READ_TCP;}
			else										
			{w5500_mode.mode_op = MODE_OP_READ_UDP;}
		return;
	}									{w5500_mode.mode_op=MODE_OP_READ_UDP;}
	return;
}

void eth_parse (BYTE numb_sock,WORD size)
{	
	if(eth_sock[numb_sock].r_status == 1)	{ return;}
		
	eth_sock[numb_sock].counters.rx++;
	eth_sock[numb_sock].r_status = 1;
}

BYTE check_data_wr_process (void)
{
	BYTE numb_sock=0;

	for( numb_sock = 0; numb_sock < MAX_SOCKETS_CNT; numb_sock++ )
	{
		if( eth_sock[numb_sock].w_status == 1 )
		{
			eth_sock[numb_sock].w_status = 0;
			eth_sock[numb_sock].counters.tx++;
			return (numb_sock);
		}
	}
	return (NO_SOKET);	
}
