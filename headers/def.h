#ifndef DEF_H_
#define DEF_H_
#define wdt_start() { WDT->CONFIG.bit.PER=0x0B;/*500 ms*/ WDT->EWCTRL.bit.EWOFFSET=0x0A;/*250 ms*/ WDT->INTENSET.bit.EW=0x01;/*Early Warning Interrupt Enable*/ WDT->CTRL.bit.ENABLE=0x01; }
#define wdt_reset() { if(WDT->INTFLAG.bit.EW==1){WDT->INTFLAG.bit.EW=0x01;  WDT->CLEAR.bit.CLEAR=0xA5;} }
#define wdt_stop()  { WDT->CTRL.bit.ENABLE=0x00; WDT->INTENCLR.bit.EW=0x01; } 		
	
#define NULL  0
#define TRUE  1
#define FALSE 0

#define SKIPP_HADER 8 //TCP connect reciv without UDP HEADER

typedef enum
{
	SUCCESS = 0,  //0
	TIMEOUT,      //1
	ERROR         //2
} func_st;


typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned int   DWORD;

#include <string.h>
#include <stdlib.h>

//#define PROC_HERZ48		48000000
#define PROC_HERZ8		8000000

#ifdef PROC_HERZ8
	#define PROC_HERZ  PROC_HERZ8
	#define TIMER_COEF 0x01F4 //500
#endif

#ifdef PROC_HERZ48
	#define PROC_HERZ  PROC_HERZ48
	#define TIMER_COEF 0x02EE //750
#endif

#define TIMER_LMT (0xFFFF - (TIMER_COEF * 1.2))// лимит - считается как: <<предел wordа - (размер коэф * множитель для малой вероятности)>>

#define ANSWER	0
#define SPAM	1
#define LED_BIP	3
#define NO		200
#define BIP		2
#define SPI_DOLBIT 4
#define DMA_TEST	5

#include "samr21g18a.h"
#include "gpio.h"
#include "usart.h"
#include "sys_clock.h"
#include "timer.h"
#include "flash.h"
#include "iap.h"
#include "crc.h"
#include "cfg.h"
#include "spi.h"
#include "w5500_reg.h"
#include "eth_parse.h"
#include "socket.h"
#include "udp.h"
#include "cycle.h"
#include "moduls.h"
#include "dma.h"
#include "debug.h"

extern WORD reset;

#endif /* DEF_H_ */