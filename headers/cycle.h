#ifndef CYCLE_H_
#define CYCLE_H_

#define UCMD_CH 0
#define UCMD_RS485_ETH 1
#define UCMD_ETH_RS485 2
#define UCMD_TLN 3

#define TIMER_COEF_8 0x01F4 //500
#define TIMER_COEF_48 0x02EE //750
#define TIMER_COEF 0x01F4 //500

#define TIMER_LMT (0xFFFF - (TIMER_COEF * 1.2))// лимит - считается как предел wordа - (размер коэф * множитель для малой вероятности)

#endif /* CYCLE_H_ */