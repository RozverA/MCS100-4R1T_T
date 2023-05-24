#include "def.h"

WORD last_ptr_rx_buf[]  = {0x00,0x00,0x00,0x00};
BYTE port_stat[] = {UCMD_CH,UCMD_CH,UCMD_CH,UCMD_CH};
WORD port_time[] = {0x0000,0x0000,0x0000,0x0000};
WORD u_size = 0;

void usart_process (BYTE n_port)
{
	WORD len_mes;
	if ( port_time[n_port-1] < eth_wait )	{port_time[n_port-1] = 0;};//check end block port
	switch(port_stat[n_port-1])
	{
		case UCMD_CH:
			if ((!port_time[n_port-1]) && (port_udp[n_port].r_status))//block (0 - True) and check by read staus
			{
				port_stat[n_port-1] = UCMD_DWN; //status for switch case
				if ( eth_wait > TIMER_LMT)	{port_time[n_port-1] = eth_wait - TIMER_LMT;	return;}//check read timeout
				else						{port_time[n_port-1] = eth_wait + TIMER_COEF;	return;}//check read timeout (alternative)
			}
			if (port[n_port-1].rx != port[n_port-1].rn)	
			{
				port_stat[n_port-1] = UCMD_UP;
				return;
			}//проверка на чтение 
		return;
			
		case UCMD_UP://UP
			u_rd(n_port, (port[n_port-1].rn - port[n_port-1].rx) );   //give mess size
			if (u_size != 0)
			{
				memcpy(port_udp[n_port].data, port[n_port-1].rbuf, u_size);//copy in buffer
				port_udp[n_port].len[0] = (u_size & 0xFF00) >> 8;	port_udp[n_port].len[1] = u_size & 0x00FF; //write mess size in port_udp
				port_udp[n_port].w_status = 1;
			}
			port_stat[n_port-1] = UCMD_CH;
		return;
			
		case UCMD_DWN://DWN
			if (cfg.sock_rs485[n_port-1].mode == TCP_MODE)	{len_mes = port_udp[n_port].ptr_rx_buf - last_ptr_rx_buf[n_port-1];} 
			else                                   			{len_mes = port_udp[n_port].ptr_rx_buf - last_ptr_rx_buf[n_port-1] - 8;}
			u_wr(n_port, len_mes);
			last_ptr_rx_buf[n_port-1] = port_udp[n_port].ptr_rx_buf;//write last position pointer for compare 
			port_udp[n_port].r_status = 0;// read_status off for correct work usart_proc
			port_stat[n_port-1] = UCMD_CH;
		return;
	}
}

//DWN
void u_wr (BYTE n_port, WORD len)//number number to funx
{
	switch (n_port)
	{
		case 1:
			usart_0_write( (port_udp[n_port].data), len);//write in uart 0 (5001 port)
		break;
		case 2:
			usart_1_write( (port_udp[n_port].data), len);//write in uart 1 (5002 port)
		break;
		case 3:
			usart_2_write( (port_udp[n_port].data), len);//write in uart 2 (5003 port)
		break;
		case 4:
			usart_3_write( (port_udp[n_port].data), len);//write in uart 3 (5004 port)
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
