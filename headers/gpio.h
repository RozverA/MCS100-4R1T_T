#ifndef GPIO_H_
#define GPIO_H_


	extern void gpio_init (void);
	
	//LED

	#define LED_TX0_ON()	{ PORT_IOBUS->Group[1].OUTCLR.reg=PORT_PB03; }
	#define LED_TX0_OFF()	{ PORT_IOBUS->Group[1].OUTSET.reg=PORT_PB03; }
	#define LED_TX0_TGL()	{ PORT_IOBUS->Group[1].OUTTGL.reg=PORT_PB03; }
	
	#define LED_RX0_ON()	{ PORT_IOBUS->Group[1].OUTCLR.reg=PORT_PB02; }
	#define LED_RX0_OFF()	{ PORT_IOBUS->Group[1].OUTSET.reg=PORT_PB02; }
	#define LED_RX0_TGL()	{ PORT_IOBUS->Group[1].OUTTGL.reg=PORT_PB02; }

	#define LED_TX1_ON()	{ PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA07; }
	#define LED_TX1_OFF()	{ PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA07; }
	#define LED_TX1_TGL()	{ PORT_IOBUS->Group[0].OUTTGL.reg=PORT_PA07; }

	#define LED_RX1_ON()	{ PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA06; }
	#define LED_RX1_OFF()	{ PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA06; }
	#define LED_RX1_TGL()	{ PORT_IOBUS->Group[0].OUTTGL.reg=PORT_PA06; }
		
	#define LED_TX2_ON()	{ PORT_IOBUS->Group[1].OUTCLR.reg=PORT_PB22; }
	#define LED_TX2_OFF()	{ PORT_IOBUS->Group[1].OUTSET.reg=PORT_PB22; }
	#define LED_TX2_TGL()	{ PORT_IOBUS->Group[1].OUTTGL.reg=PORT_PB22; }		
		
	#define LED_RX2_ON()	{ PORT_IOBUS->Group[1].OUTCLR.reg=PORT_PB23; }
	#define LED_RX2_OFF()	{ PORT_IOBUS->Group[1].OUTSET.reg=PORT_PB23; }
	#define LED_RX2_TGL()	{ PORT_IOBUS->Group[1].OUTTGL.reg=PORT_PB23; }
		
	#define LED_TX3_ON()	{ PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA28; }
	#define LED_TX3_OFF()	{ PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA28; }
	#define LED_TX3_TGL()	{ PORT_IOBUS->Group[0].OUTTGL.reg=PORT_PA28; }

	#define LED_RX3_ON()	{ PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA27; }
	#define LED_RX3_OFF()	{ PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA27; }
	#define LED_RX3_TGL()	{ PORT_IOBUS->Group[0].OUTTGL.reg=PORT_PA27; }		

	#define UART_0_RTS()	 { return(PORT->Group[0].IN.reg & PORT_PA12); }
	#define UART_0_RTS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA12; }
	#define UART_0_RTS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA12; }
		
	#define UART_1_RTS()	 { return(PORT->Group[0].IN.reg & PORT_PA13); }
	#define UART_1_RTS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA13; }
	#define UART_1_RTS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA13; }	
		
	#define UART_2_RTS()	 { return(PORT->Group[0].IN.reg & PORT_PA14); }
	#define UART_2_RTS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA14; }
	#define UART_2_RTS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA14; }		
		
	#define UART_3_RTS()	 { return(PORT->Group[0].IN.reg & PORT_PA15); }
	#define UART_3_RTS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA15; }
	#define UART_3_RTS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA15; }	


	#define CS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA24; }
	#define CS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA24; }
				
#endif /* GPIO_H_ */