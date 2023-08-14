#include "def.h"

volatile USART port[4];

void usart_init(){for(BYTE i = 0; i < 4; i++) {init(i);}}
	
void init(BYTE n_port)
{
	double tout = 0;
	double bsize = 0;
	DWORD val = 0;
	bsize = 9;

	switch(n_port)
	{
		case 0:
			PM->APBCMASK.bit.SERCOM1_ = 1; // Bit  2    - SERCOM0: SERCOM0 APB Clock Enable (1 - enabled)
			port[n_port].sercom = SERCOM1;
			port[n_port].gclk_sercom = GCLK_CLKCTRL_ID_SERCOM1_CORE;
			port[n_port].irqn_sercom = SERCOM1_IRQn;
		break;
		case 1:
			PM->APBCMASK.bit.SERCOM0_ = 1; // Bit  2    - SERCOM0: SERCOM0 APB Clock Enable (1 - enabled)
			port[n_port].sercom = SERCOM0;
			port[n_port].gclk_sercom = GCLK_CLKCTRL_ID_SERCOM0_CORE;
			port[n_port].irqn_sercom = SERCOM0_IRQn;
		break;
		case 2:
			PM->APBCMASK.bit.SERCOM2_ = 1; // Bit  2    - SERCOM0: SERCOM0 APB Clock Enable (1 - enabled)
			port[n_port].sercom = SERCOM2;
			port[n_port].gclk_sercom = GCLK_CLKCTRL_ID_SERCOM2_CORE;
			port[n_port].irqn_sercom = SERCOM2_IRQn;
		break;
		case 3:
			PM->APBCMASK.bit.SERCOM3_ = 1; // Bit  2    - SERCOM0: SERCOM0 APB Clock Enable (1 - enabled)
			port[n_port].sercom = SERCOM3;
			port[n_port].gclk_sercom = GCLK_CLKCTRL_ID_SERCOM3_CORE;
			port[n_port].irqn_sercom = SERCOM3_IRQn;
		break;
	}
	
	PM->APBCSEL.bit.APBCDIV   = 0x00;
	GCLK->CLKCTRL.bit.WRTLOCK = 0x00;
	GCLK->CLKCTRL.bit.ID      = port[n_port].gclk_sercom; // Bits 5:0  - ID: Generic Clock Selection ID (0x14 - SERCOM0_CORE)
	GCLK->CLKCTRL.bit.GEN     = 0x00; // Bits 11:8 - GEN: Generic Clock Generator (0: Generic clock generator 0)
	GCLK->CLKCTRL.bit.CLKEN   = 0x01; // Bit  14   - CLKEN: Clock Enable (1: The generic clock is enabled)

	//.............................................................................

	port[n_port].sercom->USART.CTRLA.bit.ENABLE = 0x00; // Bit 1 - ENABLE: Enable (0: The peripheral is disabled or being disabled)
	while(port[n_port].sercom->USART.SYNCBUSY.bit.ENABLE) { ; }

	//.............................................................................

	port[n_port].sercom->USART.CTRLA.bit.SWRST   =0x01; //Bit0-SWRST: Software Reset (Writing a one to this bit resets all registers in the SERCOM, except DBGCTRL, to their initial state, and the SERCOM will be disabled.)
	while(port[n_port].sercom->USART.SYNCBUSY.bit.SWRST) { ; } // Bit 0 - SWRST: Software Reset Synchronization Busy

	port[n_port].sercom->USART.CTRLA.bit.DORD    =0x01;									// Bit  30    - DORD: Data Order (1: LSB is transmitted first.)
	port[n_port].sercom->USART.CTRLA.bit.CPOL    =0x00;									// Bit  29    - CPOL: Clock Polarity (0: SCK is low when idle. The leading edge of a clock cycle is a rising edge, while the trailing edge is a falling edge)
	port[n_port].sercom->USART.CTRLA.bit.CMODE   =0x00;									// Bit  28    - CMODE: Communication Mode (0: Asynchronous communication.)
	port[n_port].sercom->USART.CTRLA.bit.SAMPA   =0x00;									// Bits 23:22 - SAMPA[1:0]: Sample Adjustment (0: 3-4-5)
	port[n_port].sercom->USART.CTRLA.bit.SAMPR   =0x02;									// Bits 15:13 - SAMPR[2:0]: Sample Rate (2: 8x over-sampling using arithmetic baud rate generation.)
	port[n_port].sercom->USART.CTRLA.bit.IBON    =0x00;									// Bit  8     - IBON: Immediate Buffer Overflow Notification (0: STATUS.BUFOVF is asserted when it occurs in the data stream)
	port[n_port].sercom->USART.CTRLA.bit.RUNSTDBY=0x00;									// Bit  7     - RUNSTDBY: Run In Standby (Generic clock is disabled when ongoing transaction is finished. All interrupts can wake up the device.)
	port[n_port].sercom->USART.CTRLA.bit.MODE    =0x01;									// Bits 4:2   - MODE: Operating Mode (1: USART with internal clock.)
	port[n_port].sercom->USART.CTRLA.bit.TXPO    =0x02;									// Bits 17:16 - TXPO: Transmit Data Pinout (2: TX->PAD[0], RTS->PAD[2])
	port[n_port].sercom->USART.CTRLA.bit.RXPO    =0x01;									// Bits 21:20 - RXPO: Receive Data Pinout (1: RX->PAD[1])
	port[n_port].sercom->USART.CTRLB.bit.ENC     =0x00;									// Bit  10    - ENC: Encoding Format (0: Data is not encoded.)
	port[n_port].sercom->USART.CTRLB.bit.SFDE    =0x01;									// Bit  9     - SFDE: Start of Frame Detection Enable (1: Start-of-frame detection enabled.)
	port[n_port].sercom->USART.CTRLB.bit.COLDEN  =0x00;									// Bit  8     - COLDEN: Collision Detection Enable (0: Collision detection is not enabled.)
	

	//cfg settings
	//frame
	if (cfg.sock_rs485[n_port].parity == PARITY_NONE) {val = FRAME_NO_PARITY;}	else {val = FRAME_WITH_PARITY;}					// Bits 27:24 - FORM: Frame Format (0: USART frame NO PARITY)
	port[n_port].sercom->USART.CTRLA.bit.FORM    = val;
	//parity
	if( 2 < cfg.sock_rs485[n_port].parity < 1) { val = cfg.sock_rs485[n_port].parity;} else { val = 0;}							// Bit  13    - PMODE: Parity Mode (0: Even parity)
	port[n_port].sercom->USART.CTRLB.bit.PMODE = val;
	//stop bit
	if( 0 <= cfg.sock_rs485[n_port].stop <= 1)	{port[n_port].sercom->USART.CTRLB.bit.SBMODE  = cfg.sock_rs485[n_port].stop;}	// Bit  6     - SBMODE: Stop Bit Mode (0: One stop bit.)
	//char size
	switch (cfg.sock_rs485[n_port].bsize)
	{
		case 8:	port[n_port].sercom->USART.CTRLB.bit.CHSIZE = 0x0; break;
		case 7:	port[n_port].sercom->USART.CTRLB.bit.CHSIZE = 0x7; break;
		default:port[n_port].sercom->USART.CTRLB.bit.CHSIZE = 0x0; break;
	}
	//baud
	val = cfg.sock_rs485[n_port].baud;
	if ( !((val == 0x2580) || (val == 0x9600) || (val == 0x1C200)) ) {val = 0x9600;}
	port[n_port].sercom->USART.BAUD.bit.BAUD = 65536.0f*(1.0f-(8.0*(float)(val))/(float)(PROC_HERZ)); 



	port[n_port].sercom->USART.INTENSET.bit.RXC  =0x01; // Bit 2 RXC: Receive Complete Interrupt Enable

	while(port[n_port].sercom->USART.SYNCBUSY.reg & 0x07) { ; }
	port[n_port].sercom->USART.CTRLB.bit.RXEN=0x01;     // Bit 17 - RXEN: Receiver Enable (11: The receiver is enabled or will be enabled when the USART is enabled)

	while(port[n_port].sercom->USART.SYNCBUSY.reg & 0x07) { ; }
	port[n_port].sercom->USART.CTRLB.bit.TXEN=0x01;     // Bit 16 - TXEN: Transmitter Enable (1: The transmitter is enabled or will be enabled when the USART is enabled.)

	while(port[n_port].sercom->USART.SYNCBUSY.reg & 0x07) { ; }
	port[n_port].sercom->USART.CTRLA.bit.ENABLE=0x01;   // Bit 1 - ENABLE: Enable (1: The peripheral is enabled or being enabled.)

	tout = (double)1000000 / (double)38400;
	tout = tout * bsize;
	tout = tout * 4;
	tout = tout / 100;
	port[n_port].rtout = tout;

	NVIC_DisableIRQ(port[n_port].irqn_sercom);
	NVIC_SetPriority(port[n_port].irqn_sercom, 1);
	NVIC_EnableIRQ(port[n_port].irqn_sercom);
	pin_ctrl(RTS, n_port, CLR);
}

