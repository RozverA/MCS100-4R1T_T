#ifndef CYCLE_H_
#define CYCLE_H_

#define RS485_CHECK 0
#define RS485_READ 1
#define RS485_WRITE 2

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