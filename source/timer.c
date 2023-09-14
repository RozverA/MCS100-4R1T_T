#include "def.h"

volatile DWORD  time_wait;
volatile WORD	spi_wait;
volatile WORD	cmd_spi_wait;
volatile DWORD  tc3_cnt=0;

volatile WORD	ger_wait = 0;

volatile DWORD  time_100mk=0;
volatile DWORD  TTL=0;

void TC3_Handler(void)
//-----------------------------------------------------------------------------
{
	TC3->COUNT16.INTFLAG.reg|=0xFFFF;
	
	tc3_cnt++;
	time_wait++;
	port[0].rtime++;
	port[1].rtime++;
	port[2].rtime++;
	port[3].rtime++;
	
	
	if(spi_wait--);
	if(cmd_spi_wait--);
}

void TC3_init(void)
//-----------------------------------------------------------------------------
{
	GCLK->CLKCTRL.bit.ID   =0x1B; // Bits 5:0  - ID: Generic Clock Selection ID (0x1B - TCC2,TC3)
	GCLK->CLKCTRL.bit.GEN  =0x00; // Bits 11:8 - GEN: Generic Clock Generator (0: Generic clock generator 0)
	GCLK->CLKCTRL.bit.CLKEN=0x01; // Bit  14   - CLKEN: Clock Enable (1: The generic clock is enabled)

	PM->APBCSEL.bit.APBCDIV=0x00;
	PM->APBCMASK.bit.TC3_  =0x01; //

	TC3->COUNT16.CTRLA.bit.MODE     =0x0; // Bits 3:2   - MODE: (0-Counter in 16-bit mode)
	TC3->COUNT16.CTRLA.bit.PRESCALER=TC_CTRLA_PRESCALER_DIV16_Val; // Bits 10:8  - PRESCALER: Prescaler (4: GCLK_TC/16)
	TC3->COUNT16.CTRLA.bit.PRESCSYNC=0x00; // Bits 13:12 - PRESCSYNC: Prescaler and Counter Synchronization (Reload or reset the counter on next generic clock)
	TC3->COUNT16.CTRLA.bit.WAVEGEN  =0x01; // Bits 6:5   - WAVEGEN: Waveform Generation Operation

	TC3->COUNT16.CC[0].reg=0xFFFF;
	TC3->COUNT16.CC[1].reg=0xFFFF;

	//TC3->COUNT16.CTRLBSET.bit.ONESHOT=0x00; // Bit 2 - ONESHOT: One-Shot (1: The timer/counter will wrap around and stop on the next underflow/overflow condition.)
	TC3->COUNT16.CTRLBSET.bit.DIR    =0x00; // Bit 0 - DIR: Counter Direction (0: The timer/counter is counting up (incrementing))
	TC3->COUNT16.CTRLA.bit.ENABLE    =0x01; // Bit 1 - ENABLE: Enable

	TC3->COUNT16.CC[0].reg=0xFFFF;
	TC3->COUNT16.CC[1].reg=0xFFFF;
	
	while(TC3->COUNT16.STATUS.bit.SYNCBUSY == 0x01); // wait for TC3 to be enabled

	NVIC_DisableIRQ(TC3_IRQn);					// Disable the interrupt
	//TC3->COUNT16.INTENSET.bit.MC0=0x01;	// Enable match interrupts on compare channel 0
	TC3->COUNT16.INTENSET.bit.OVF=0x01;
	NVIC_SetPriority(TC3_IRQn, 0);			// Set the interrupt priority to lowest value
	NVIC_EnableIRQ(TC3_IRQn);						// Enable the interrupt
	
	TC3_start(TC3_100mk);
}


	void wait(DWORD time_100mk)
	{
	time_wait=0;
	while(1){if(reset   == NULL) {wdt_reset();}if(time_wait>time_100mk){break;}}
	}
	
	
	
	void tc3_process(void)
	//-----------------------------------------------------------------------------
	{
		if(!tc3_cnt) {return;}
			
		if(port[0].time_port){port[0].time_port--;}
		if(port[1].time_port){port[1].time_port--;}
		if(port[2].time_port){port[2].time_port--;}
		if(port[3].time_port){port[3].time_port--;}
				
		if(eth_sock[0].time_wait_SEND_OK){eth_sock[0].time_wait_SEND_OK--;}
		if(eth_sock[1].time_wait_SEND_OK){eth_sock[1].time_wait_SEND_OK--;}
		if(eth_sock[2].time_wait_SEND_OK){eth_sock[2].time_wait_SEND_OK--;}
		if(eth_sock[3].time_wait_SEND_OK){eth_sock[3].time_wait_SEND_OK--;}
		if(eth_sock[4].time_wait_SEND_OK){eth_sock[4].time_wait_SEND_OK--;}	
		
		time_100mk=time_100mk+tc3_cnt;	
		if(time_100mk>=10000){time_100mk=0;TTL++;}
			
		tc3_cnt=0;		
		
	}
