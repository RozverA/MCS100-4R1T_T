#ifndef CYCLE_H_
#define CYCLE_H_

#define UCMD_CH 0
#define UCMD_UP 1
#define UCMD_DWN 2

//#define TIMER_COEF 0x00FA //250
#define TIMER_COEF 0x02EE //750
//#define TIMER_COEF 0x01F4 //500
//#define TIMER_COEF 0x03E8 //1000
#define TIMER_LMT (0xFFFF - (TIMER_COEF * 1.2))// лимит - считается как предел wordа - (размер коэф * множитель для малой вероятности)

#endif /* CYCLE_H_ */