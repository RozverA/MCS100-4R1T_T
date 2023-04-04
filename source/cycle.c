#include "def.h"

WORD lasta[]  = {0x00,0x00,0x00,0x00};

void usart_process (BYTE n_port,BYTE* cbuf_p)//����
{
	BYTE* e_cb_p = eth_cbuf_ptr();//�������� ��������� �� ������ eth_cbuf
	if ( port_udp[n_port].ptr_rx_buf != lasta[n_port-1])//�������� �� ������ ��������� ������
	{
		usart_forward_down(n_port,e_cb_p);//��������� �������� eth -> uart
	} 

	if (port[n_port-1].rx != port[n_port-1].rn) //�������� �� ������� ������ ��������� � uart
	{
		usart_forward_up(n_port, cbuf_p);//��������� �������� uart -> eth
	} 
	else {return;}
}

void usart_write (BYTE n_port,BYTE* e_cb_p8,WORD len)//����������� ����� � ��������������� �������
{
	switch (n_port)
	{
		case 1:
			usart_0_write(e_cb_p8,len);//������ � uart 0 (5001 port)
			break;
		case 2:
			usart_1_write(e_cb_p8,len);//������ � uart 1 (5002 port)
			break;
		case 3:
			usart_2_write(e_cb_p8,len);//������ � uart 2 (5003 port)
			break;
		case 4:
			usart_3_write(e_cb_p8,len);//������ � uart 3 (5004 port)
			break;	
	}
}

WORD usart_read (BYTE n_port,BYTE* mess,int len)//����������� ����� � ��������������� �������
{
	WORD size;
	switch (n_port)
	{
		case 1:
			size = usart_0_read(mess,len);//������ �� uart 0 (5001 port)
			break;
		case 2:
			size = usart_1_read(mess,len);//������ �� uart 1 (5002 port)
			break;
		case 3:
			size = usart_2_read(mess,len);//������ �� uart 2 (5003 port)
			break;
		case 4:
			size = usart_3_read(mess,len);//������ �� uart 3 (5004 port)
			break;
	}
	return size;
}

void usart_forward_down (BYTE n_port, BYTE* e_cb_p)//eth to uart
{
	int a = (port_udp[n_port].ptr_rx_buf - lasta[n_port-1]) - 8;
	usart_write(n_port,&e_cb_p[8],a);//��������� (����, ����, �����(����� ����))
	lasta[n_port-1] = port_udp[n_port].ptr_rx_buf;//������ ���������� ��������� ��������� ��� ��������� ��� ���������
}

void usart_forward_up (BYTE n_port, BYTE* cbuf_p)//uart to eth
{
	
	WORD size = usart_read(n_port,cbuf_p,(port[n_port-1].rn - port[n_port-1].rx));//�������� ������ ���������
	if (size != 0)
	{
		memcpy(port_udp[n_port].data,cbuf_p,size);//��������� � �����
		port_udp[n_port].len[0] = (size & 0xFF00) >> 8;
		port_udp[n_port].len[1] = size & 0x00FF;
		port_udp[n_port].w_status = 1;
	}
}