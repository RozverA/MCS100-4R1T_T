#include "def.h"

volatile USART port[4];


void usart_init(void)
{
usart_0_init();
usart_1_init();
usart_2_init();
usart_3_init();
}

//port 0
void usart_0_init(void)
{
	double tout =0;
	double bsize=0;
	bsize = 9;


	PM->APBCMASK.bit.SERCOM1_ = 1; // Bit  2    - SERCOM0: SERCOM0 APB Clock Enable (1 - enabled)
	PM->APBCSEL.bit.APBCDIV   = 0x00;

	GCLK->CLKCTRL.bit.WRTLOCK = 0x00;
	GCLK->CLKCTRL.bit.ID      = GCLK_CLKCTRL_ID_SERCOM1_CORE; // Bits 5:0  - ID: Generic Clock Selection ID (0x14 - SERCOM0_CORE)
	GCLK->CLKCTRL.bit.GEN     = 0x00; // Bits 11:8 - GEN: Generic Clock Generator (0: Generic clock generator 0)
	GCLK->CLKCTRL.bit.CLKEN   = 0x01; // Bit  14   - CLKEN: Clock Enable (1: The generic clock is enabled)

	//.............................................................................

	SERCOM1->USART.CTRLA.bit.ENABLE = 0x00; // Bit 1 - ENABLE: Enable (0: The peripheral is disabled or being disabled)
	while(SERCOM1->USART.SYNCBUSY.bit.ENABLE) { ; }

	//.............................................................................

	SERCOM1->USART.CTRLA.bit.SWRST   =0x01; //Bit0-SWRST: Software Reset (Writing a one to this bit resets all registers in the SERCOM, except DBGCTRL, to their initial state, and the SERCOM will be disabled.)
	while(SERCOM1->USART.SYNCBUSY.bit.SWRST) { ; } // Bit 0 - SWRST: Software Reset Synchronization Busy


	SERCOM1->USART.CTRLA.bit.DORD    =0x01;  // Bit  30    - DORD: Data Order (1: LSB is transmitted first.)
	SERCOM1->USART.CTRLA.bit.CPOL    =0x00;  // Bit  29    - CPOL: Clock Polarity (0: SCK is low when idle. The leading edge of a clock cycle is a rising edge, while the trailing edge is a falling edge)
	SERCOM1->USART.CTRLA.bit.CMODE   =0x00;  // Bit  28    - CMODE: Communication Mode (0: Asynchronous communication.)
	SERCOM1->USART.CTRLA.bit.FORM    =0x00;  // Bits 27:24 - FORM: Frame Format (0: USART frame NO PARITY)
	SERCOM1->USART.CTRLA.bit.SAMPA   =0x00;  // Bits 23:22 - SAMPA[1:0]: Sample Adjustment (0: 3-4-5)
	SERCOM1->USART.CTRLA.bit.SAMPR   =0x02;  // Bits 15:13 - SAMPR[2:0]: Sample Rate (2: 8x over-sampling using arithmetic baud rate generation.)
	SERCOM1->USART.CTRLA.bit.IBON    =0x00;  // Bit  8     - IBON: Immediate Buffer Overflow Notification (0: STATUS.BUFOVF is asserted when it occurs in the data stream)
	SERCOM1->USART.CTRLA.bit.RUNSTDBY=0x00;  // Bit  7     - RUNSTDBY: Run In Standby (Generic clock is disabled when ongoing transaction is finished. All interrupts can wake up the device.)
	SERCOM1->USART.CTRLA.bit.MODE    =0x01;  // Bits 4:2   - MODE: Operating Mode (1: USART with internal clock.)
	SERCOM1->USART.CTRLA.bit.TXPO    =0x02;  // Bits 17:16 - TXPO: Transmit Data Pinout (2: TX->PAD[0], RTS->PAD[2])
	SERCOM1->USART.CTRLA.bit.RXPO    =0x01;  // Bits 21:20 - RXPO: Receive Data Pinout (1: RX->PAD[1])
	SERCOM1->USART.CTRLB.bit.PMODE   =0x00;  // Bit  13    - PMODE: Parity Mode (0: Even parity)
	SERCOM1->USART.CTRLB.bit.ENC     =0x00;  // Bit  10    - ENC: Encoding Format (0: Data is not encoded.)
	SERCOM1->USART.CTRLB.bit.SFDE    =0x01;  // Bit  9     - SFDE: Start of Frame Detection Enable (1: Start-of-frame detection enabled.)
	SERCOM1->USART.CTRLB.bit.COLDEN  =0x00;  // Bit  8     - COLDEN: Collision Detection Enable (0: Collision detection is not enabled.)
	SERCOM1->USART.CTRLB.bit.SBMODE  =0x00;  // Bit  6     - SBMODE: Stop Bit Mode (0: One stop bit.)
	SERCOM1->USART.CTRLB.bit.CHSIZE  =0x00;  // Bits 2:0   - CHSIZE: Character Size (0: 8 bits)

	//SERCOM0->USART.BAUD.bit.BAUD=65536.0f*(1.0f-(8.0*(float)(9600))/(float)(8000000));
	SERCOM1->USART.BAUD.bit.BAUD=65536.0f*(1.0f-(8.0*(float)(38400))/(float)(PROC_HERZ));
	//SERCOM0->USART.BAUD.bit.BAUD=65536.0f*(1.0f-(8.0*(float)(115200))/(float)(8000000));

	SERCOM1->USART.INTENSET.bit.RXC  =0x01; // Bit 2 RXC: Receive Complete Interrupt Enable

	while(SERCOM1->USART.SYNCBUSY.reg & 0x07) { ; }
	SERCOM1->USART.CTRLB.bit.RXEN=0x01;     // Bit 17 - RXEN: Receiver Enable (11: The receiver is enabled or will be enabled when the USART is enabled)

	while(SERCOM1->USART.SYNCBUSY.reg & 0x07) { ; }
	SERCOM1->USART.CTRLB.bit.TXEN=0x01;     // Bit 16 - TXEN: Transmitter Enable (1: The transmitter is enabled or will be enabled when the USART is enabled.)

	while(SERCOM1->USART.SYNCBUSY.reg & 0x07) { ; }
	SERCOM1->USART.CTRLA.bit.ENABLE=0x01;   // Bit 1 - ENABLE: Enable (1: The peripheral is enabled or being enabled.)

	tout = (double)1000000/(double)38400;
	tout = tout*bsize;
	tout = tout*4;
	tout = tout/100;
	port[0].rtout = tout;

	NVIC_DisableIRQ(SERCOM1_IRQn);
	NVIC_SetPriority(SERCOM1_IRQn, 1);
	NVIC_EnableIRQ(SERCOM1_IRQn);


	UART_0_RTS_clr();
}
WORD usart_0_write(BYTE* wbuf,WORD wn)
{
	if(wn == 0) { return(0); }
	UART_0_RTS_set();
	LED_TX0_ON();
	
	if(wn > USART_BUF_SIZE) { wn = USART_BUF_SIZE; }
	memcpy(port[0].wbuf,wbuf,wn);
	port[0].wn=wn;
	port[0].wx=1;
	SERCOM1->USART.DATA.reg=port[0].wbuf[0];
	SERCOM1->USART.INTENCLR.bit.RXC=1;
	SERCOM1->USART.INTENSET.bit.DRE=1;
	return(wn);
}
WORD usart_0_read (BYTE* rbuf,WORD rn)
{
	WORD size = 0;
	if(port[0].rn     ==       0x00) { port[0].rtime=0;return(0); }
	if(port[0].rtout  >  port[0].rtime) { return(0); }
	//!!!!!!!!!!!

	size = port[0].rn;

	if(rn < size) { size = rn;  }

	memcpy(rbuf,port[0].rbuf,size);
	port[0].rn=0;
	port[0].rx=0;
	LED_RX0_OFF();
	
	return(size);


}
void SERCOM1_Handler(void)
{
	
	if(SERCOM1->USART.INTFLAG.bit.RXC)
	{

		port[0].rxc++;
		if(port[0].rn >= USART_BUF_SIZE){port[0].rn=0;}
		port[0].rbuf[port[0].rn] = SERCOM1->USART.DATA.reg;
		port[0].rn   ++;
		port[0].rtime=0;
		LED_RX0_ON();
		return;
	}

	if(SERCOM1->USART.INTFLAG.bit.TXC)
	{
		port[0].txc++;
		SERCOM1->USART.INTFLAG.bit.TXC=1;
		LED_TX0_OFF();
		UART_0_RTS_clr();
		SERCOM1->USART.INTENCLR.bit.TXC=1;
		SERCOM1->USART.INTENSET.bit.RXC=1;
		return;
	}
	
	if(SERCOM1->USART.INTFLAG.bit.DRE)
	{
		port[0].dre++;
		if(port[0].wx < port[0].wn)
		{
			SERCOM1->USART.DATA.reg = port[0].wbuf[port[0].wx];
			port[0].wx  ++;
			return;
		}
		SERCOM1->USART.INTENCLR.bit.DRE=1;
		SERCOM1->USART.INTENSET.bit.TXC=1;
		return;
	}
	

}

