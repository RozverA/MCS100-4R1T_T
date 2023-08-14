#ifndef USART_H_
#define USART_H_

#define USART_BUF_SIZE 256

#define PARITY_NONE 0
#define PARITY_EVEN 1
#define PARITY_ODD  2

#define FRAME_NO_PARITY 0
#define FRAME_WITH_PARITY 1

typedef struct
//-----------------------------------------------------------------------------
{
	Sercom* sercom;
	DWORD gclk_sercom;
	BYTE irqn_sercom;
	
	
	
	unsigned char  wbuf[USART_BUF_SIZE];
	unsigned char  rbuf[USART_BUF_SIZE];
	
	unsigned short wn;
	unsigned short wx;
	unsigned short rn;
	unsigned short rx;
	
	
	unsigned short rtout;
	unsigned short wtout;
	unsigned short rtime;
	unsigned short wtime;
	
	unsigned short dre;
	unsigned short rxc;
	unsigned short txc;
}USART;

extern volatile USART port[4];


extern void usart_init();

extern WORD usart_write(BYTE num, BYTE* wbuf,WORD wn);
extern WORD usart_read(BYTE num, BYTE* rbuf,WORD wn);

#endif /* USART_H_ */