#include "def.h"


void gpio_init (void)
{
	//.............................................................................
	// LED ........................................................................
	//.............................................................................
	
	PORT->Group[0].OUTSET.reg=PORT_PA06;       // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA06;       // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[6].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)
		
	PORT->Group[0].OUTSET.reg=PORT_PA07;       // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA07;       // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[7].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)*/
		
	PORT->Group[0].OUTSET.reg=PORT_PA27;       // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA27;       // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[27].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)
		
	PORT->Group[0].OUTSET.reg=PORT_PA28;       // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA28;       // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[28].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)*/
		
	PORT->Group[1].OUTSET.reg=PORT_PB02;       // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[1].DIRSET.reg=PORT_PB02;       // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[1].PINCFG[2].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)

	PORT->Group[1].OUTSET.reg=PORT_PB03;       // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[1].DIRSET.reg=PORT_PB03;       // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[1].PINCFG[3].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)
		
	PORT->Group[1].OUTSET.reg=PORT_PB22;       // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[1].DIRSET.reg=PORT_PB22;       // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[1].PINCFG[22].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)

	PORT->Group[1].OUTSET.reg=PORT_PB23;       // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[1].DIRSET.reg=PORT_PB23;       // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[1].PINCFG[23].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)
		
	//.............................................................................
	// USART1 SERCOM1_ALT ............................................................
	//.............................................................................
 
	// PA12 - USART_RTS (Output)
	PORT->Group[0].OUTSET.reg=PORT_PA12;      // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA12;      // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[12].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)

	// PA00 - USART_TX (Output)
	PORT->Group[0].OUTCLR.reg=PORT_PA00;      // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA00;      // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[0].bit.PMUXEN=1;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (1: The peripheral multiplexer selection is enabled)
	PORT->Group[0].PMUX  [0].bit.PMUXE=0x03;  // Bits 3:0  - PMUXE: Peripheral Multiplexing Even (2: Peripheral function C selected)

	// PA01 - USART_RX (Input)
	PORT->Group[0].DIRCLR.reg=PORT_PA01;	  	// Bits 31:0 - DIRCLR: Port Data Direction Clear
	PORT->Group[0].PINCFG[1].bit.INEN  =0x01;	// Bit  17   - INEN:   Input Enable
	PORT->Group[0].PINCFG[1].bit.PMUXEN=0x01;	// Bit  0    - PMUXEN: Peripheral Multiplexer Enable (1: The peripheral multiplexer selection is enabled)
	PORT->Group[0].PMUX  [0].bit.PMUXO =3;  	// Bits 3:0  - PMUXE: Peripheral Multiplexing Odd (2: Peripheral function C selected)	


	//.............................................................................
	// USART2 SERCOM0_ALT ............................................................
	//.............................................................................
	
	// PA13 - USART_RTS (Output)
	PORT->Group[0].OUTSET.reg=PORT_PA13;      // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA13;      // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[13].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)

	// PA04 - USART_TX (Output)
	PORT->Group[0].OUTCLR.reg=PORT_PA04;      // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA04;      // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[4].bit.PMUXEN=1;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (1: The peripheral multiplexer selection is enabled)
	PORT->Group[0].PMUX  [2].bit.PMUXE=0x03;  // Bits 3:0  - PMUXE: Peripheral Multiplexing Even (2: Peripheral function C selected)

	// PA05 - USART_RX (Input)
	PORT->Group[0].DIRCLR.reg=PORT_PA05;	  	// Bits 31:0 - DIRCLR: Port Data Direction Clear
	PORT->Group[0].PINCFG[5].bit.INEN  =0x01;	// Bit  17   - INEN:   Input Enable
	PORT->Group[0].PINCFG[5].bit.PMUXEN=0x01;	// Bit  0    - PMUXEN: Peripheral Multiplexer Enable (1: The peripheral multiplexer selection is enabled)
	PORT->Group[0].PMUX  [2].bit.PMUXO =3;  	// Bits 3:0  - PMUXE: Peripheral Multiplexing Odd (2: Peripheral function C selected)	
	
	
	//.............................................................................
	// USART3 SERCOM2_ALT ............................................................
	//.............................................................................
		
	// PA14 - USART_RTS (Output)
	PORT->Group[0].OUTSET.reg=PORT_PA14;      // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA14;      // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[14].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)

	// PA08 - USART_TX (Output)
	PORT->Group[0].OUTCLR.reg=PORT_PA08;      // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA08;      // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[8].bit.PMUXEN=1;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (1: The peripheral multiplexer selection is enabled)
	PORT->Group[0].PMUX  [4].bit.PMUXE=0x03;  // Bits 3:0  - PMUXE: Peripheral Multiplexing Even (2: Peripheral function C selected)

	// PA09 - USART_RX (Input)
	PORT->Group[0].DIRCLR.reg=PORT_PA09;	  	// Bits 31:0 - DIRCLR: Port Data Direction Clear
	PORT->Group[0].PINCFG[9].bit.INEN  =0x01;	// Bit  17   - INEN:   Input Enable
	PORT->Group[0].PINCFG[9].bit.PMUXEN=0x01;	// Bit  0    - PMUXEN: Peripheral Multiplexer Enable (1: The peripheral multiplexer selection is enabled)
	PORT->Group[0].PMUX  [4].bit.PMUXO =3;  	// Bits 3:0  - PMUXE: Peripheral Multiplexing Odd (2: Peripheral function C selected)	
	
	
	//.............................................................................
	// USART4 SERCOM3_ALT ............................................................
	//.............................................................................
		
	// PA15 - USART_RTS (Output)
	PORT->Group[0].OUTSET.reg=PORT_PA15;      // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA15;      // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[15].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)

	// PA16 - USART_TX (Output)
	PORT->Group[0].OUTCLR.reg=PORT_PA16;      // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA16;      // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[16].bit.PMUXEN=1;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (1: The peripheral multiplexer selection is enabled)
	PORT->Group[0].PMUX  [8].bit.PMUXE=0x03;  // Bits 3:0  - PMUXE: Peripheral Multiplexing Even (2: Peripheral function C selected)

	// PA17 - USART_RX (Input)
	PORT->Group[0].DIRCLR.reg=PORT_PA17;	  	// Bits 31:0 - DIRCLR: Port Data Direction Clear
	PORT->Group[0].PINCFG[17].bit.INEN  =0x01;	// Bit  17   - INEN:   Input Enable
	PORT->Group[0].PINCFG[17].bit.PMUXEN=0x01;	// Bit  0    - PMUXEN: Peripheral Multiplexer Enable (1: The peripheral multiplexer selection is enabled)
	PORT->Group[0].PMUX  [8].bit.PMUXO =3;  	// Bits 3:0  - PMUXE: Peripheral Multiplexing Odd (2: Peripheral function C selected)	
	
	
	
	/*
	//.............................................................................
	// SPI SERCOM5_ALT ....................................................................
	//.............................................................................*/

	// PA23 - SPI_MOSI (Output)
	PORT->Group[0].OUTCLR.reg=PORT_PA23;       // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA23;       // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[23].bit.PMUXEN=1;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (1: The peripheral multiplexer selection is enabled)
	PORT->Group[0].PMUX  [11].bit.PMUXO=3;  // Bits 3:0  - PMUXE: Peripheral Multiplexing Even (5: Peripheral function F selected)

	// PA22 - SPI_MISO (Input)
	PORT->Group[0].DIRCLR.reg=PORT_PA22;       // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[22].bit.INEN  =0x01; // Bit  17   - INEN:   Input Enable
	PORT->Group[0].PINCFG[22].bit.PMUXEN=1;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (1: The peripheral multiplexer selection is enabled)
	PORT->Group[0].PMUX  [11].bit.PMUXE=3;  // Bits 3:0  - PMUXE: Peripheral Multiplexing Even (5: Peripheral function F selected)

	// PA24 - SPI_SCK (Output)
	PORT->Group[0].OUTCLR.reg=PORT_PA25;       // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA25;       // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[25].bit.PMUXEN=0x01; // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (1: The peripheral multiplexer selection is enabled)
	PORT->Group[0].PMUX  [12].bit.PMUXO=3;  // Bits 3:0  - PMUXO: Peripheral Multiplexing Odd (5: Peripheral function F selected)

	// PA25 - SPI_CS (Output)
	PORT->Group[0].OUTSET.reg=PORT_PA24;       // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[0].DIRSET.reg=PORT_PA24;       // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[0].PINCFG[24].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)
	
	// PA19 - SPI_INT (Input)
	PORT->Group[0].DIRCLR.reg=PORT_PA19;       // Bits 31:0 - DIRCLR: Port Data Direction Clear
	PORT->Group[0].PINCFG[19].bit.PMUXEN=0x00; // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (1: The peripheral multiplexer selection is enabled)

    // PA18 - SPI_RESET (Output)
    PORT->Group[0].OUTSET.reg=PORT_PA18;       // Bits 31:0 - OUTSET: Port Data Output Value Set
    PORT->Group[0].DIRSET.reg=PORT_PA18;       // Bits 31:0 - DIRSET: Port Data Direction Set
    PORT->Group[0].PINCFG[18].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)*/  
	PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA18;  
}