//port 1
void usart_1_init(void)
{
	double tout =0;
	double bsize=0;
	bsize = 9;
	
	PM->APBCMASK.bit.SERCOM0_ =1; // Bit  2    - SERCOM0: SERCOM0 APB Clock Enable (1 - enabled)
	PM->APBCSEL.bit.APBCDIV = 0x00;

	GCLK->CLKCTRL.bit.WRTLOCK=0x00;
	GCLK->CLKCTRL.bit.ID   =GCLK_CLKCTRL_ID_SERCOM0_CORE; // Bits 5:0  - ID: Generic Clock Selection ID (0x14 - SERCOM0_CORE)
	GCLK->CLKCTRL.bit.GEN  =0x00; // Bits 11:8 - GEN: Generic Clock Generator (0: Generic clock generator 0)
	GCLK->CLKCTRL.bit.CLKEN=0x01; // Bit  14   - CLKEN: Clock Enable (1: The generic clock is enabled)

	//.............................................................................

	SERCOM0->USART.CTRLA.bit.ENABLE=0x00; // Bit 1 - ENABLE: Enable (0: The peripheral is disabled or being disabled)
	while(SERCOM0->USART.SYNCBUSY.bit.ENABLE) { ; }

	//.............................................................................

	SERCOM0->USART.CTRLA.bit.SWRST   =0x01; //Bit0-SWRST: Software Reset (Writing a one to this bit resets all registers in the SERCOM, except DBGCTRL, to their initial state, and the SERCOM will be disabled.)
	while(SERCOM0->USART.SYNCBUSY.bit.SWRST) { ; } // Bit 0 - SWRST: Software Reset Synchronization Busy

	SERCOM0->USART.CTRLA.bit.DORD    =0x01;  // Bit  30    - DORD: Data Order (1: LSB is transmitted first.)
	SERCOM0->USART.CTRLA.bit.CPOL    =0x00;  // Bit  29    - CPOL: Clock Polarity (0: SCK is low when idle. The leading edge of a clock cycle is a rising edge, while the trailing edge is a falling edge)
	SERCOM0->USART.CTRLA.bit.CMODE   =0x00;  // Bit  28    - CMODE: Communication Mode (0: Asynchronous communication.)
	SERCOM0->USART.CTRLA.bit.FORM    =0x00;  // Bits 27:24 - FORM: Frame Format (0: USART frame NO PARITY)
	SERCOM0->USART.CTRLA.bit.SAMPA   =0x00;  // Bits 23:22 - SAMPA[1:0]: Sample Adjustment (0: 3-4-5)
	SERCOM0->USART.CTRLA.bit.SAMPR   =0x02;  // Bits 15:13 - SAMPR[2:0]: Sample Rate (2: 8x over-sampling using arithmetic baud rate generation.)
	SERCOM0->USART.CTRLA.bit.IBON    =0x00;  // Bit  8     - IBON: Immediate Buffer Overflow Notification (0: STATUS.BUFOVF is asserted when it occurs in the data stream)
	SERCOM0->USART.CTRLA.bit.RUNSTDBY=0x00;  // Bit  7     - RUNSTDBY: Run In Standby (Generic clock is disabled when ongoing transaction is finished. All interrupts can wake up the device.)
	SERCOM0->USART.CTRLA.bit.MODE    =0x01;  // Bits 4:2   - MODE: Operating Mode (1: USART with internal clock.)
	SERCOM0->USART.CTRLA.bit.TXPO    =0x02;  // Bits 17:16 - TXPO: Transmit Data Pinout (2: TX->PAD[0], RTS->PAD[2])
	SERCOM0->USART.CTRLA.bit.RXPO    =0x01;  // Bits 21:20 - RXPO: Receive Data Pinout (1: RX->PAD[1])
	SERCOM0->USART.CTRLB.bit.PMODE   =0x00;  // Bit  13    - PMODE: Parity Mode (0: Even parity)
	SERCOM0->USART.CTRLB.bit.ENC     =0x00;  // Bit  10    - ENC: Encoding Format (0: Data is not encoded.)
	SERCOM0->USART.CTRLB.bit.SFDE    =0x01;  // Bit  9     - SFDE: Start of Frame Detection Enable (1: Start-of-frame detection enabled.)
	SERCOM0->USART.CTRLB.bit.COLDEN  =0x00;  // Bit  8     - COLDEN: Collision Detection Enable (0: Collision detection is not enabled.)
	SERCOM0->USART.CTRLB.bit.SBMODE  =0x00;  // Bit  6     - SBMODE: Stop Bit Mode (0: One stop bit.)
	SERCOM0->USART.CTRLB.bit.CHSIZE  =0x00;  // Bits 2:0   - CHSIZE: Character Size (0: 8 bits)

	//SERCOM0->USART.BAUD.bit.BAUD=65536.0f*(1.0f-(8.0*(float)(9600))/(float)(8000000));
	SERCOM0->USART.BAUD.bit.BAUD=65536.0f*(1.0f-(8.0*(float)(38400))/(float)(PROC_HERZ));
	//SERCOM0->USART.BAUD.bit.BAUD=65536.0f*(1.0f-(8.0*(float)(115200))/(float)(8000000));

	SERCOM0->USART.INTENSET.bit.RXC  =0x01; // Bit 2 RXC: Receive Complete Interrupt Enable

	while(SERCOM0->USART.SYNCBUSY.reg & 0x07) { ; }
	SERCOM0->USART.CTRLB.bit.RXEN=0x01;     // Bit 17 - RXEN: Receiver Enable (11: The receiver is enabled or will be enabled when the USART is enabled)

	while(SERCOM0->USART.SYNCBUSY.reg & 0x07) { ; }
	SERCOM0->USART.CTRLB.bit.TXEN=0x01;     // Bit 16 - TXEN: Transmitter Enable (1: The transmitter is enabled or will be enabled when the USART is enabled.)

	while(SERCOM0->USART.SYNCBUSY.reg & 0x07) { ; }
	SERCOM0->USART.CTRLA.bit.ENABLE=0x01;   // Bit 1 - ENABLE: Enable (1: The peripheral is enabled or being enabled.)

	tout = (double)1000000/(double)38400;
	tout = tout*bsize;
	tout = tout*4;
	tout = tout/100;
	port[1].rtout = tout;

	NVIC_DisableIRQ(SERCOM0_IRQn);
	NVIC_SetPriority(SERCOM0_IRQn, 1);
	NVIC_EnableIRQ(SERCOM0_IRQn);


	UART_1_RTS_clr();

}
WORD usart_1_write(BYTE* wbuf,WORD wn)
{
	if(wn == 0) { return(0); }
	UART_1_RTS_set();
	LED_TX1_ON();
	
	if(wn > USART_BUF_SIZE) { wn = USART_BUF_SIZE; }
	memcpy(port[1].wbuf,wbuf,wn);
	port[1].wn=wn;
	port[1].wx=1;
	SERCOM0->USART.DATA.reg=port[1].wbuf[0];
	SERCOM0->USART.INTENCLR.bit.RXC=1;
	SERCOM0->USART.INTENSET.bit.DRE=1;
	return(wn);
}
WORD usart_1_read (BYTE* rbuf,WORD rn)
{
	WORD size = 0;
	if(port[1].rn     ==       0x00)	{ port[1].rtime=0;return(0); }
	if(port[1].rtout  >  port[1].rtime) { return(0); }
	//!!!!!!!!!!!

	size = port[1].rn;

	if(rn < size) { size = rn;  }


	memcpy(rbuf,port[1].rbuf,size);
	port[1].rn=0;
	port[1].rx=0;
	LED_RX1_OFF();
	
	return(size);


}
void SERCOM0_Handler(void)
{
	
	if(SERCOM0->USART.INTFLAG.bit.RXC)
	{
		port[1].rxc++;
		if(port[1].rn >= USART_BUF_SIZE){port[1].rn=0;}
		port[1].rbuf[port[1].rn] = SERCOM0->USART.DATA.reg;
		port[1].rn   ++;
		port[1].rtime=0;
		LED_RX1_ON();
		return;
	}

	if(SERCOM0->USART.INTFLAG.bit.TXC)
	{
		port[1].txc++;
		SERCOM0->USART.INTFLAG.bit.TXC=1;
		LED_TX1_OFF();
		UART_1_RTS_clr();
		SERCOM0->USART.INTENCLR.bit.TXC=1;
		SERCOM0->USART.INTENSET.bit.RXC=1;
		return;
	}
	
	if(SERCOM0->USART.INTFLAG.bit.DRE)
	{
		port[1].dre++;
		if(port[1].wx < port[1].wn)
		{
			SERCOM0->USART.DATA.reg = port[1].wbuf[port[1].wx];
			port[1].wx  ++;
			return;
		}
		SERCOM0->USART.INTENCLR.bit.DRE=1;
		SERCOM0->USART.INTENSET.bit.TXC=1;
		return;
	}
	

}

