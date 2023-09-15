#include "def.h"

void gpio_init (void)
{
	//.............................................................................
	// LED ........................................................................
	//.............................................................................

	PORT->Group[1].OUTSET.reg=PORT_PB03;       // Bits 31:0 - OUTSET: Port Data Output Value Set
	PORT->Group[1].DIRSET.reg=PORT_PB03;       // Bits 31:0 - DIRSET: Port Data Direction Set
	PORT->Group[1].PINCFG[3].bit.PMUXEN=0;    // Bit  0    - PMUXEN: Peripheral Multiplexer Enable (0: The peripheral multiplexer selection is disabled)

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
	
	/*
	//.............................................................................
	// Gerkon ....................................................................
	//.............................................................................*/
	
	PORT->Group[1].DIRCLR.reg=PORT_PB23;	  	// Bits 31:0 - DIRCLR: Port Data Direction Clear
	PORT->Group[1].PINCFG[23].bit.INEN  =0x01;	// Bit  17   - INEN:   Input Enable
	PORT->Group[1].PINCFG[23].bit.PMUXEN=0x00;
}

void led_init()
{
	BYTE i = 0;
	for (i = 0; i<5; i++)
	{
		pin_ctrl(LED,PWR,ON);
		wait(2000);
		pin_ctrl(LED,PWR,OFF);
		wait(2000);
	}
	pin_ctrl(LED,PWR,ON);
	
	
	
	
}

DWORD pin_ctrl(BYTE device, BYTE  numb, BYTE mod)
{
	switch(device << 8 | numb << 4 | mod)
	{
		//RTS
		case (RTS << 8| PORT485_0 << 4| STAT):		return(PORT->Group[0].IN.reg & PORT_PA15);
		case (RTS << 8| PORT485_0 << 4| SET):		PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA15;return 0;
		case (RTS << 8| PORT485_0 << 4| CLR):		PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA15;return 0;
		case (RTS << 8| PORT485_1 << 4| STAT):		return(PORT->Group[0].IN.reg & PORT_PA14);
		case (RTS << 8| PORT485_1 << 4| SET):		PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA14;return 0;
		case (RTS << 8| PORT485_1 << 4| CLR):		PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA14;return 0;		
		case (RTS << 8| PORT485_2 << 4| STAT):		return(PORT->Group[0].IN.reg & PORT_PA13);
		case (RTS << 8| PORT485_2 << 4| SET):		PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA13;return 0;
		case (RTS << 8| PORT485_2 << 4| CLR):		PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA13;return 0;				
		case (RTS << 8| PORT485_3 << 4| STAT):		return(PORT->Group[0].IN.reg & PORT_PA12);
		case (RTS << 8| PORT485_3 << 4| SET):		PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA12;return 0;
		case (RTS << 8| PORT485_3 << 4| CLR):		PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA12;return 0;

		//LED TX
		case (LED << 8| PWR << 4| ON):		PORT_IOBUS->Group[1].OUTCLR.reg=PORT_PB03;return 0; 
		case (LED << 8| PWR << 4| OFF):		PORT_IOBUS->Group[1].OUTSET.reg=PORT_PB03;return 0;
		case (LED << 8| PWR << 4| TGL):		PORT_IOBUS->Group[1].OUTTGL.reg=PORT_PB03;return 0;
		case (W55 << 8| PWR << 4| CLR):     PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA18;return 0;
		case (W55 << 8| PWR << 4| SET):     PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA18;return 0;
		
	}
}

void check_gerkon(void)
{
	static BYTE gerkon_st = 0;
	
	if (PORT->Group[1].IN.reg & PORT_PB23)	{if(PORT->Group[1].OUTSET.reg & PORT_PB03){pin_ctrl(LED,PWR,ON);} gerkon_st = 0; return;}
	
	switch (gerkon_st)
	{
		case 0:
			pin_ctrl(LED,PWR,OFF);
			ger_wait = 10;
			gerkon_st = 1;
		break;
		case 1:
			if (ger_wait)	{return;}
			reset = 1; 
			cfg_default(); 
			cfg_save();
		break;
	}
}