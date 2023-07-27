#include "def.h"

volatile USART port[4];
Sercom* sercom[] = {SERCOM1, SERCOM0, SERCOM2, SERCOM3};
DWORD gclk_sercom[] = {_U(0x15),_U(0x14),_U(0x16),_U(0x17)};
BYTE irqn_sercom[] = {10, 9, 11, 12};


void usart_init(BYTE num)
{
	double tout =0;
	double bsize=0;
	bsize = 9;
	
	switch(num)
	{
		case 0:
			PM->APBCMASK.bit.SERCOM1_ = 1; // Bit  2    - SERCOM0: SERCOM0 APB Clock Enable (1 - enabled)
		break;
		case 1:
			PM->APBCMASK.bit.SERCOM0_ = 1; // Bit  2    - SERCOM0: SERCOM0 APB Clock Enable (1 - enabled)
		break;
		case 2:
			PM->APBCMASK.bit.SERCOM2_ = 1; // Bit  2    - SERCOM0: SERCOM0 APB Clock Enable (1 - enabled)
		break;
		case 3:
			PM->APBCMASK.bit.SERCOM3_ = 1; // Bit  2    - SERCOM0: SERCOM0 APB Clock Enable (1 - enabled)
		break;
	}
	
	PM->APBCSEL.bit.APBCDIV   = 0x00;
	GCLK->CLKCTRL.bit.WRTLOCK = 0x00;
	GCLK->CLKCTRL.bit.ID      = gclk_sercom[num]; // Bits 5:0  - ID: Generic Clock Selection ID (0x14 - SERCOM0_CORE)
	GCLK->CLKCTRL.bit.GEN     = 0x00; // Bits 11:8 - GEN: Generic Clock Generator (0: Generic clock generator 0)
	GCLK->CLKCTRL.bit.CLKEN   = 0x01; // Bit  14   - CLKEN: Clock Enable (1: The generic clock is enabled)

	//.............................................................................

	sercom[num]->USART.CTRLA.bit.ENABLE = 0x00; // Bit 1 - ENABLE: Enable (0: The peripheral is disabled or being disabled)
	while(sercom[num]->USART.SYNCBUSY.bit.ENABLE) { ; }

	//.............................................................................

	sercom[num]->USART.CTRLA.bit.SWRST   =0x01; //Bit0-SWRST: Software Reset (Writing a one to this bit resets all registers in the SERCOM, except DBGCTRL, to their initial state, and the SERCOM will be disabled.)
	//SERCOM1->USART.CTRLA.bit.SWRST   =0x01; //Bit0-SWRST: Software Reset (Writing a one to this bit resets all registers in the SERCOM, except DBGCTRL, to their initial state, and the SERCOM will be disabled.)
	while(sercom[num]->USART.SYNCBUSY.bit.SWRST) { ; } // Bit 0 - SWRST: Software Reset Synchronization Busy


	sercom[num]->USART.CTRLA.bit.DORD    =0x01;  // Bit  30    - DORD: Data Order (1: LSB is transmitted first.)
	sercom[num]->USART.CTRLA.bit.CPOL    =0x00;  // Bit  29    - CPOL: Clock Polarity (0: SCK is low when idle. The leading edge of a clock cycle is a rising edge, while the trailing edge is a falling edge)
	sercom[num]->USART.CTRLA.bit.CMODE   =0x00;  // Bit  28    - CMODE: Communication Mode (0: Asynchronous communication.)
	sercom[num]->USART.CTRLA.bit.FORM    =0x00;  // Bits 27:24 - FORM: Frame Format (0: USART frame NO PARITY)
	sercom[num]->USART.CTRLA.bit.SAMPA   =0x00;  // Bits 23:22 - SAMPA[1:0]: Sample Adjustment (0: 3-4-5)
	sercom[num]->USART.CTRLA.bit.SAMPR   =0x02;  // Bits 15:13 - SAMPR[2:0]: Sample Rate (2: 8x over-sampling using arithmetic baud rate generation.)
	sercom[num]->USART.CTRLA.bit.IBON    =0x00;  // Bit  8     - IBON: Immediate Buffer Overflow Notification (0: STATUS.BUFOVF is asserted when it occurs in the data stream)
	sercom[num]->USART.CTRLA.bit.RUNSTDBY=0x00;  // Bit  7     - RUNSTDBY: Run In Standby (Generic clock is disabled when ongoing transaction is finished. All interrupts can wake up the device.)
	sercom[num]->USART.CTRLA.bit.MODE    =0x01;  // Bits 4:2   - MODE: Operating Mode (1: USART with internal clock.)
	sercom[num]->USART.CTRLA.bit.TXPO    =0x02;  // Bits 17:16 - TXPO: Transmit Data Pinout (2: TX->PAD[0], RTS->PAD[2])
	sercom[num]->USART.CTRLA.bit.RXPO    =0x01;  // Bits 21:20 - RXPO: Receive Data Pinout (1: RX->PAD[1])
	sercom[num]->USART.CTRLB.bit.PMODE   =0x00;  // Bit  13    - PMODE: Parity Mode (0: Even parity)
	sercom[num]->USART.CTRLB.bit.ENC     =0x00;  // Bit  10    - ENC: Encoding Format (0: Data is not encoded.)
	sercom[num]->USART.CTRLB.bit.SFDE    =0x01;  // Bit  9     - SFDE: Start of Frame Detection Enable (1: Start-of-frame detection enabled.)
	sercom[num]->USART.CTRLB.bit.COLDEN  =0x00;  // Bit  8     - COLDEN: Collision Detection Enable (0: Collision detection is not enabled.)
	sercom[num]->USART.CTRLB.bit.SBMODE  =0x00;  // Bit  6     - SBMODE: Stop Bit Mode (0: One stop bit.)
	sercom[num]->USART.CTRLB.bit.CHSIZE  =0x00;  // Bits 2:0   - CHSIZE: Character Size (0: 8 bits)

	sercom[num]->USART.BAUD.bit.BAUD=65536.0f*(1.0f-(8.0*(float)(38400))/(float)(PROC_HERZ));

	sercom[num]->USART.INTENSET.bit.RXC  =0x01; // Bit 2 RXC: Receive Complete Interrupt Enable

	while(sercom[num]->USART.SYNCBUSY.reg & 0x07) { ; }
	sercom[num]->USART.CTRLB.bit.RXEN=0x01;     // Bit 17 - RXEN: Receiver Enable (11: The receiver is enabled or will be enabled when the USART is enabled)

	while(sercom[num]->USART.SYNCBUSY.reg & 0x07) { ; }
	sercom[num]->USART.CTRLB.bit.TXEN=0x01;     // Bit 16 - TXEN: Transmitter Enable (1: The transmitter is enabled or will be enabled when the USART is enabled.)

	while(sercom[num]->USART.SYNCBUSY.reg & 0x07) { ; }
	sercom[num]->USART.CTRLA.bit.ENABLE=0x01;   // Bit 1 - ENABLE: Enable (1: The peripheral is enabled or being enabled.)

	tout = (double)1000000/(double)38400;
	tout = tout*bsize;
	tout = tout*4;
	tout = tout/100;
	port[num].rtout = tout;

	NVIC_DisableIRQ(irqn_sercom[num]);
	NVIC_SetPriority(irqn_sercom[num], 1);
	NVIC_EnableIRQ(irqn_sercom[num]);

	gpio_ctrl(RTS, num, CLR);
}
WORD usart_write(BYTE num, BYTE* wbuf,WORD wn)
{
	if(wn == 0) { return(0); }
	num -= 1;
	//UART_0_RTS_set();
	gpio_ctrl(RTS, num, SET);
	//LED_TX0_ON();
	gpio_ctrl(LED_TX, num, ON);
	
	if(wn > USART_BUF_SIZE) { wn = USART_BUF_SIZE; }
	memcpy(port[num].wbuf,wbuf,wn);
	port[num].wn=wn;
	port[num].wx=1;
	sercom[num]->USART.DATA.reg=port[num].wbuf[num];
	sercom[num]->USART.INTENCLR.bit.RXC=1;
	sercom[num]->USART.INTENSET.bit.DRE=1;
	return(wn);
}
WORD usart_read (BYTE num, BYTE* rbuf,WORD rn)
{
	WORD size = 0;
	num -= 1;
	if(port[num].rn     ==       0x00) { port[num].rtime=0;return(0); }
	if(port[num].rtout  >  port[num].rtime) { return(0); }

	size = port[num].rn;

	if(rn < size) { size = rn;  }

	memcpy(rbuf,port[num].rbuf,size);
	port[num].rn=0;
	port[num].rx=0;
	gpio_ctrl(LED_RX, num, OFF);
	counters.rs485[num].rx++;
	return(size);
}