void led_init()
{
	pin_ctrl(LED_TX, 0, ON);
	pin_ctrl(LED_RX, 0, ON);
	pin_ctrl(LED_TX, 1, ON);
	pin_ctrl(LED_RX, 1, ON);
	pin_ctrl(LED_TX, 2, ON);
	pin_ctrl(LED_RX, 2, ON);
	pin_ctrl(LED_TX, 3, ON);
	pin_ctrl(LED_RX, 3, ON);
	
	wait(time_500ms);
	
	pin_ctrl(LED_TX, 0, OFF);
	pin_ctrl(LED_RX, 0, OFF);
	pin_ctrl(LED_TX, 1, OFF);
	pin_ctrl(LED_RX, 1, OFF);
	pin_ctrl(LED_TX, 2, OFF);
	pin_ctrl(LED_RX, 2, OFF);
	pin_ctrl(LED_TX, 3, OFF);
	pin_ctrl(LED_RX, 3, OFF);
	
	
}

DWORD pin_ctrl(BYTE device, BYTE  numb, BYTE mod)
{
	switch(device << 8 | numb << 4 | mod)
	{
		//RTS
		case (RTS << 8| PORT485_0 << 4| STAT):		return(PORT->Group[0].IN.reg & PORT_PA12);
		case (RTS << 8| PORT485_0 << 4| SET):		PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA12;return;
		case (RTS << 8| PORT485_0 << 4| CLR):		PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA12;return;
		case (RTS << 8| PORT485_1 << 4| STAT):		return(PORT->Group[0].IN.reg & PORT_PA13);
		case (RTS << 8| PORT485_1 << 4| SET):		PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA13;return;
		case (RTS << 8| PORT485_1 << 4| CLR):		PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA13;return;
		case (RTS << 8| PORT485_2 << 4| STAT):		return(PORT->Group[0].IN.reg & PORT_PA14);
		case (RTS << 8| PORT485_2 << 4| SET):		PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA14;return;
		case (RTS << 8| PORT485_2 << 4| CLR):		PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA14;return;
		case (RTS << 8| PORT485_3 << 4| STAT):		return(PORT->Group[0].IN.reg & PORT_PA15);
		case (RTS << 8| PORT485_3 << 4| SET):		PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA15;return;
		case (RTS << 8| PORT485_3 << 4| CLR):		PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA15;return;
		//LED TX
		case (LED_TX << 8| PORT485_0 << 4| ON):		PORT_IOBUS->Group[1].OUTCLR.reg=PORT_PB03;return; 
		case (LED_TX << 8| PORT485_0 << 4| OFF):	PORT_IOBUS->Group[1].OUTSET.reg=PORT_PB03;return;
		case (LED_TX << 8| PORT485_0 << 4| TGL):	PORT_IOBUS->Group[1].OUTTGL.reg=PORT_PB03;return;
		case (LED_TX << 8| PORT485_1 << 4| ON):		PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA07;return;
		case (LED_TX << 8| PORT485_1 << 4| OFF):	PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA07;return;
		case (LED_TX << 8| PORT485_1 << 4| TGL):	PORT_IOBUS->Group[0].OUTTGL.reg=PORT_PA07;return;
		case (LED_TX << 8| PORT485_2 << 4| ON):		PORT_IOBUS->Group[1].OUTCLR.reg=PORT_PB22;return;
		case (LED_TX << 8| PORT485_2 << 4| OFF):	PORT_IOBUS->Group[1].OUTSET.reg=PORT_PB22;return;
		case (LED_TX << 8| PORT485_2 << 4| TGL):	PORT_IOBUS->Group[1].OUTTGL.reg=PORT_PB22;return;
		case (LED_TX << 8| PORT485_3 << 4| ON):		PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA28;return;
		case (LED_TX << 8| PORT485_3 << 4| OFF):	PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA28;return;
		case (LED_TX << 8| PORT485_3 << 4| TGL):	PORT_IOBUS->Group[0].OUTTGL.reg=PORT_PA28;return;
		//LED RX
		case (LED_RX << 8| PORT485_0 << 4| ON):		PORT_IOBUS->Group[1].OUTCLR.reg=PORT_PB02;return;
		case (LED_RX << 8| PORT485_0 << 4| OFF):	PORT_IOBUS->Group[1].OUTSET.reg=PORT_PB02;return;
		case (LED_RX << 8| PORT485_0 << 4| TGL):	PORT_IOBUS->Group[1].OUTTGL.reg=PORT_PB02;return;
		case (LED_RX << 8| PORT485_1 << 4| ON):		PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA06;return;
		case (LED_RX << 8| PORT485_1 << 4| OFF):	PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA06;return;
		case (LED_RX << 8| PORT485_1 << 4| TGL):	PORT_IOBUS->Group[0].OUTTGL.reg=PORT_PA06;return;
		case (LED_RX << 8| PORT485_2 << 4| ON):		PORT_IOBUS->Group[1].OUTCLR.reg=PORT_PB23;return;
		case (LED_RX << 8| PORT485_2 << 4| OFF):	PORT_IOBUS->Group[1].OUTSET.reg=PORT_PB23;return;
		case (LED_RX << 8| PORT485_2 << 4| TGL):	PORT_IOBUS->Group[1].OUTTGL.reg=PORT_PB23;return;
		case (LED_RX << 8| PORT485_3 << 4| ON):		PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA27;return;
		case (LED_RX << 8| PORT485_3 << 4| OFF):	PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA27;return;
		case (LED_RX << 8| PORT485_3 << 4| TGL):	PORT_IOBUS->Group[0].OUTTGL.reg=PORT_PA27;return;
	}
}