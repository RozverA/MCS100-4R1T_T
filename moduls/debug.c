#include "def.h"

WORD low_timer = 0;

void stat_operator()
{
	if (low_timer < eth_wait) {timer_set(&low_timer);}
}

void debug_funx(BYTE mode)
{
	BYTE buf[] = {0,1,2,3,4,5,6,7,8,9,10};
	WORD size=0;
	switch(mode)
	{
		case SPAM:
			wait(TC3_1m);
		break;
		case LED_BIP:
			pin_ctrl(LED_TX, 0, TGL);
			wait(TC3_100mk);
		break;
		case NO:
			return;
		break;
		case SPI_DOLBIT:
			if(eth_wait <= 0x3E8){spi_write(0x00,0x14,buf,10);}
			else {int i =0;}
		break;
		case DMA_TEST:
			if (MODUL_DMA == ON)	{dma_init();}
		break;
	}
}