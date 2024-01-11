#include "def.h"

ERRORS err_dword;
WORD reset;
ACCOUNTS accnts;
DWORD ERR1, ERR2;

int main(void)
{
	SCB->VTOR=0x00004000;
	
	if (PROC_HERZ == (8 * 1000000))	{sys_clock_init_USE_OSC8M();}   //8MHz
	else							{sys_clock_init_USE_DFLL48M();}	//48MHz
		
	__enable_irq();
	wdt_reset();
	wdt_start();
		ERR1 = sizeof(CFG_1);
		ERR2 = sizeof(CFG_2);
		if (sizeof(CFG_1) != 0x100 ) { ERR1 = sizeof(CFG_1);}
		if (sizeof(CFG_2) != 0x100 ) { ERR2 = sizeof(CFG_2);}	
			
	cfg_init  ();
	if(cfg_2_read() == CFG_ERR) {cfg_2_err = CFG_ERR; err_dword.cfg_2_init = 1; }
	if(cfg_read()   == CFG_ERR)	{err_dword.cfg_2_init = 1; cfg_default(); acc(DROP);}
	cfg_check();

	gpio_init();
	TC3_init();
	spi_init();
	usart_init();
	eth_init();
	
	acc(READ);
	
	led_init();
		if (ERR1)	{warning_led(1);}
		if (ERR2)	{warning_led(2);}
	log_drop();
	acc(DROP);
	//cfg_drop();
				
	while (1)
	{
	    if(reset == NULL) {wdt_reset();}//wdt-drop timer
		eth_process();
		cmd_process();
		tc3_process();
	}
}
