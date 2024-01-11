#ifndef DEF_H_
#define DEF_H_
//typedefs
	typedef unsigned char  BYTE;
	typedef unsigned short WORD;
	typedef unsigned int   DWORD;
//includes
////includes glob
	#include <string.h>
	#include <stdlib.h>
////includes local
	#include "samr21g18a.h"
	#include "spi.h"
	#include "eth_parse.h"
	#include "gpio.h"
	#include "usart.h"
	#include "sys_clock.h"
	#include "timer.h"
	#include "iap.h"
	#include "cfg.h"
	#include "w5500_reg.h"
	#include "cmd.h"
	#include "alg.h"
	#include "encod_k.h"
	#include "flash.h"
//define
////WDT
	#define wdt_start() { if (!(WDT->CTRL.bit.ENABLE))	{WDT->CONFIG.bit.PER=0x0B;	WDT->EWCTRL.bit.EWOFFSET=0x0A;	WDT->INTENSET.bit.EW=0x01;	WDT->CTRL.bit.ENABLE=0x01;}}
	#define wdt_reset() { if (WDT->INTFLAG.bit.EW==1)	{WDT->INTFLAG.bit.EW=0x01;  WDT->CLEAR.bit.CLEAR=0xA5;} }
	#define wdt_stop()  { WDT -> CTRL.bit.ENABLE=0x00;	WDT->INTENCLR.bit.EW=0x01; }
////bool
	#define NULL  0
	#define TRUE  1
	#define FALSE 0
////proc end status
	#define PROC_WAIT	0
	#define PROC_OK		1
	#define PROC_ER		2
////HERZ SETTINGS

//#define PROC_HERZ48		48000000
#define PROC_HERZ8			8000000

#ifdef PROC_HERZ8
	#define PROC_HERZ		PROC_HERZ8
	#define TC3_100mk		50
#endif

#ifdef PROC_HERZ48
	#define PROC_HERZ		PROC_HERZ48
	#define TC3_100mk		75
#endif

////returns status
	#define SUCCESS				0
	#define TIMEOUT				1
	#define ERROR				2
////proc
	#define READ				0
	#define WRITE				1
	#define DROP				2

//externs
////variables
	extern WORD reset;
	extern ACCOUNTS accnts;
////fun
//////crc
	extern WORD crc16_ccit(BYTE *buf,WORD size);
	extern WORD crc16_mbus( BYTE *buf, WORD size );
//////flash
	extern WORD flash_empty(DWORD addr,WORD size);
	extern void flash_erase_page(DWORD addr);
	extern void flash_write(DWORD addr,BYTE *buf,WORD size);
	extern WORD flash_copy(DWORD dst,DWORD src,DWORD size);
	extern WORD flash_read(DWORD addr,void* buf,WORD size);

#endif /* DEF_H_ */