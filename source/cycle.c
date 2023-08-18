#include "def.h"

WORD u_size = 0;	//for usart_read
VAR  vars;			//last ptr, stage status, timer
DWORD tout[4];

void usart_process (BYTE n_port)
{
	WORD size=0;

	if(cfg.sock_rs485[n_port-1].en==FALSE) {return;} 
		
	switch(vars.stage[n_port-1])
	{
		case RS485_CHECK:	
			//ETH message check
			if (eth_sock[n_port].r_status)//block (0 - True) and check by read status
			{
				vars.stage[n_port-1] = RS485_WRITE;
				vars.time[n_port-1]  = time_eth_wait + port[n_port-1].tout_port;		
			}
		return;
		case RS485_WRITE://DWN
			if (cfg.sock_rs485[n_port-1].mode == TCP_MODE)	{size = eth_sock[n_port].ptr_rx_buf - vars.last_ptr_rx_buf[n_port-1];} 
			else                                   			{size = eth_sock[n_port].ptr_rx_buf - vars.last_ptr_rx_buf[n_port-1] - SKIP_HDR;}
			usart_write(n_port - 1, eth_sock[n_port].data, size);
			tout[n_port - 1] = time_eth_wait;
			
			vars.last_ptr_rx_buf[n_port-1] = eth_sock[n_port].ptr_rx_buf;//write last position pointer for compare 
			eth_sock[n_port].r_status = 0;// read_status off for correct work usart_proc
			vars.stage[n_port-1] = RS485_READ;
		return;
		case RS485_READ://UP
			u_size = usart_read(n_port - 1, port[n_port-1].rbuf, (port[n_port-1].rn - port[n_port-1].rx) );   //give mess size
			if (u_size != 0)
			{
				if (u_size > USART_BUF_SIZE){u_size = USART_BUF_SIZE;}
				memcpy(eth_sock[n_port].data, port[n_port-1].rbuf, u_size);//copy in buffer
				eth_sock[n_port].len[0]		= (u_size & 0xFF00) >> 8;	eth_sock[n_port].len[1] = u_size & 0x00FF; //write mess size in port_udp
				eth_sock[n_port].w_status	= 1;
				port[n_port-1].dt			= (WORD)(time_eth_wait-tout[n_port - 1]);
				vars.stage[n_port-1]		= RS485_CHECK;
				return;
			}
			if (vars.time[n_port-1]< time_eth_wait)
				{
					vars.stage[n_port-1] = RS485_CHECK;
					port[n_port-1].dt 	 = 0;
				}
			return;
	}
}