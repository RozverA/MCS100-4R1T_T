#ifndef GPIO_H_
#define GPIO_H_

void led_init();

#define LED				1
#define RTS				0
#define W55				2

#define PWR				0

#define PORT485_0		0
#define PORT485_1		1	
#define PORT485_2		2	
#define PORT485_3		3

#define ON				1
#define OFF				0
#define TGL				2

#define STAT			0
#define SET				1 //Request to send (запрос на отправку) - низкий уровень на этом выходе сигнализирует о готовности USART принимать данные.
#define CLR				2 //Clear To Send (очистить для отправки)- высокий уровень на входе приостанавливает передачу данных после завершения текущей посылки.

extern void gpio_init (void);
extern DWORD pin_ctrl(BYTE device, BYTE  numb, BYTE mod);
extern void check_gerkon(void);

#define CS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA24; }
#define CS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA24; }
	
#endif /* GPIO_H_ */