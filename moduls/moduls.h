#ifndef MODULS_H_
#define MODULS_H_

#define ON				1
#define OFF				0

#define MODUL_DMA		OFF
#define MODUL_TELNET	OFF
#define MODUL_DEBUG		OFF

#pragma pack(1)
typedef struct
{
	WORD last_ptr_rx_buf[5];
	BYTE port_stat[5];
	WORD port_time[5];
}VAR;
#pragma pack()

extern VAR vars;

#endif /* MODULS_H_ */