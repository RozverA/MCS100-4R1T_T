#include "def.h"

ERRORS err_dword;
WORD reset;

int main(void)
{
	SCB->VTOR=0x00004000;
	
	wdt_reset();
	
	if (PROC_HERZ == 8000000)	{sys_clock_init_USE_OSC8M();}   //8MHz
	else						{sys_clock_init_USE_DFLL48M();}	//48MHz
		
	__enable_irq();

	gpio_init();
	pin_ctrl(LED,PWR,ON);
	TC3_init();
	usart_init();
	
	while (1)
	{
		cmd_process();
		tc3_process();
		wdt_reset();
	}
}