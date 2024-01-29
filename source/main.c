#include "def.h"

ERRORS err_dword;
WORD reset;
ACCOUNTS accnts;
DWORD crc_fw;
BYTE deb = 0;
int main(void)
{
	SCB->VTOR=0x00004000;
	
	if (PROC_HERZ == (8 * 1000000))	{sys_clock_init_USE_OSC8M();}   //8MHz
	else							{sys_clock_init_USE_DFLL48M();}	//48MHz
		
	__enable_irq();
	wdt_reset();
	wdt_start();	
			
	cfg_init  ();
	if(cfg_2_read() == CFG_ERR) {cfg_2_err = CFG_ERR; err_dword.cfg_2_init = 1; }
	if(cfg_read()   == CFG_ERR)	{err_dword.cfg_2_init = 1; cfg_default();}
	cfg_check();

	gpio_init();
	TC3_init();
	spi_init();
	usart_init();
	eth_init();
	
	acc(READ);
	log_ch();
	log_safe(0, 0x00000000, 0x00000000, START);
	
	crc_fw = crc32(0x00004000, 0x00014000);
	
	led_init();
		
	
/*DEBUG*/
	if (sizeof(CFG_1) != 0x100 ) { warning_led(1); DWORD ERR1 = sizeof(CFG_1);}
	if (sizeof(CFG_2) != 0x100 ) { warning_led(2); DWORD ERR2 = sizeof(CFG_2);}
/*DEBUG*/				
	while (1)
	{
	    if(reset == NULL)
		 {wdt_reset();}//wdt-drop timer
		eth_process();
		cmd_process();
		tc3_process();
	}
}
