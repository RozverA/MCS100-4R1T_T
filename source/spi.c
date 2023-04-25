#include "def.h"

SPI spi;


void spi_init(void)
//-----------------------------------------------------------------------------
{
	PM->APBCMASK.bit.SERCOM5_ =1; // Bit  2    - SERCOM4: SERCOM4 APB Clock Enable (1 - enabled)
	GCLK->CLKCTRL.bit.ID   =GCLK_CLKCTRL_ID_SERCOM5_CORE; // Bits 5:0  - ID: Generic Clock Selection ID (0x18 - SERCOM4_CORE)
	GCLK->CLKCTRL.bit.GEN  =0x00; // Bits 11:8 - GEN: Generic Clock Generator (0: Generic clock generator 0)
	GCLK->CLKCTRL.bit.CLKEN=0x01; // Bit  14   - CLKEN: Clock Enable (1: The generic clock is enabled)

	//.............................................................................

	if(SERCOM5->SPI.SYNCBUSY.bit.SWRST)   // Bit 0 - SWRST: Software Reset Synchronization Busy
	{
		if(SERCOM5->SPI.CTRLA.bit.ENABLE)   // Bit 1 - ENABLE: Enable
		{
			while(SERCOM5->SPI.SYNCBUSY.bit.ENABLE) { ; }
			SERCOM5->SPI.CTRLA.bit.ENABLE=0x00; // Bit 1 - ENABLE: Enable (0: The peripheral is disabled or being disabled)
			while(SERCOM5->SPI.SYNCBUSY.bit.ENABLE) { ; }
		}
	}

	//.............................................................................

	SERCOM5->SPI.CTRLA.bit.SWRST   =0x01; // Bit  0     - SWRST: Software Reset (Writing a one to this bit resets all registers in the SERCOM, except DBGCTRL, to their initial state, and the SERCOM will be disabled.)
	while(SERCOM5->SPI.SYNCBUSY.bit.SWRST) { ; } // Bit 0 - SWRST: Software Reset Synchronization Busy

	SERCOM5->SPI.CTRLA.bit.DORD    =0x00; // Bit  30    - DORD: Data Order (0: MSB is transferred first)
	SERCOM5->SPI.CTRLA.bit.CPOL    =0x00; // Bit  29    - CPOL: Clock Polarity (0: SCK is low when idle. The leading edge of a clock cycle is a rising edge, while the trailing edge is a falling edge)
	SERCOM5->SPI.CTRLA.bit.CPHA    =0x00; // Bit  28    - CPHA: Clock Phase (0: The data is sampled on a leading SCK edge and changed on a trailing SCK edge)
	SERCOM5->SPI.CTRLA.bit.FORM    =0x00; // Bits 27:24 - FORM: Frame Format (0: SPI frame)
	SERCOM5->SPI.CTRLA.bit.IBON    =0x00; // Bit  8     - IBON: Immediate Buffer Overflow Notification (0: STATUS.BUFOVF is asserted when it occurs in the data stream)
	SERCOM5->SPI.CTRLA.bit.RUNSTDBY=0x00; // Bit  7     - RUNSTDBY: Run In Standby (Generic clock is disabled when ongoing transaction is finished. All interrupts can wake up the device.)
	SERCOM5->SPI.CTRLA.bit.MODE    =0x03; // Bits 4:2   - MODE: Operating Mode (3: SPI master operation)

	while(SERCOM5->SPI.SYNCBUSY.reg & 0x07) { ; }
	SERCOM5->SPI.CTRLA.bit.DOPO=0x02;     // Bits 17:16 - DOPO: Data Out Pinout (0: MOSI->PAD[3], SCK->PAD[1])

	while(SERCOM5->SPI.SYNCBUSY.reg & 0x07) { ; }
	SERCOM5->SPI.CTRLA.bit.DIPO=0x00;     // Bits 21:20 - DIPO: Data In Pinout (0: SERCOM PAD[0] is used as data input)

	SERCOM5->SPI.CTRLB.bit.RXEN   =0x01;  // Bit  17    - RXEN: Receiver Enable (1: The receiver is enabled or it will be enabled when SPI is enabled.)
	SERCOM5->SPI.CTRLB.bit.MSSEN  =0x00;  // Bit  13    - MSSEN: Master Slave Select Enable (0: Hardware _SS control is disabled)
	SERCOM5->SPI.CTRLB.bit.AMODE  =0x00;  // Bits 15:14 - AMODE: Address Mode (0: ADDRMASK is used as a mask to the ADDR register)
	SERCOM5->SPI.CTRLB.bit.SSDE   =0x00;  // Bit  9     - SSDE: Slave Select Low Detect Enable (0: _SS low detector is disabled.)
	SERCOM5->SPI.CTRLB.bit.PLOADEN=0x00;  // Bit 6      – PLOADEN: Slave Data Preload Enable ()
	SERCOM5->SPI.CTRLB.bit.CHSIZE =0x00;  // Bits 2:0   - CHSIZE: Character Size (0: 8 bits)

	//SERCOM5->SPI.BAUD.bit.BAUD   =0x00;   // 0: 16MHz/1
     /*
     / Fix the baud rate at 1000000
     / SystemCoreClock / (2 * baudrate) - 1
     / SystemCoreClock = 8000000
     / baudrate = 1000000
    */	

	SERCOM5->SPI.BAUD.bit.BAUD        = (float)(48000000 ) / (2 * (float)(8000000 )) - 1;
	SERCOM5->SPI.INTENSET.bit.TXC	  = 0x00;
	SERCOM5->SPI.INTENSET.bit.DRE     = 0x00;
	SERCOM5->SPI.INTENSET.bit.RXC     = 0x00;	

	while(SERCOM5->SPI.SYNCBUSY.reg & 0x07) { ; }
	SERCOM5->SPI.CTRLA.bit.ENABLE=0x01;   // Bit 1 - ENABLE: Enable (1: The peripheral is enabled or being enabled.)

	NVIC_DisableIRQ(SERCOM5_IRQn);
	NVIC_SetPriority(SERCOM5_IRQn, 1);
	NVIC_EnableIRQ(SERCOM5_IRQn);
}


