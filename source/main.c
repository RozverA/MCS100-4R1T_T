#include "def.h"

ERRORS err_dword;
WORD reset;

int main(void)
{
	SCB->VTOR=0x00004000;
	
	if (PROC_HERZ == 8000000)	{sys_clock_init_USE_OSC8M();}   //8MHz
	else						{sys_clock_init_USE_DFLL48M();}	//48MHz
	
	__enable_irq();
	wdt_reset();
	wdt_start();
			
	cfg_init  ();
	if(cfg_2_read() == CFG_ERR) {cfg_2_err = CFG_ERR; err_dword.cfg_2_init = 1;}
	if(cfg_read()   == CFG_ERR)	{err_dword.cfg_2_init = 1; cfg_default();}
	
	cfg_check();
	
	gpio_init();
	pin_ctrl(LED,PWR,ON);
	TC3_init();
	spi_init();
	usart_init();
	eth_init();

	warning();
	
	while (1)
	{
	    if(reset == NULL) {wdt_reset();}//wdt-drop timer
		eth_process();
		cmd_process();
		tc3_process();
	}
}