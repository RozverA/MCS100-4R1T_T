#ifndef USART_H_
#define USART_H_

#define USART_BUF_SIZE 256

#define PARITY_NONE 0
#define PARITY_EVEN 1
#define PARITY_ODD  2

#define FRAME_NO_PARITY 0
#define FRAME_WITH_PARITY 1

#pragma pack(1)
typedef struct
//-----------------------------------------------------------------------------
{
	DWORD	tx;
	DWORD	rx;
	WORD	crc_err;
}POIN;
#pragma pack()

typedef struct
//-----------------------------------------------------------------------------
{
	Sercom* sercom;
	DWORD gclk_sercom;
	BYTE irqn_sercom;
	
	BYTE  wbuf[USART_BUF_SIZE];
	BYTE  rbuf[USART_BUF_SIZE];
	
	WORD wn;
	WORD wx;
	WORD rn;
	WORD rx;
	
	
	WORD rtout;
	WORD wtout;
	WORD rtime;
	WORD wtime;
	
	WORD tout_port;
	WORD dt;
	
	WORD dre;
	WORD rxc;
	WORD txc;
	
	POIN counters;
}USART;

extern volatile USART port[4];


extern void usart_init(void);

extern WORD usart_write(BYTE num, BYTE* wbuf,WORD wn);
extern WORD usart_read(BYTE num, BYTE* rbuf,WORD wn);

#endif /* USART_H_ */