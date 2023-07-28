#ifndef GPIO_H_
#define GPIO_H_

void led_init();

#define LED_RX	2
#define LED_TX	1
#define RTS		0

#define PORT485_0		0
#define PORT485_1		1	
#define PORT485_2		2	
#define PORT485_3		3

#define ON				1
#define OFF				0
#define TGL				2

#define STAT			0
#define SET				1
#define CLR				2

extern void gpio_init (void);
extern DWORD pin_ctrl(BYTE device, BYTE  numb, BYTE mod);

#define CS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA24; }
#define CS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA24; }
	
#endif /* GPIO_H_ */