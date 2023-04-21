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


extern void usart_init(void);

extern void usart_0_init(void);
extern WORD usart_0_write(BYTE* wbuf,WORD wn);
extern WORD usart_0_read(BYTE* rbuf,WORD wn);

extern void usart_1_init(void);
extern WORD usart_1_write(BYTE* wbuf,WORD wn);
extern WORD usart_1_read(BYTE* rbuf,WORD wn);

extern void usart_2_init(void);
extern WORD usart_2_write(BYTE* wbuf,WORD wn);
extern WORD usart_2_read(BYTE* rbuf,WORD wn);

extern void usart_3_init(void);
extern WORD usart_3_write(BYTE* wbuf,WORD wn);
extern WORD usart_3_read(BYTE* rbuf,WORD wn);

#endif /* USART_H_ */