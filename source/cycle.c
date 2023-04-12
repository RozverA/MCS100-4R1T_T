#include "def.h"

WORD last_ptr_rx_buf[]  = {0x00,0x00,0x00,0x00};
BYTE port_stat[] = {UCMD_CH,UCMD_CH,UCMD_CH,UCMD_CH};
WORD port_time[] = {0x00,0x00,0x00,0x00};
WORD u_size = 0;

void usart_process (BYTE n_port)//вход
{
	if ( port_time[n_port-1] > port[0].rtime )	{port_time[n_port-1] = 0;};//проверка на завершение блокировки порта
	switch(port_stat[n_port-1])
	{
		case UCMD_CH:
			if (!port_time[n_port-1])//блокировка (0 - True)
			{
				if (port_udp[n_port].r_status)//проверка наличи€ новых данных
				{
					port_stat[n_port-1] = UCMD_RD; //статус дл€ кейса
					if ( (port[n_port-1].rtime >> 24) == 0x000F )	{port_time[n_port-1] = (port[n_port-1].rtime - 0xF000);}//проверка по переполнению и назначение проверочного времени
					else {port_time[n_port-1] = port[0].rtime + TC3_100m;}//альтернативна€ установка времени проверки
					return;
				}
			}
			if (port[n_port-1].rx != port[n_port-1].rn) {port_stat[n_port-1] = UCMD_WR;return;}
		break;
			
		case UCMD_WR://UP
			u_rd(n_port, (port[n_port-1].rn - port[n_port-1].rx) );   //получить размер сообщени€
			if (u_size != 0)
			{
				memcpy(port_udp[n_port].data, port[n_port-1].rbuf, u_size);//поместить в буфер
				port_udp[n_port].len[0] = (u_size & 0xFF00) >> 8;	port_udp[n_port].len[1] = u_size & 0x00FF; //указание размера сообщени€ port_udp
				port_udp[n_port].w_status = 1;// указание на запись
			}
			port_stat[n_port-1] = UCMD_CH;
		break;
			
		case UCMD_RD://DWN
			u_wr(n_port, ( (port_udp[n_port].ptr_rx_buf - last_ptr_rx_buf[n_port-1] ) - 8));//прочитать (порт, куда, длина(сумма байт))
			last_ptr_rx_buf[n_port-1] = port_udp[n_port].ptr_rx_buf;//запись последнего положени€ указател€ дл€ сравнени€ при изменении
			port_udp[n_port].r_status = 0;// read_status выкл (дл€ корректной работы услови€ провер€ющего наличие нового сообщени€ в usart_proc
			port_stat[n_port-1] = UCMD_CH;
		break;
	}
}

void u_wr (BYTE n_port, WORD len)//преобразует цифру в соответствующую функцию
{
	switch (n_port)
	{
		case 1:
			usart_0_write( (port_udp[n_port].data), len);//запись в uart 0 (5001 port)
			break;
		case 2:
			usart_1_write( (port_udp[n_port].data), len);//запись в uart 1 (5002 port)
			break;
		case 3:
			usart_2_write( (port_udp[n_port].data), len);//запись в uart 2 (5003 port)
			break;
		case 4:
			usart_3_write( (port_udp[n_port].data), len);//запись в uart 3 (5004 port)
			break;	
	}
}

void u_rd (BYTE n_port, WORD len)//преобразует цифру в соответствующую функцию
{
	switch (n_port)
	{
		case 1:
			u_size = usart_0_read((port[n_port-1].rbuf),len);//чтение из uart 0 (5001 port)
			break;
		case 2:
			u_size = usart_1_read((port[n_port-1].rbuf),len);//чтение из uart 1 (5002 port)
			break;
		case 3:
			u_size = usart_2_read((port[n_port-1].rbuf),len);//чтение из uart 2 (5003 port)
			break;
		case 4:
			u_size = usart_3_read((port[n_port-1].rbuf),len);//чтение из uart 3 (5004 port)
			break;
	}
}