WORD usart_write(BYTE n_port, BYTE* wbuf,WORD wn)
{
	if(wn == 0) { return(0); }
	pin_ctrl(RTS, n_port, SET);
	pin_ctrl(LED_TX, n_port, ON);
	
	if(wn > USART_BUF_SIZE) { wn = USART_BUF_SIZE; }
	memcpy(port[n_port].wbuf,wbuf,wn);
	port[n_port].wn = wn;
	port[n_port].wx = 1;
	port[n_port].sercom->USART.DATA.reg=port[n_port].wbuf[0];
	port[n_port].sercom->USART.INTENCLR.bit.RXC = 1;
	port[n_port].sercom->USART.INTENSET.bit.DRE = 1;
	port[n_port].counters.tx++;
	return(wn);
}

WORD usart_read (BYTE n_port, BYTE* rbuf,WORD rn)
{
	WORD size = 0;
	if(port[n_port].rn     ==       0x00)			{ port[n_port].rtime=0; return(0); }
	if(port[n_port].rtout  >  port[n_port].rtime)	{ return(0); }

	size = port[n_port].rn;

	if(rn < size) { size = rn;}
	
	if(size > USART_BUF_SIZE) { size = USART_BUF_SIZE; }
	memcpy(rbuf,port[n_port].rbuf,size);
	port[n_port].rn = 0;
	port[n_port].rx = 0;
	pin_ctrl(LED_RX, n_port, OFF);
	port[n_port].counters.rx++;
	return(size);
}

