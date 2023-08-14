#include "def.h"

WORD u_size = 0;//for usart_read
VAR var; //last ptr, command status, timer

void usart_process (BYTE n_port)
{
	WORD size;
	DWORD timer_lim = (0xFFFF - (cfg.sock_rs485[n_port-1].tout * 1.2));
	
	if ( var.time[n_port-1] < eth_wait )	{var.time[n_port-1] = 0;};//check end block port
	switch(var.stage[n_port-1])
	{
		case UCMD_CH:
			
			//ETH message check
			if ((!var.time[n_port-1]) && (eth_sock[n_port].r_status))//block (0 - True) and check by read status
			{
				var.stage[n_port-1] = UCMD_ETH_RS485;
				if ( eth_wait > timer_lim)	{var.time[n_port-1] = eth_wait - timer_lim;	return;}//check read timeout
				else						{var.time[n_port-1] = eth_wait + TIMER_COEF;	return;}
			}
			//RS485 message check
			if (port[n_port-1].rx != port[n_port-1].rn)	{var.stage[n_port-1] = UCMD_RS485_ETH;}
		return;
		
		
		//__________________________________________________________________________________//	
		case UCMD_RS485_ETH://UP
			u_size = usart_read(n_port - 1, port[n_port-1].rbuf, (port[n_port-1].rn - port[n_port-1].rx) );   //give mess size
			if (u_size != 0)
			{
				if (u_size > DEFAULT_MTU_TCP){u_size = DEFAULT_MTU_TCP;}
				memcpy(eth_sock[n_port].data, port[n_port-1].rbuf, u_size);//copy in buffer
				eth_sock[n_port].len[0] = (u_size & 0xFF00) >> 8;	eth_sock[n_port].len[1] = u_size & 0x00FF; //write mess size in port_udp
				eth_sock[n_port].w_status = 1;
			}
			var.stage[n_port-1] = UCMD_CH;
		return;
		
		
		//__________________________________________________________________________________//
		case UCMD_ETH_RS485://DWN
			if (cfg.sock_rs485[n_port-1].mode == TCP_MODE)	{size = eth_sock[n_port].ptr_rx_buf - var.last_ptr_rx_buf[n_port-1];} 
			else                                   			{size = eth_sock[n_port].ptr_rx_buf - var.last_ptr_rx_buf[n_port-1] - 8;}
			usart_write(n_port - 1, eth_sock[n_port].data, size);
			var.last_ptr_rx_buf[n_port-1] = eth_sock[n_port].ptr_rx_buf;//write last position pointer for compare 
			eth_sock[n_port].r_status = 0;// read_status off for correct work usart_proc
			var.stage[n_port-1] = UCMD_CH;
		return;
	}
}