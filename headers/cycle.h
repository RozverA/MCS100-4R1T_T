#ifndef CYCLE_H_
#define CYCLE_H_

#define UCMD_CH 0
#define UCMD_RS485_ETH 1
#define UCMD_ETH_RS485 2


#define TIMER_LMT (0xFFFF - (TIMER_COEF * 1.2))// лимит - считается как предел wordа - (размер коэф * множитель для малой вероятности)

#pragma pack(1)
typedef struct
{
	WORD last_ptr_rx_buf[5];
	BYTE port_stat[5];
	WORD port_time[5];
}VAR;
#pragma pack()

extern VAR vars;

#endif /* CYCLE_H_ */