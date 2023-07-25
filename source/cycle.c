#include "def.h"

WORD u_size = 0;//for usart_read
VAR var; //last ptr, command status, timer

void usart_process (BYTE n_port)
{
	WORD len_mes;
	if ( var.port_time[n_port-1] < eth_wait )	{var.port_time[n_port-1] = 0;};//check end block port
	switch(var.port_stat[n_port-1])
	{
		case UCMD_CH:
			if ((!var.port_time[n_port-1]) && (u_port[n_port].r_status))//block (0 - True) and check by read staus
			{
				var.port_stat[n_port-1] = UCMD_ETH_RS485; //status for switch case
				if ( eth_wait > TIMER_LMT)	{var.port_time[n_port-1] = eth_wait - TIMER_LMT;	return;}//check read timeout
				else						
					{if (PROC_HERZ == PROC_HERZ48)	{var.port_time[n_port-1] = eth_wait + TIMER_COEF_48;return;}
					else							{var.port_time[n_port-1] = eth_wait + TIMER_COEF_8 ;return;}}//check read timeout (alternative)
			}
			if (port[n_port-1].rx != port[n_port-1].rn)	
			{
				var.port_stat[n_port-1] = UCMD_RS485_ETH;
				return;
			}//проверка на чтение 
			
			
			if (MODUL_TELNET == OFF){return;}
			if ( (n_port == 5) && (u_port[TEL_SOCK].r_status) && (!var.port_time[n_port-1]) )// (5port - read mode on - timeout)
			{
			 	var.port_stat[n_port-1] = UCMD_TLN;
			 	if ( eth_wait > TIMER_LMT)	{var.port_time[TEL_SOCK] = eth_wait - TIMER_LMT;	return;}//check read timeout
			 	else						{var.port_time[TEL_SOCK] = eth_wait + TIMER_COEF;	return;}//check read timeout (alternative)
			 	return;
			}
			
		return;
		
		
		//__________________________________________________________________________________//	
		case UCMD_RS485_ETH://UP
			u_rd(n_port, (port[n_port-1].rn - port[n_port-1].rx) );   //give mess size
			if (u_size != 0)
			{
				memcpy(u_port[n_port].data, port[n_port-1].rbuf, u_size);//copy in buffer
				u_port[n_port].len[0] = (u_size & 0xFF00) >> 8;	u_port[n_port].len[1] = u_size & 0x00FF; //write mess size in port_udp
				u_port[n_port].w_status = 1;
			}
			var.port_stat[n_port-1] = UCMD_CH;
		return;
		
		
		//__________________________________________________________________________________//
		case UCMD_ETH_RS485://DWN
			if (cfg.sock_rs485[n_port-1].mode == TCP_MODE)	{len_mes = u_port[n_port].ptr_rx_buf - var.last_ptr_rx_buf[n_port-1];} 
			else                                   			{len_mes = u_port[n_port].ptr_rx_buf - var.last_ptr_rx_buf[n_port-1] - 8;}
			u_wr(n_port, len_mes);
			var.last_ptr_rx_buf[n_port-1] = u_port[n_port].ptr_rx_buf;//write last position pointer for compare 
			u_port[n_port].r_status = 0;// read_status off for correct work usart_proc
			var.port_stat[n_port-1] = UCMD_CH;
		return;
		
		
		//__________________________________________________________________________________//
		case UCMD_TLN:
			if (MODUL_TELNET == OFF){return;}
			tell_funx(n_port,&var.port_stat[TEL_SOCK-1]);
		return;
	}
}

//DWN
void u_wr (BYTE n_port, WORD len)//number number to funx
{
	switch (n_port)
	{
		case 1:
			usart_0_write( (u_port[n_port].data), len);//write in uart 0 (5001 port)
		break;
		case 2:
			usart_1_write( (u_port[n_port].data), len);//write in uart 1 (5002 port)
		break;
		case 3:
			usart_2_write( (u_port[n_port].data), len);//write in uart 2 (5003 port)
		break;
		case 4:
			usart_3_write( (u_port[n_port].data), len);//write in uart 3 (5004 port)
		break;	
	}
}

//UP
void u_rd (BYTE n_port, WORD len)//number number to funx
{
	switch (n_port)
	{
		case 1:
			u_size = usart_0_read((port[n_port-1].rbuf),len);//read from uart 0 (5001 port)
		break;
		case 2:
			u_size = usart_1_read((port[n_port-1].rbuf),len);//read from uart 1 (5002 port)
		break;
		case 3:
			u_size = usart_2_read((port[n_port-1].rbuf),len);//read from uart 2 (5003 port)
		break;
		case 4:
			u_size = usart_3_read((port[n_port-1].rbuf),len);//read from uart 3 (5004 port)
		break;
	}
}
