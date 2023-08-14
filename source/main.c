#include "def.h"


WORD reset;

int main(void)
{
	SCB->VTOR=0x00004000;
	
	if (PROC_HERZ == 8000000)		{sys_clock_init_USE_OSC8M();}   //8MHz
	else							{sys_clock_init_USE_DFLL48M();}	//48MHz
		
	__enable_irq();
	wdt_start();
		
	cfg_init  ();
	if(cfg_read() == CFG_ERR)		{cfg_default();}
	cfg_check();
		
	gpio_init();
	TC3_init();
	spi_init();
	usart_init();
	eth_init();
	led_init();
	
	while (1)
	{
	    if(reset == NULL) {wdt_reset();}//wdt-drop timer
		eth_process();
		cmd_process();
		//if (MODUL_DEBUG){debug_funx(NO);}
	}
}