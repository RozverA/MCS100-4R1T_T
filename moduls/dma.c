#include "def.h"
#include "moduls.h"

Descriptor DESCR;

void dma_init(DWORD* test_ptr1, DWORD* test_ptr2, DWORD* test_ptr3)
{

	deb_descriptor_setup(test_ptr1, test_ptr2);
	deb_dma_setup(test_ptr3);
	
	//if this register if disable
		//we can change value this register
	
// 	DMAC->CTRL.bit.DMAENABLE = 0;
// 		DMAC->BASEADDR
// 		DMAC->WRBADDR
// 	DMAC->CTRL.bit.CRCENABLE = 0;//und DMAC->CTRL.bit.DMAENABLE = 0;
// 		DMAC->CTRL.bit.SWRST = 0;//no reset
// 	DMAC->CHCTRLA.bit.ENABLE = 0;//chanel DMA
// 		DMAC->CHCTRLB.bit.EVACT		//
// 		DMAC->CHCTRLB.bit.EVIE		//
// 		DMAC->CHCTRLB.bit.EVOE		//0 - no generation event, 1
// 		DMAC->CHCTRLB.bit.TRIGACT	//trigger setup
// 		DMAC->CHCTRLB.bit.TRIGSRC	//trigger status?
	//can change
	//DMAC->CHCTRLB.bit.CMD // 1 -pause 2-continue
		//DMAC->CHCTRLB.bit.LVL	//arbitrage level (prioriti 0-hight 3-minimal)
	//if the channel you want to configure is turned off
// 		DMAC->CHCTRLA.bit.SWRST = 0;
// 		DMAC->CTRL.bit.CRCENABLE = 0;
// 		DMAC->CRCCTRL.bit.CRCBEATSIZE = 0;
// 		DMAC->CRCCTRL.bit.CRCPOLY = 0;
// 		DMAC->CRCCTRL.bit.CRCSRC = 0;
}

void deb_dma_setup(DWORD* test_ptr3)
{
	//setup DMA before enable
		DMAC->BASEADDR.bit.BASEADDR		= &DESCR;
		DMAC->WRBADDR.bit.WRBADDR		= test_ptr3;
		DMAC->CTRL.bit.LVLEN0			= 1;
			DMAC->CTRL.bit.DMAENABLE	= 1;
	//DMA channel setup before enable
		DMAC->CHID.bit.ID				= 0x00;    //chanel number
		DMAC->CHCTRLB.bit.TRIGACT		= 0x03; //тригерное действие 3 - транзакция
		DMAC->CHCTRLB.bit.TRIGSRC		= 0x0B; //source triger (источник) - 0 disable trigger
			DMAC->CHCTRLA.bit.ENABLE	= 1;
}

void deb_descriptor_setup(DWORD* test_ptr1,DWORD* test_ptr2)
{
	//Descriptor setup
	DESCR.BTCTRL.bit.BEATSIZE	= 0; //0 - 8b (BYTE), 1 - 16b(WORD), 2 - 32(DWORD); размер передачи
	DESCR.BTCTRL.bit.VALID		= 1; //0 - noValid, 1 - Valid
	DESCR.BTCNT.bit.BTCNT		= 0; //счетчик отправленных блоков
	DESCR.SRCADDR.bit.SRCADDR	= test_ptr1; //source addr
	DESCR.DSTADDR.bit.DSTADDR	= test_ptr2; //dist addr
	//DESCR.DESCADDR				= 0; //next descriptor addr
}

void desc(BYTE size_block, BYTE valid, DWORD SRC, DWORD DST)
{
	DESCR.BTCTRL.bit.BEATSIZE	= size_block; //0 - 8b (BYTE), 1 - 16b(WORD), 2 - 32(DWORD); размер передачи
	DESCR.BTCTRL.bit.VALID		= valid; //0 - noValid, 1 - Valid
	//DESCR.BTCNT.bit.BTCNT		= 0; //счетчик отправленных блоков
	DESCR.SRCADDR.bit.SRCADDR	= SRC; //source addr
	DESCR.DSTADDR.bit.DSTADDR	= DST; //dist addr
}