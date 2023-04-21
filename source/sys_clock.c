#include "def.h"



void sys_clock_init_USE_OSC8M(void)
//-----------------------------------------------------------------------------
{
	uint32_t temp = SYSCTRL->OSC8M.reg;
	temp &= ~((uint32_t)0x0000FFFF);
	temp |= SYSCTRL_OSC8M_PRESC(0x00) | SYSCTRL_OSC8M_ENABLE;
	SYSCTRL->OSC8M.reg = temp;
	while (!(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_OSC8MRDY)){};
}


void sys_clock_init_USE_DFLL48M(void)
{
	uint32_t coarse, fine;

	NVMCTRL->CTRLB.bit.RWS=2; //this is needed for 48 MHz

	coarse=NVM_READ_CAL(DFLL48M_COARSE_CAL); //read coarse calibration value from NVM Software Calibration Area Map
	fine  =NVM_READ_CAL(DFLL48M_FINE_CAL  ); //read fine calibration value from NVM Software Calibration Area Map

	SYSCTRL->DFLLCTRL.reg=0; // See Errata 9905
	while((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0) { ; }

	SYSCTRL->DFLLVAL.reg=SYSCTRL_DFLLVAL_COARSE(coarse) | SYSCTRL_DFLLVAL_FINE(fine); //this will set DFLL48 close to 48 MHz
	
	SYSCTRL->DFLLCTRL.bit.STABLE	=1; // Stable frequency (FINE calibration register value will be fixed after a fine lock)
	SYSCTRL->DFLLCTRL.bit.BPLCKC	=1; // Bypass coarse clock enable
	SYSCTRL->DFLLCTRL.bit.MODE		=0; // Open loop mode
	SYSCTRL->DFLLCTRL.bit.ONDEMAND  =0; // On Demand is disabled, the oscillator will always be running when enabled.
	SYSCTRL->DFLLCTRL.bit.ENABLE	=1; // Enable DFLL48
	SYSCTRL->DFLLCTRL.bit.CCDIS		=1; // Chill Cycle is disabled
	while((SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0) { ; } // wait till DFLL is stable and ready to be used as a clock source

	GCLK->GENDIV.reg =GCLK_GENDIV_ID (GCLK_CLKCTRL_GEN_GCLK0_Val) | GCLK_GENDIV_DIV (MAIN_CLK_DEVIDER);//16MHz
	GCLK->GENCTRL.reg=GCLK_GENCTRL_ID(GCLK_CLKCTRL_GEN_GCLK0_Val) | GCLK_GENCTRL_SRC (GCLK_SOURCE_DFLL48M) | GCLK_GENCTRL_RUNSTDBY | GCLK_GENCTRL_GENEN;
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) { ; }
}
