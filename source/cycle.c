#include "def.h"

BYTE mess[] = {0x00,0x00,0x03,0xFC,0xFF};
BYTE testBuf[20];

BYTE lasta[]  = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

BYTE messN = 0x00;

void usart_process (BYTE n_port,BYTE* cbuf_p)
{
	BYTE* e_cb_p = eth_cbuf_ptr();
	if ( (port_udp[n_port].len[0] != lasta[n_port*2+0]) | (port_udp[n_port].len[1] != lasta[n_port*2+1]) ) //проверка на наличие нового сообщения в eth
	{
		usart_forward_down(n_port,e_cb_p);
	} 

	if (port[n_port-1].rx != port[n_port-1].rn) //проверка на занятость чтением
	{
		usart_forward_up(n_port, cbuf_p, e_cb_p);
	} 
	else {return;}
}

void usart_write (BYTE n_port,BYTE* mess,int len)//преобразует цифру в соответствующую функцию
{
	switch (n_port)
	{
		case 1:
			usart_0_write(mess,len);
			break;
		case 2:
			usart_1_write(mess,len);
			//messN--;
			break;
		case 3:
			usart_2_write(mess,len);
			//messN--;
			break;
		case 4:
			usart_3_write(mess,len);
			//messN--;
			break;	
	}
}

// void usart_write (BYTE n_port,BYTE* mess,int len)//преобразует цифру в соответствующую функцию
// {
// 	switch (n_port)
// 	{
// 		case 0:
// 		usart_0_write(mess,len);
// 		break;
// 		case 1:
// 		usart_1_write(mess,len);
// 		//messN--;
// 		break;
// 		case 2:
// 		usart_2_write(mess,len);
// 		//messN--;
// 		break;
// 		case 3:
// 		usart_3_write(mess,len);
// 		//messN--;
// 		break;
// 	}
// }

void usart_read (BYTE n_port,BYTE* mess,int len)//преобразует цифру в соответствующую функцию
{
	switch (n_port)
	{
		case 1:
		usart_0_read(mess,len);
		break;
		case 2:
		usart_1_read(mess,len);
		//messN--;
		break;
		case 3:
		usart_2_read(mess,len);
		//messN--;
		break;
		case 4:
		usart_3_read(mess,len);
		//messN--;
		break;
	}
	if (testBuf[2] != 0 )
	{
		testBuf[0] = 0x01;
	}
}

// void usart_read (BYTE n_port,BYTE* mess,int len)//преобразует цифру в соответствующую функцию
// {
// 	switch (n_port)
// 	{
// 		case 0:
// 			usart_0_read(mess,len);
// 			break;
// 		case 1:
// 			usart_1_read(mess,len);
// 			//messN--;
// 			break;
// 		case 2:
// 			usart_2_read(mess,len);
// 			//messN--;
// 			break;
// 		case 3:
// 			usart_3_read(mess,len);
// 			//messN--;
// 			break;
// 	}
// 	if (testBuf[2] != 0 )
// 	{
// 		testBuf[0] = 0x01;
// 	}
// }

void usart_forward_down (BYTE n_port, BYTE* e_cb_p)//eth to uart
{
	int a = (port_udp[n_port].len[0]<<8) | (port_udp[n_port].len[1]);
	usart_write(n_port,&e_cb_p[8],a);
	lasta[n_port*2+0] = port_udp[n_port].len[0];
	lasta[n_port*2+1] = port_udp[n_port].len[1];
}

void usart_forward_up (BYTE n_port, BYTE* cbuf_p, BYTE* e_cb_p)//uart to eth
{
	BYTE a = port[n_port].rn - port[n_port].rx;
	usart_read(n_port,cbuf_p,a);
	// место для сдвига поинтеров
	memcpy(e_cb_p,cbuf_p,a);
	port_udp[n_port].w_status = 1;
}