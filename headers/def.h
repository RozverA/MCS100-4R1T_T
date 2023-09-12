#ifndef DEF_H_
#define DEF_H_
#define wdt_start() {if(!(WDT->CTRL.bit.ENABLE)){WDT->CONFIG.bit.PER=0x0B;WDT->EWCTRL.bit.EWOFFSET=0x0A;WDT->INTENSET.bit.EW=0x01;WDT->CTRL.bit.ENABLE=0x01;}}
#define wdt_reset() { if(WDT->INTFLAG.bit.EW==1){WDT->INTFLAG.bit.EW=0x01;  WDT->CLEAR.bit.CLEAR=0xA5;} }
#define wdt_stop()  { WDT->CTRL.bit.ENABLE=0x00; WDT->INTENCLR.bit.EW=0x01; } 		
	
#define NULL  0
#define TRUE  1
#define FALSE 0

#define SKIP_HDR 8 //TCP connect reciv without UDP HEADER

#define PROC_WAIT	0
#define PROC_OK		1
#define PROC_ER		2


typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned int   DWORD;

#include <string.h>
#include <stdlib.h>

//#define PROC_HERZ48		48000000
#define PROC_HERZ8		8000000

#ifdef PROC_HERZ8
	#define PROC_HERZ  PROC_HERZ8
#endif

#ifdef PROC_HERZ48
	#define PROC_HERZ  PROC_HERZ48
#endif

#define ANSWER	0
#define SPAM	1
#define LED_BIP	3
#define NO		200
#define BIP		2
#define SPI_DOLBIT 4
#define DMA_TEST	5
#define SIZE_HISTORY 6

#include "samr21g18a.h"
#include "gpio.h"
#include "usart.h"
#include "sys_clock.h"
#include "timer.h"
#include "flash.h"
#include "iap.h"
#include "cfg.h"
#include "spi.h"
#include "w5500_reg.h"
#include "eth_parse.h"
#include "cmd.h"
#include "ssh.h"

extern WORD reset;

extern WORD crc16_ccit(BYTE *buf,WORD size);
extern WORD crc16_mbus( BYTE *buf, WORD size );

#endif /* DEF_H_ */