WORD spi_process (uint16_t addr, uint8_t cb, uint8_t *buf, uint16_t len, BYTE cmd)
{
	BYTE st_spi=0;
	WORD rn_wn=0;
	st_spi = cmd;
	switch(st_spi)
	{
		case 0:
			return (1);
		break;
		case READ_DATA:
			rn_wn=spi_read(addr,cb,buf,len);
		break;
		case WRITE_DATA:
			rn_wn=spi_write(addr,cb,buf,len);
		break;			
	}
	return (rn_wn);
}

WORD spi_write(uint16_t addr, uint8_t cb, uint8_t *tx_buf, uint16_t len)
{
	WORD cnt=0;
	static BYTE st_read=0;
	
	switch (st_read)
	{
		case 0:
			CS_clr();			
			spi.wbuf[cnt]=(addr >> 8);						cnt++;
			spi.wbuf[cnt]=(addr & 0xFF);					cnt++;
			spi.wbuf[cnt]=cb | WRITE_REG | LEN_N_BYTES;		cnt++;
							
			if((len+cnt)>MAX_SIZE_BUF_SPI)		{len=MAX_SIZE_BUF_SPI-cnt;}
			memcpy(&spi.wbuf[cnt],tx_buf,len);  cnt=cnt+len;
							
			spi.wx = cnt; 
			spi.wn = 0;
			spi.rn = 0;
							
			if(spi.wx>MAX_SIZE_BUF_SPI)			{spi.wx=MAX_SIZE_BUF_SPI;}
			st_read++;
			SERCOM5->SPI.INTENSET.bit.DRE=0x01;
			SERCOM5->SPI.INTENSET.bit.RXC=0x01;							
		break;
		case 1:	
			if(spi.wx)							{return 0;}
			if(spi.rn!=spi.wn)					{return 0;}
			st_read--;
			CS_set();
			
			return (spi.rn);
		break;
	}
	return 0;
}

WORD spi_read(uint16_t addr, uint8_t cb, uint8_t *rx_buf, uint16_t len)
{
	WORD cnt=0;
	static BYTE st_read=0;
	
	switch (st_read)
	{
		case 0:
			CS_clr();
			spi.wbuf[cnt]=(addr >> 8);						cnt++;
			spi.wbuf[cnt]=(addr & 0xFF);					cnt++;
			spi.wbuf[cnt]=cb | READ_REG | LEN_N_BYTES;		cnt++;
			spi.wx = cnt + len;
			spi.wn = 0;
			spi.rn = 0;
			if(spi.wx>MAX_SIZE_BUF_SPI) {spi.wx=MAX_SIZE_BUF_SPI;}
			st_read++;
			SERCOM5->SPI.INTENSET.bit.DRE=0x01;
			SERCOM5->SPI.INTENSET.bit.RXC=0x01;
		break;
		case 1:
			if(spi.wx)							{return 0;}
			if(spi.rn!=spi.wn)					{return 0;}
			memcpy(rx_buf,&spi.rbuf[3],spi.rn-3);
			st_read--;
			CS_set();
				
			return (spi.rn);
		break;
	}
	return 0;
}

void SERCOM5_Handler(void)
//-----------------------------------------------------------------------------
{
	
	if(SERCOM5->SPI.INTFLAG.bit.RXC)
	{
		spi.rbuf[spi.rn] = SERCOM5->SPI.DATA.reg;
		spi.rn++;	
		spi.RXC_sum++;
		if(!spi.wx) 
		{
			SERCOM5->SPI.INTENCLR.bit.RXC =0x01;
		}	
	}
	if(SERCOM5->SPI.INTFLAG.bit.DRE)
	{
		spi.DRE_sum++;
		if(spi.rn!=spi.wn){return;}
		if(spi.wx)
		{ 
			SERCOM5->SPI.DATA.reg=spi.wbuf[spi.wn];
			spi.wn++;
			spi.wx--;
		}
		else {SERCOM5->SPI.INTENCLR.bit.DRE =0x01;}
	}
}