//port 2
void usart_2_init(void)
{	
	double tout =0;
	double bsize=0;
	bsize = 9;
	PM->APBCMASK.bit.SERCOM2_ =1; // Bit  2    - SERCOM0: SERCOM0 APB Clock Enable (1 - enabled)
	PM->APBCSEL.bit.APBCDIV = 0x00;

	GCLK->CLKCTRL.bit.WRTLOCK=0x00;
	GCLK->CLKCTRL.bit.ID   =GCLK_CLKCTRL_ID_SERCOM2_CORE; // Bits 5:0  - ID: Generic Clock Selection ID (0x14 - SERCOM0_CORE)
	GCLK->CLKCTRL.bit.GEN  =0x00; // Bits 11:8 - GEN: Generic Clock Generator (0: Generic clock generator 0)
	GCLK->CLKCTRL.bit.CLKEN=0x01; // Bit  14   - CLKEN: Clock Enable (1: The generic clock is enabled)

	//.............................................................................

	SERCOM2->USART.CTRLA.bit.ENABLE=0x00; // Bit 1 - ENABLE: Enable (0: The peripheral is disabled or being disabled)
	while(SERCOM2->USART.SYNCBUSY.bit.ENABLE) { ; }

	//.............................................................................

	SERCOM2->USART.CTRLA.bit.SWRST   =0x01; //Bit0-SWRST: Software Reset (Writing a one to this bit resets all registers in the SERCOM, except DBGCTRL, to their initial state, and the SERCOM will be disabled.)
	while(SERCOM2->USART.SYNCBUSY.bit.SWRST) { ; } // Bit 0 - SWRST: Software Reset Synchronization Busy

	SERCOM2->USART.CTRLA.bit.DORD    =0x01;  // Bit  30    - DORD: Data Order (1: LSB is transmitted first.)
	SERCOM2->USART.CTRLA.bit.CPOL    =0x00;  // Bit  29    - CPOL: Clock Polarity (0: SCK is low when idle. The leading edge of a clock cycle is a rising edge, while the trailing edge is a falling edge)
	SERCOM2->USART.CTRLA.bit.CMODE   =0x00;  // Bit  28    - CMODE: Communication Mode (0: Asynchronous communication.)
	SERCOM2->USART.CTRLA.bit.FORM    =0x00;  // Bits 27:24 - FORM: Frame Format (0: USART frame NO PARITY)
	SERCOM2->USART.CTRLA.bit.SAMPA   =0x00;  // Bits 23:22 - SAMPA[1:0]: Sample Adjustment (0: 3-4-5)
	SERCOM2->USART.CTRLA.bit.SAMPR   =0x02;  // Bits 15:13 - SAMPR[2:0]: Sample Rate (2: 8x over-sampling using arithmetic baud rate generation.)
	SERCOM2->USART.CTRLA.bit.IBON    =0x00;  // Bit  8     - IBON: Immediate Buffer Overflow Notification (0: STATUS.BUFOVF is asserted when it occurs in the data stream)
	SERCOM2->USART.CTRLA.bit.RUNSTDBY=0x00;  // Bit  7     - RUNSTDBY: Run In Standby (Generic clock is disabled when ongoing transaction is finished. All interrupts can wake up the device.)
	SERCOM2->USART.CTRLA.bit.MODE    =0x01;  // Bits 4:2   - MODE: Operating Mode (1: USART with internal clock.)
	SERCOM2->USART.CTRLA.bit.TXPO    =0x02;  // Bits 17:16 - TXPO: Transmit Data Pinout (2: TX->PAD[0], RTS->PAD[2])
	SERCOM2->USART.CTRLA.bit.RXPO    =0x01;  // Bits 21:20 - RXPO: Receive Data Pinout (1: RX->PAD[1])
	SERCOM2->USART.CTRLB.bit.PMODE   =0x00;  // Bit  13    - PMODE: Parity Mode (0: Even parity)
	SERCOM2->USART.CTRLB.bit.ENC     =0x00;  // Bit  10    - ENC: Encoding Format (0: Data is not encoded.)
	SERCOM2->USART.CTRLB.bit.SFDE    =0x01;  // Bit  9     - SFDE: Start of Frame Detection Enable (1: Start-of-frame detection enabled.)
	SERCOM2->USART.CTRLB.bit.COLDEN  =0x00;  // Bit  8     - COLDEN: Collision Detection Enable (0: Collision detection is not enabled.)
	SERCOM2->USART.CTRLB.bit.SBMODE  =0x00;  // Bit  6     - SBMODE: Stop Bit Mode (0: One stop bit.)
	SERCOM2->USART.CTRLB.bit.CHSIZE  =0x00;  // Bits 2:0   - CHSIZE: Character Size (0: 8 bits)

	//SERCOM0->USART.BAUD.bit.BAUD=65536.0f*(1.0f-(8.0*(float)(9600))/(float)(8000000));
	SERCOM2->USART.BAUD.bit.BAUD=65536.0f*(1.0f-(8.0*(float)(38400))/(float)(PROC_HERZ));
	//SERCOM0->USART.BAUD.bit.BAUD=65536.0f*(1.0f-(8.0*(float)(115200))/(float)(8000000));

	SERCOM2->USART.INTENSET.bit.RXC  =0x01; // Bit 2 RXC: Receive Complete Interrupt Enable

	while(SERCOM2->USART.SYNCBUSY.reg & 0x07) { ; }
	SERCOM2->USART.CTRLB.bit.RXEN=0x01;     // Bit 17 - RXEN: Receiver Enable (11: The receiver is enabled or will be enabled when the USART is enabled)

	while(SERCOM2->USART.SYNCBUSY.reg & 0x07) { ; }
	SERCOM2->USART.CTRLB.bit.TXEN=0x01;     // Bit 16 - TXEN: Transmitter Enable (1: The transmitter is enabled or will be enabled when the USART is enabled.)

	while(SERCOM2->USART.SYNCBUSY.reg & 0x07) { ; }
	SERCOM2->USART.CTRLA.bit.ENABLE=0x01;   // Bit 1 - ENABLE: Enable (1: The peripheral is enabled or being enabled.)

	tout = (double)1000000/(double)38400;
	tout = tout*bsize;
	tout = tout*4;
	tout = tout/100;
	port[2].rtout = tout;

	NVIC_DisableIRQ(SERCOM2_IRQn);
	NVIC_SetPriority(SERCOM2_IRQn, 1);
	NVIC_EnableIRQ(SERCOM2_IRQn);


	UART_2_RTS_clr();

}
WORD usart_2_write(BYTE* wbuf,WORD wn)
{
	if(wn == 0) { return(0); }
	UART_2_RTS_set();
	LED_TX2_ON();
	wait(1);
	if(wn > USART_BUF_SIZE) { wn = USART_BUF_SIZE; }
	memcpy(port[2].wbuf,wbuf,wn);
	port[2].wn=wn;
	port[2].wx=1;
	SERCOM2->USART.DATA.reg=port[2].wbuf[0];
	SERCOM2->USART.INTENCLR.bit.RXC=1;
	SERCOM2->USART.INTENSET.bit.DRE=1;
	return(wn);
}
WORD usart_2_read (BYTE* rbuf,WORD rn)
{
	WORD size = 0;
	if(port[2].rn     ==       0x00)	{ port[2].rtime=0;return(0); }
	if(port[2].rtout  >  port[2].rtime) { return(0); }
	//!!!!!!!!!!!

	size = port[2].rn;

	if(rn < size) { size = rn;  }


	memcpy(rbuf,port[2].rbuf,size);
	port[2].rn=0;
	port[2].rx=0;
	LED_RX2_OFF();
	
	return(size);
}
void SERCOM2_Handler(void)
{
	
	if(SERCOM2->USART.INTFLAG.bit.RXC)
	{
		port[2].rxc++;
		if(port[2].rn >= USART_BUF_SIZE){port[2].rn=0;}
		port[2].rbuf[port[2].rn] = SERCOM2->USART.DATA.reg;
		port[2].rn   ++;
		port[2].rtime=0;
		LED_RX2_ON();
		return;
	}

	if(SERCOM2->USART.INTFLAG.bit.TXC)
	{
		port[2].txc++;
		SERCOM2->USART.INTFLAG.bit.TXC=1;
		LED_TX2_OFF();
		UART_2_RTS_clr();
		SERCOM2->USART.INTENCLR.bit.TXC=1;
		SERCOM2->USART.INTENSET.bit.RXC=1;
		return;
	}
	
	if(SERCOM2->USART.INTFLAG.bit.DRE)
	{
		port[2].dre++;
		if(port[2].wx < port[2].wn)
		{
			SERCOM2->USART.DATA.reg = port[2].wbuf[port[2].wx];
			port[2].wx  ++;
			return;
		}
		SERCOM2->USART.INTENCLR.bit.DRE=1;
		SERCOM2->USART.INTENSET.bit.TXC=1;
		return;
	}
	

}

