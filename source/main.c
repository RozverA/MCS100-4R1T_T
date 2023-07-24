#include "def.h"

WORD size=0;

WORD reset;

int main(void)
{
	SCB->VTOR=0x00004000;
	
	if (PROC_HERZ == PROC_HERZ8)
	{sys_clock_init_USE_OSC8M();}   //8MHz
	else
	{sys_clock_init_USE_DFLL48M();}	//48MHz
		
	__enable_irq();
	
	wdt_start();
	
	
	cfg_init  ();
	
	if(cfg_read() == CFG_ERR)
	{
		cfg_default();
	}
		
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

void debug_funx(BYTE mode)
{
	BYTE buf[] = {0,1,2,3,4,5,6,7,8,9,10};
	switch(mode)
	{
		case ANSWER:
		size=usart_0_read(buf,256);
		if(size){usart_0_write(buf,size);size=0;}
		
		size=usart_1_read(buf,256);
		if(size){usart_1_write(buf,size);size=0;}
		
		size=usart_2_read(buf,256);
		if(size){usart_2_write(buf,size);size=0;}
		
		size=usart_3_read(buf,256);
		if(size){usart_3_write(buf,size);size=0;}
		
		// 			size=usart_4_read(buf,256);
		// 			if(size){usart_4_write(buf,size);size=0;}
		break;
		case SPAM:
		usart_0_write(buf,8);
		wait(TC3_1m);
		usart_1_write(buf,size);
		usart_2_write(buf,size);
		usart_3_write(buf,size);
		//usart_4_write(buf,size); (only for msc100-5r)
		break;
		// 		case BIP:(only for msc100-5r)
		// 			if (eth_wait > led_pwr_timer) {bip_block = 0;}
		// 			if (!bip_block)
		// 			{
		// 				LED_PWR_TGL();
		// 				led_pwr_timer = eth_wait;
		// 				bip_block = 1;
		// 				if (led_pwr_timer > (0xFFFFFFFF - up)) {led_pwr_timer = up;}
		// 				else {led_pwr_timer = led_pwr_timer + (up/2);}
		// 			}
		// 		break;
		case LED_BIP:
		LED_TX0_TGL();
		wait(TC3_100mk);
		break;
		case NO:
		for (int i = 0 ; i<1;i++){}
		break;
		case SPI_DOLBIT:
		if(eth_wait <= 0x3E8)
		{spi_write(0x00,0x14,buf,10);}
		else {int i =0;}
		usart_0_write(buf,10);
		usart_1_write(buf,10);
		usart_2_write(buf,10);
		usart_3_write(buf,10);

		break;
		case DMA_TEST:
		
		break;
	}
}