void SERCOM1_Handler(void)
{
	if(SERCOM1->USART.INTFLAG.bit.RXC)
	{

		port[0].rxc++;
		if(port[0].rn >= USART_BUF_SIZE){port[0].rn=0;}
		port[0].rbuf[port[0].rn] = sercom[0]->USART.DATA.reg;
		port[0].rn   ++;
		port[0].rtime=0;
		//LED_RX0_ON();
		gpio_ctrl(LED_RX, 0, ON);
		
		return;
	}

	if(SERCOM1->USART.INTFLAG.bit.TXC)
	{
		port[0].txc++;
		SERCOM1->USART.INTFLAG.bit.TXC=1;
		//LED_TX0_OFF();
		gpio_ctrl(LED_TX, 0, OFF);
		//UART_0_RTS_clr();
		gpio_ctrl(RTS, 0, CLR);
		SERCOM1->USART.INTENCLR.bit.TXC=1;
		SERCOM1->USART.INTENSET.bit.RXC=1;
		counters.rs485[0].tx++;
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

void SERCOM0_Handler(void)
{
	
	if(SERCOM0->USART.INTFLAG.bit.RXC)
	{
		port[1].rxc++;
		if(port[1].rn >= USART_BUF_SIZE){port[1].rn=0;}
		port[1].rbuf[port[1].rn] = SERCOM0->USART.DATA.reg;
		port[1].rn   ++;
		port[1].rtime=0;
		//LED_RX1_ON();
		return;
	}

	if(SERCOM0->USART.INTFLAG.bit.TXC)
	{
		port[1].txc++;
		SERCOM0->USART.INTFLAG.bit.TXC=1;
		//LED_TX1_OFF();
		//UART_1_RTS_clr();
		SERCOM0->USART.INTENCLR.bit.TXC=1;
		SERCOM0->USART.INTENSET.bit.RXC=1;
		counters.rs485[1].tx++;
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

void SERCOM2_Handler(void)
{
	
	if(SERCOM2->USART.INTFLAG.bit.RXC)
	{
		port[2].rxc++;
		if(port[2].rn >= USART_BUF_SIZE){port[2].rn=0;}
		port[2].rbuf[port[2].rn] = SERCOM2->USART.DATA.reg;
		port[2].rn   ++;
		port[2].rtime=0;
		//LED_RX2_ON();
		return;
	}

	if(SERCOM2->USART.INTFLAG.bit.TXC)
	{
		port[2].txc++;
		SERCOM2->USART.INTFLAG.bit.TXC=1;
		//LED_TX2_OFF();
		//UART_2_RTS_clr();
		SERCOM2->USART.INTENCLR.bit.TXC=1;
		SERCOM2->USART.INTENSET.bit.RXC=1;
		counters.rs485[2].tx++;
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

void SERCOM3_Handler(void)
{
	
	if(SERCOM3->USART.INTFLAG.bit.RXC)
	{
		port[3].rxc++;
		if(port[3].rn >= USART_BUF_SIZE){port[3].rn=0;}
		port[3].rbuf[port[3].rn] = SERCOM3->USART.DATA.reg;
		port[3].rn   ++;
		port[3].rtime=0;
		//LED_RX3_ON();
		return;
	}

	if(SERCOM3->USART.INTFLAG.bit.TXC)
	{
		port[3].txc++;
		SERCOM3->USART.INTFLAG.bit.TXC=1;
		//LED_TX3_OFF();
		//UART_3_RTS_clr();
		SERCOM3->USART.INTENCLR.bit.TXC=1;
		SERCOM3->USART.INTENSET.bit.RXC=1;
		counters.rs485[3].tx++;
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