//port 3
void usart_3_init(void)
{	
	double tout =0;
	double bsize=0;
	bsize = 9;
		
	PM->APBCMASK.bit.SERCOM3_ =1; // Bit  2    - SERCOM0: SERCOM0 APB Clock Enable (1 - enabled)
	PM->APBCSEL.bit.APBCDIV = 0x00;

	GCLK->CLKCTRL.bit.WRTLOCK=0x00;
	GCLK->CLKCTRL.bit.ID   =GCLK_CLKCTRL_ID_SERCOM3_CORE; // Bits 5:0  - ID: Generic Clock Selection ID (0x14 - SERCOM0_CORE)
	GCLK->CLKCTRL.bit.GEN  =0x00; // Bits 11:8 - GEN: Generic Clock Generator (0: Generic clock generator 0)
	GCLK->CLKCTRL.bit.CLKEN=0x01; // Bit  14   - CLKEN: Clock Enable (1: The generic clock is enabled)

	//.............................................................................

	SERCOM3->USART.CTRLA.bit.ENABLE=0x00; // Bit 1 - ENABLE: Enable (0: The peripheral is disabled or being disabled)
	while(SERCOM3->USART.SYNCBUSY.bit.ENABLE) { ; }

	//.............................................................................

	SERCOM3->USART.CTRLA.bit.SWRST   =0x01; //Bit0-SWRST: Software Reset (Writing a one to this bit resets all registers in the SERCOM, except DBGCTRL, to their initial state, and the SERCOM will be disabled.)
	while(SERCOM3->USART.SYNCBUSY.bit.SWRST) { ; } // Bit 0 - SWRST: Software Reset Synchronization Busy

	SERCOM3->USART.CTRLA.bit.DORD    =0x01;  // Bit  30    - DORD: Data Order (1: LSB is transmitted first.)
	SERCOM3->USART.CTRLA.bit.CPOL    =0x00;  // Bit  29    - CPOL: Clock Polarity (0: SCK is low when idle. The leading edge of a clock cycle is a rising edge, while the trailing edge is a falling edge)
	SERCOM3->USART.CTRLA.bit.CMODE   =0x00;  // Bit  28    - CMODE: Communication Mode (0: Asynchronous communication.)
	SERCOM3->USART.CTRLA.bit.FORM    =0x00;  // Bits 27:24 - FORM: Frame Format (0: USART frame NO PARITY)
	SERCOM3->USART.CTRLA.bit.SAMPA   =0x00;  // Bits 23:22 - SAMPA[1:0]: Sample Adjustment (0: 3-4-5)
	SERCOM3->USART.CTRLA.bit.SAMPR   =0x02;  // Bits 15:13 - SAMPR[2:0]: Sample Rate (2: 8x over-sampling using arithmetic baud rate generation.)
	SERCOM3->USART.CTRLA.bit.IBON    =0x00;  // Bit  8     - IBON: Immediate Buffer Overflow Notification (0: STATUS.BUFOVF is asserted when it occurs in the data stream)
	SERCOM3->USART.CTRLA.bit.RUNSTDBY=0x00;  // Bit  7     - RUNSTDBY: Run In Standby (Generic clock is disabled when ongoing transaction is finished. All interrupts can wake up the device.)
	SERCOM3->USART.CTRLA.bit.MODE    =0x01;  // Bits 4:2   - MODE: Operating Mode (1: USART with internal clock.)
	SERCOM3->USART.CTRLA.bit.TXPO    =0x02;  // Bits 17:16 - TXPO: Transmit Data Pinout (2: TX->PAD[0], RTS->PAD[2])
	SERCOM3->USART.CTRLA.bit.RXPO    =0x01;  // Bits 21:20 - RXPO: Receive Data Pinout (1: RX->PAD[1])
	SERCOM3->USART.CTRLB.bit.PMODE   =0x00;  // Bit  13    - PMODE: Parity Mode (0: Even parity)
	SERCOM3->USART.CTRLB.bit.ENC     =0x00;  // Bit  10    - ENC: Encoding Format (0: Data is not encoded.)
	SERCOM3->USART.CTRLB.bit.SFDE    =0x01;  // Bit  9     - SFDE: Start of Frame Detection Enable (1: Start-of-frame detection enabled.)
	SERCOM3->USART.CTRLB.bit.COLDEN  =0x00;  // Bit  8     - COLDEN: Collision Detection Enable (0: Collision detection is not enabled.)
	SERCOM3->USART.CTRLB.bit.SBMODE  =0x00;  // Bit  6     - SBMODE: Stop Bit Mode (0: One stop bit.)
	SERCOM3->USART.CTRLB.bit.CHSIZE  =0x00;  // Bits 2:0   - CHSIZE: Character Size (0: 8 bits)

	//SERCOM0->USART.BAUD.bit.BAUD=65536.0f*(1.0f-(8.0*(float)(9600))/(float)(8000000));
	SERCOM3->USART.BAUD.bit.BAUD=65536.0f*(1.0f-(8.0*(float)(38400))/(float)(PROC_HERZ));
	//SERCOM0->USART.BAUD.bit.BAUD=65536.0f*(1.0f-(8.0*(float)(115200))/(float)(8000000));

	SERCOM3->USART.INTENSET.bit.RXC  =0x01; // Bit 2 RXC: Receive Complete Interrupt Enable

	while(SERCOM3->USART.SYNCBUSY.reg & 0x07) { ; }
	SERCOM3->USART.CTRLB.bit.RXEN=0x01;     // Bit 17 - RXEN: Receiver Enable (11: The receiver is enabled or will be enabled when the USART is enabled)

	while(SERCOM3->USART.SYNCBUSY.reg & 0x07) { ; }
	SERCOM3->USART.CTRLB.bit.TXEN=0x01;     // Bit 16 - TXEN: Transmitter Enable (1: The transmitter is enabled or will be enabled when the USART is enabled.)

	while(SERCOM3->USART.SYNCBUSY.reg & 0x07) { ; }
	SERCOM3->USART.CTRLA.bit.ENABLE=0x01;   // Bit 1 - ENABLE: Enable (1: The peripheral is enabled or being enabled.)

	tout = (double)1000000/(double)38400;
	tout = tout*bsize;
	tout = tout*4;
	tout = tout/100;
	port[3].rtout = tout;

	NVIC_DisableIRQ(SERCOM3_IRQn);
	NVIC_SetPriority(SERCOM3_IRQn, 1);
	NVIC_EnableIRQ(SERCOM3_IRQn);


	UART_3_RTS_clr();

}
WORD usart_3_write(BYTE* wbuf,WORD wn)
{
	if(wn == 0) { return(0); }
	UART_3_RTS_set();
	LED_TX3_ON();
	wait(1);
	if(wn > USART_BUF_SIZE) { wn = USART_BUF_SIZE; }
	memcpy(port[3].wbuf,wbuf,wn);
	port[3].wn=wn;
	port[3].wx=1;
	SERCOM3->USART.DATA.reg=port[3].wbuf[0];
	SERCOM3->USART.INTENCLR.bit.RXC=1;
	SERCOM3->USART.INTENSET.bit.DRE=1;
	return(wn);
}
WORD usart_3_read (BYTE* rbuf,WORD rn)
{
	WORD size = 0;
	if(port[3].rn     ==       0x00)	{ port[3].rtime=0;return(0); }
	if(port[3].rtout  >  port[3].rtime) { return(0); }
	//!!!!!!!!!!!

	size = port[3].rn;

	if(rn < size) { size = rn;  }


	memcpy(rbuf,port[3].rbuf,size);
	port[3].rn=0;
	port[3].rx=0;
	LED_RX3_OFF();
	
	return(size);
}
void SERCOM3_Handler(void)
{
	
	if(SERCOM3->USART.INTFLAG.bit.RXC)
	{
		port[3].rxc++;
		if(port[3].rn >= USART_BUF_SIZE){port[3].rn=0;}
		port[3].rbuf[port[3].rn] = SERCOM3->USART.DATA.reg;
		port[3].rn   ++;
		port[3].rtime=0;
		LED_RX3_ON();
		return;
	}

	if(SERCOM3->USART.INTFLAG.bit.TXC)
	{
		port[3].txc++;
		SERCOM3->USART.INTFLAG.bit.TXC=1;
		LED_TX3_OFF();
		UART_3_RTS_clr();
		SERCOM3->USART.INTENCLR.bit.TXC=1;
		SERCOM3->USART.INTENSET.bit.RXC=1;
		return;
	}
	
	if(SERCOM3->USART.INTFLAG.bit.DRE)
	{
		port[3].dre++;
		if(port[3].wx < port[3].wn)
		{
			SERCOM3->USART.DATA.reg = port[3].wbuf[port[3].wx];
			port[3].wx  ++;
			return;
		}
		SERCOM3->USART.INTENCLR.bit.DRE=1;
		SERCOM3->USART.INTENSET.bit.TXC=1;
		return;
	}
	

}