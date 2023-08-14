#include "def.h"

ERR er_list;

WORD low_timer = 0;

void stat_operator()
{
	if (low_timer < eth_wait) {timer_upd(&low_timer);}
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

void chip_stat()
{
	if (NVMCTRL->STATUS.bit.NVME)		{er_list.nvwc.NVWC_err++;	/*NVMCTRL->STATUS.bit.NVME = 1;*/}
	if (NVMCTRL->STATUS.bit.LOCKE)		{er_list.nvwc.lock_area++;	/*NVMCTRL->STATUS.bit.LOCKE = 1;*/}
	if (NVMCTRL->STATUS.bit.PROGE)		{er_list.nvwc.prog_err++;	/*NVMCTRL->STATUS.bit.PROGE = 1;*/}
	if (NVMCTRL->STATUS.bit.LOAD)		{er_list.nvwc.load_buf++;	/*NVMCTRL.STATUS.bit.LOAD = 1;*/}
	if (NVMCTRL->STATUS.bit.PRM)		{er_list.nvwc.PRM++;		/*NVMCTRL.STATUS.bit.PRM = 1;*/}
}