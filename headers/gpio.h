#ifndef GPIO_H_
#define GPIO_H_

void led_init();
//define
	////pin control
		//////device
			#define LED				1
			#define RTS				0
			#define W55				2
		//////device name
			#define PWR				0
			#define PORT485_0		0
			#define PORT485_1		1
			#define PORT485_2		2
			#define PORT485_3		3
		//////work mode
			#define ON				1
			#define OFF				0
			#define TGL				2
			#define STAT			0
			#define SET				1 //Request to send (������ �� ��������) - ������ ������� �� ���� ������ ������������� � ���������� USART ��������� ������.
			#define CLR				2 //Clear To Send (�������� ��� ��������)- ������� ������� �� ����� ���������������� �������� ������ ����� ���������� ������� �������.
	////funx
		#define CS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA24; }
		#define CS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA24; }
//extern
	extern void gpio_init (void);
	extern DWORD pin_ctrl(BYTE device, BYTE  numb, BYTE mod);
	extern void check_gerkon(void);
	
#endif /* GPIO_H_ */