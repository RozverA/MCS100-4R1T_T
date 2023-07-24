#ifndef TIMER_H_
#define TIMER_H_


#define TC3_100mk		50	//8H
//#define TC3_100mk		75	//48H

#define TC3_1m			(TC3_100mk * 10)
#define TC3_10m			(TC3_100mk * 100)
#define TC3_100m		(TC3_100mk * 1000)
#define TC3_1SEC_TICKS	(TC3_100mk * 10000)


static inline void TC3_start(WORD cc) { TC3->COUNT16.CC[0].reg=cc-1; TC3->COUNT16.CTRLBSET.bit.CMD=0x01; }
static inline void TC3_stop(void) { TC3->COUNT16.CTRLBSET.bit.CMD=0x02; }
static inline WORD TC3_cnt(void) { return(TC3->COUNT16.COUNT.bit.COUNT); }
static inline WORD TC3_mc0(void) { return(TC3->COUNT16.INTFLAG.bit.MC0); }


extern void TC3_init(void);
extern void wait(DWORD time_100mk);


#define time_500ms 5000
#define time_50ms 100

extern volatile DWORD time_wait;
extern volatile WORD  spi_wait;
extern volatile WORD  cmd_spi_wait;
extern volatile WORD  eth_wait;

#endif /* TIMER_H_ */