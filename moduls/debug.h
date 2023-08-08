#ifndef DEBUG_H_
#define DEBUG_H_



#pragma pack(1)
typedef struct
//-----------------------------------------------------------------------------
{
	WORD	collision;
	WORD	synchr;
	BYTE	CTS;
	WORD	buf_ovf;
	WORD	st_bit;
	WORD	prty;
}USART_STAT;
#pragma pack()

#pragma pack(1)
typedef struct
//-----------------------------------------------------------------------------
{
	BYTE	secur_b;
	WORD	NVWC_err;
	WORD	lock_area;
	WORD	prog_err;
	BYTE	load_buf;
	BYTE	PRM; //POWER REDUCTION MODE
}NVWC_STAT;
#pragma pack()

#pragma pack(1)
typedef struct
//-----------------------------------------------------------------------------
{
	USART_STAT	usart[4];
	NVWC_STAT	nvwc;
}ERR;
#pragma pack()

extern ERR er_list;

void chip_stat();

#endif /* DEBUG_H_ */