void sercom_proc(BYTE n_port)
{
	if (port[n_port].sercom->USART.STATUS.bit.COLL)		{er_list.usart[n_port].collision++;	port[n_port].sercom->USART.STATUS.bit.COLL = 1;}
	if (port[n_port].sercom->USART.STATUS.bit.ISF)		{er_list.usart[n_port].synchr++;		port[n_port].sercom->USART.STATUS.bit.ISF = 1;}
	if (port[n_port].sercom->USART.STATUS.bit.CTS)		{er_list.usart[n_port].CTS++;		/*port[i].sercom->USART.STATUS.bit.CTS = 1;*/}
	if (port[n_port].sercom->USART.STATUS.bit.BUFOVF)	{er_list.usart[n_port].buf_ovf++;	port[n_port].sercom->USART.STATUS.bit.BUFOVF = 1;}
	if (port[n_port].sercom->USART.STATUS.bit.FERR)		{er_list.usart[n_port].st_bit++;		port[n_port].sercom->USART.STATUS.bit.FERR = 1;}
	if (port[n_port].sercom->USART.STATUS.bit.PERR)		{er_list.usart[n_port].prty++;		port[n_port].sercom->USART.STATUS.bit.PERR = 1;}
	if (port[n_port].sercom->USART.INTFLAG.bit.RXC)
	{
		port[n_port].rxc++;
		if(port[n_port].rn >= USART_BUF_SIZE){port[n_port].rn = 0;}
		port[n_port].rbuf[port[n_port].rn] = port[n_port].sercom->USART.DATA.reg;
		port[n_port].rn++;
		port[n_port].rtime = 0;
		pin_ctrl(LED_RX, n_port, ON);
		return;
	}

	if(port[n_port].sercom->USART.INTFLAG.bit.TXC)
	{
		port[n_port].txc++;
		port[n_port].sercom->USART.INTFLAG.bit.TXC = 1;
		pin_ctrl(LED_TX, n_port, OFF);
		pin_ctrl(RTS, n_port, CLR);
		port[n_port].sercom->USART.INTENCLR.bit.TXC = 1;
		port[n_port].sercom->USART.INTENSET.bit.RXC = 1;
		return;
	}
	
	if(port[n_port].sercom->USART.INTFLAG.bit.DRE)
	{
		port[n_port].dre++;
		if(port[n_port].wx < port[n_port].wn)
		{
			port[n_port].sercom->USART.DATA.reg = port[n_port].wbuf[port[n_port].wx];
			port[n_port].wx  ++;
			return;
		}
		port[n_port].sercom->USART.INTENCLR.bit.DRE=1;
		port[n_port].sercom->USART.INTENSET.bit.TXC=1;
		return;
	}
};

void SERCOM1_Handler(void)
{
	sercom_proc(0);
}

void SERCOM0_Handler(void)
{
	sercom_proc(1);
}

void SERCOM2_Handler(void)
{
	sercom_proc(2);
}

void SERCOM3_Handler(void)
{
	sercom_proc(3);
}