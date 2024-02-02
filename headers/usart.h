#ifndef USART_H_
#define USART_H_

//define
	//usart
		#define USART_BUF_SIZE 256
	//parity
		#define PARITY_NONE 0
		#define PARITY_EVEN 2
		#define PARITY_ODD  1
	//dop parity
		#define EVEN	0
		#define ODD		1
	//frame
		#define FRAME_NO_PARITY 0
		#define FRAME_WITH_PARITY 1
	//rs485 mod
		#define RS485_WRITE 0
		#define RS485_READ  1
//typedef
	//usart statistic
		#pragma pack(1)
		typedef struct
		//-----------------------------------------------------------------------------
		{
			WORD	collision;
			WORD	synchr;
			WORD	buf_ovf;
			WORD	st_bit;
			WORD	prty;
		}USART_STAT;
		#pragma pack()
	//counter
		#pragma pack(1)
		typedef struct
		//-----------------------------------------------------------------------------
		{
			DWORD	tx;
			DWORD	rx;
			WORD	crc_err;
		}POIN;
		#pragma pack()
	//USART struct
		typedef struct
		//-----------------------------------------------------------------------------
		{
			Sercom*	sercom;
			DWORD	gclk_sercom;
			BYTE	irqn_sercom;
	
			BYTE	wbuf[USART_BUF_SIZE];
			BYTE	rbuf[USART_BUF_SIZE];
	
			WORD	wn;
			WORD	wx;
			WORD	rn;	
	
			WORD	rtout;
			WORD	wtout;
			WORD	rtime;
			WORD	wtime;
	
			WORD	tout_port;
			WORD	dt;//таймаут - оставшееся время (Время передачи - время приема = дельта время)
	
			BYTE	stage;
			DWORD	time_port;
	
			WORD	dre;
			WORD	rxc;
			WORD	txc;
	
			USART_STAT errors;
	
			POIN	counters;
		}USART;
//extern
	//struct
		extern volatile USART port[4];
	//funx
		extern void usart_init(void);
		extern WORD usart_write(BYTE n_port, BYTE* wbuf,WORD size);
		extern WORD usart_read (BYTE n_port, BYTE* rbuf,WORD size);

#endif /* USART_H_ */