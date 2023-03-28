#include "def.h"

BYTE mess[] = {0x00,0x00,0x03,0xFC,0xFF};
BYTE testBuf[20];

BYTE messN = 0x00;

void usart_send_mess (BYTE n_port)
{
	if (port[n_port].wx == port[n_port].wn) //�������� �� ��������� �������
	{
		switch (messN)
		{
			case 0:
				usart_write(n_port, mess, sizeof(mess));//2
				messN++;//next mess
				break;
			case 1:
// 				usart_write(n_port, &test_mess,sizeof(test_mess));
// 				messN++;
 				break;
			case 2:
// 				usart_write(n_port, &an_stat,sizeof(an_stat));
// 				messN++;
 				break;
			case 3:
// 				usart_write(n_port, &VERSION,sizeof(VERSION));
// 				messN++;
				break;
		}
	}
	if (port[n_port].rx != port[n_port].rn)
	{
		usart_read(n_port,testBuf,port[n_port].rn - port[n_port].rx);
	} 
	else {return;}
}

void usart_write (BYTE n_port,BYTE* mess,int len)//����������� ����� � ��������������� �������
{
	switch (n_port)
	{
		case 0:
			usart_0_write(mess,len);
			break;
		case 1:
			usart_1_write(mess,len);
			//messN--;
			break;
		case 2:
			usart_2_write(mess,len);
			//messN--;
			break;
		case 3:
			usart_3_write(mess,len);
			//messN--;
			break;	
	}
}

void usart_read (BYTE n_port,BYTE* mess,int len)//����������� ����� � ��������������� �������
{
	switch (n_port)
	{
		case 0:
			usart_0_read(mess,len);
			break;
		case 1:
			usart_1_read(mess,len);
			//messN--;
			break;
		case 2:
			usart_2_read(mess,len);
			//messN--;
			break;
		case 3:
			usart_3_read(mess,len);
			//messN--;
			break;
	}
	if (testBuf[2] != 0 )
	{
		testBuf[0] = 0x01;
	}
}