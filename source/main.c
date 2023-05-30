#include "def.h"

WORD size=0;

WORD reset;

int main(void)
{
	SCB->VTOR=0x00004000;
	
    sys_clock_init_USE_DFLL48M();
	//das fur telnet
	__enable_irq();
	
	wdt_start();
	
	
	cfg_init  ();
	
	
	
	if(cfg_read() == CFG_ERR)
	{
		cfg_default();
	}
	
	//cfg_drop();
	
	gpio_init();
	TC3_init();
	spi_init();

	usart_init();
	eth_init();
	
	LED_TX0_ON();
	LED_RX0_ON();
	LED_TX1_ON();
	LED_RX1_ON();
	LED_TX2_ON();
	LED_RX2_ON();
	LED_TX3_ON();
	LED_RX3_ON();
	
	wait(time_500ms);
	
	LED_TX0_OFF();
	LED_RX0_OFF();
	LED_TX1_OFF();
	LED_RX1_OFF();
	LED_TX2_OFF();
	LED_RX2_OFF();
	LED_TX3_OFF();
	LED_RX3_OFF();	
	size=sizeof(CFG);
	while (1)
	{
	    if(reset == NULL) {wdt_reset();}//wdt-drop timer
		
		eth_process();
		cmd_process();
	}
}