#include "def.h"

WORD u_size = 0;//for usart_read
VAR var; //last ptr, command status, timer

void usart_process (BYTE n_port)
{
	WORD size;
	if ( var.port_time[n_port-1] < eth_wait )	{var.port_time[n_port-1] = 0;};//check end block port
	switch(var.port_stat[n_port-1])
	{
		case UCMD_CH:
			
			//ETH message check
			if ((!var.port_time[n_port-1]) && (u_port[n_port].r_status))//block (0 - True) and check by read staus
			{
				var.port_stat[n_port-1] = UCMD_ETH_RS485;
				if ( eth_wait > TIMER_LMT)	{var.port_time[n_port-1] = eth_wait - TIMER_LMT;	return;}//check read timeout
				else						{var.port_time[n_port-1] = eth_wait + TIMER_COEF;	return;}
			}
			//RS485 message check
			if (port[n_port-1].rx != port[n_port-1].rn)	{var.port_stat[n_port-1] = UCMD_RS485_ETH;}
		return;
		
		
		//__________________________________________________________________________________//	
		case UCMD_RS485_ETH://UP
			usart_read(n_port - 1, port[n_port-1].rbuf, (port[n_port-1].rn - port[n_port-1].rx) );   //give mess size
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
			if (cfg.sock_rs485[n_port-1].mode == TCP_MODE)	{size = u_port[n_port].ptr_rx_buf - var.last_ptr_rx_buf[n_port-1];} 
			else                                   			{size = u_port[n_port].ptr_rx_buf - var.last_ptr_rx_buf[n_port-1] - 8;}
			usart_write(n_port, u_port[n_port].data, size);
			var.last_ptr_rx_buf[n_port-1] = u_port[n_port].ptr_rx_buf;//write last position pointer for compare 
			u_port[n_port].r_status = 0;// read_status off for correct work usart_proc
			var.port_stat[n_port-1] = UCMD_CH;
		return;
	}
}