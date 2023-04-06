#include "def.h"

WORD last_ptr_rx_buf[]  = {0x00,0x00,0x00,0x00};
BYTE uart_cmd = 0;
WORD u_size = 0;
WORD a = 0;

void usart_process (BYTE n_port)//����
{
	switch(uart_cmd)
	{
		case UCMD_CH:
			if (port_udp[n_port].r_status)				{uart_cmd = UCMD_WR;return;}
			if (port[n_port-1].rx != port[n_port-1].rn) {uart_cmd = UCMD_RD;return;}
		break;
			
		case UCMD_WR:
			u_wr(n_port, ( (port_udp[n_port].ptr_rx_buf - last_ptr_rx_buf[n_port-1] ) - 8));//��������� (����, ����, �����(����� ����))
			last_ptr_rx_buf[n_port-1] = port_udp[n_port].ptr_rx_buf;//������ ���������� ��������� ��������� ��� ��������� ��� ���������
			port_udp[n_port].r_status = 0;// read_status ���� (��� ���������� ������ ������� ������������ ������� ������ ��������� � usart_proc
			uart_cmd = UCMD_CH;
		break;
			
		case UCMD_RD:
			u_rd(n_port, (port[n_port-1].rn - port[n_port-1].rx) );   //�������� ������ ���������
			if (u_size != 0)
			{
				memcpy(port_udp[n_port].data, port[n_port-1].rbuf, u_size);//��������� � �����
				port_udp[n_port].len[0] = (u_size & 0xFF00) >> 8;	port_udp[n_port].len[1] = u_size & 0x00FF; //�������� ������� ��������� port_udp
				port_udp[n_port].w_status = 1;// �������� �� ������
			}
			uart_cmd = UCMD_CH;
		break;
	}
}

void u_wr (BYTE n_port, WORD len)//����������� ����� � ��������������� �������
{
	switch (n_port)
	{
		case 1:
			usart_0_write( (port_udp[n_port].data), len);//������ � uart 0 (5001 port)
			break;
		case 2:
			usart_1_write( (port_udp[n_port].data), len);//������ � uart 1 (5002 port)
			break;
		case 3:
			usart_2_write( (port_udp[n_port].data), len);//������ � uart 2 (5003 port)
			break;
		case 4:
			usart_3_write( (port_udp[n_port].data), len);//������ � uart 3 (5004 port)
			break;	
	}
}

void u_rd (BYTE n_port, WORD len)//����������� ����� � ��������������� �������
{
	switch (n_port)
	{
		case 1:
			u_size = usart_0_read((port[n_port-1].rbuf),len);//������ �� uart 0 (5001 port)
			break;
		case 2:
			u_size = usart_1_read((port[n_port-1].rbuf),len);//������ �� uart 1 (5002 port)
			break;
		case 3:
			u_size = usart_2_read((port[n_port-1].rbuf),len);//������ �� uart 2 (5003 port)
			break;
		case 4:
			u_size = usart_3_read((port[n_port-1].rbuf),len);//������ �� uart 3 (5004 port)
			break;
	}
	//return u_size;
}