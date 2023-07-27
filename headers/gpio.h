#ifndef GPIO_H_
#define GPIO_H_

void led_init();

#define LED_RX	2
#define LED_TX	1
#define RTS		0

#define ON				1
#define OFF				0
#define TGL				2

#define STAT			0
#define SET				1
#define CLR				2

extern void gpio_init (void);
extern void gpio_ctrl(BYTE device, BYTE  numb, BYTE mod);
//LED

// #define LED_TX0_ON()	{ PORT_IOBUS->Group[1].OUTCLR.reg=PORT_PB03; }
// #define LED_TX0_OFF()	{ PORT_IOBUS->Group[1].OUTSET.reg=PORT_PB03; }
// #define LED_TX0_TGL()	{ PORT_IOBUS->Group[1].OUTTGL.reg=PORT_PB03; }
// 	
// #define LED_RX0_ON()	{ PORT_IOBUS->Group[1].OUTCLR.reg=PORT_PB02; }
// #define LED_RX0_OFF()	{ PORT_IOBUS->Group[1].OUTSET.reg=PORT_PB02; }
// #define LED_RX0_TGL()	{ PORT_IOBUS->Group[1].OUTTGL.reg=PORT_PB02; }
// 
// #define LED_TX1_ON()	{ PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA07; }
// #define LED_TX1_OFF()	{ PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA07; }
// #define LED_TX1_TGL()	{ PORT_IOBUS->Group[0].OUTTGL.reg=PORT_PA07; }
// 
// #define LED_RX1_ON()	{ PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA06; }
// #define LED_RX1_OFF()	{ PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA06; }
// #define LED_RX1_TGL()	{ PORT_IOBUS->Group[0].OUTTGL.reg=PORT_PA06; }
// 		
// #define LED_TX2_ON()	{ PORT_IOBUS->Group[1].OUTCLR.reg=PORT_PB22; }
// #define LED_TX2_OFF()	{ PORT_IOBUS->Group[1].OUTSET.reg=PORT_PB22; }
// #define LED_TX2_TGL()	{ PORT_IOBUS->Group[1].OUTTGL.reg=PORT_PB22; }		
// 		
// #define LED_RX2_ON()	{ PORT_IOBUS->Group[1].OUTCLR.reg=PORT_PB23; }
// #define LED_RX2_OFF()	{ PORT_IOBUS->Group[1].OUTSET.reg=PORT_PB23; }
// #define LED_RX2_TGL()	{ PORT_IOBUS->Group[1].OUTTGL.reg=PORT_PB23; }
// 		
// #define LED_TX3_ON()	{ PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA28; }
// #define LED_TX3_OFF()	{ PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA28; }
// #define LED_TX3_TGL()	{ PORT_IOBUS->Group[0].OUTTGL.reg=PORT_PA28; }
// 
// #define LED_RX3_ON()	{ PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA27; }
// #define LED_RX3_OFF()	{ PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA27; }
// #define LED_RX3_TGL()	{ PORT_IOBUS->Group[0].OUTTGL.reg=PORT_PA27; }		
// 
// #define UART_0_RTS()	 { return(PORT->Group[0].IN.reg & PORT_PA12); }
// #define UART_0_RTS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA12; }
// #define UART_0_RTS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA12; }
// 		
// #define UART_1_RTS()	 { return(PORT->Group[0].IN.reg & PORT_PA13); }
// #define UART_1_RTS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA13; }
// #define UART_1_RTS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA13; }	
// 		
// #define UART_2_RTS()	 { return(PORT->Group[0].IN.reg & PORT_PA14); }
// #define UART_2_RTS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA14; }
// #define UART_2_RTS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA14; }		
// 		
// #define UART_3_RTS()	 { return(PORT->Group[0].IN.reg & PORT_PA15); }
// #define UART_3_RTS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA15; }
// #define UART_3_RTS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA15; }	


#define CS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA24; }
#define CS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA24; }
	
#endif /* GPIO_H_ */