#ifndef USART_H_
#define USART_H_

#define USART_BUF_SIZE 256

typedef struct
//-----------------------------------------------------------------------------
{

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


extern void usart_init(BYTE i);

extern void usart_init(BYTE num);
extern WORD usart_write(BYTE num, BYTE* wbuf,WORD wn);
extern WORD usart_read(BYTE num, BYTE* rbuf,WORD wn);

#endif /* USART_H_ */