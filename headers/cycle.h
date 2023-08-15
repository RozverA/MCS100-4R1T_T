#ifndef CYCLE_H_
#define CYCLE_H_

#define UCMD_CH 0
#define UCMD_RS485_ETH 1
#define UCMD_ETH_RS485 2

#pragma pack(1)
typedef struct
{
	WORD last_ptr_rx_buf[4];
	BYTE stage[4];
	DWORD time[4];
}VAR;
#pragma pack()

extern VAR vars;

#endif /* CYCLE_H_ */