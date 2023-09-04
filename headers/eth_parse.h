#ifndef ETH_PARSE_H_
#define ETH_PARSE_H_

#include "moduls.h"

#define NULLS					0
#define SPI_PROCESS				1
#define MODE_OP_READ_UDP		2
#define MODE_OP_WRITE_UDP		3
#define MODE_OP_READ_TCP		4
#define MODE_OP_WRITE_TCP		5

#define MODE_OP_GIVE_STAT		7

#define LEN_HDR					8
#define MAX_SOCKETS				5
#define DEFAULT_MTU_UDP			1480 //1472(MTU) + ip + port + data
#define DEFAULT_MTU_TCP			1460


extern void eth_init(void);
extern void eth_process(void);
extern BYTE check_sockets_process (BYTE *buf);
extern BYTE check_data_wr_process (BYTE *data_buf);
extern void eth_parse (BYTE numb_sock,BYTE *buf,WORD size);
extern BYTE* eth_cbuf_ptr();

extern func_st socket_init (BYTE numb, WORD src_port, BYTE mode);

#define UDP		1
#define TCP		2

#define MBUS	2
#define GATE	1

/////socket_read_udp
#define UDP_GIVE_LEN		0
#define UDP_PART_RD			1
#define UDP_PTR_MOVE		2
#define UDP_RCV_CMD			3
#define UDP_BK_START		4
/////socket_read_tcp
#define TCP_GIVE_LEN		0
#define TCP_PART_RD			1
#define TCP_RSV_CMD			2
#define TCP_DROP_PTR		3
#define TCP_BK_START		4//
#define TCP_STATUS_RD		5//
#define TCP_FORK			6
/////socket_write_tcp
#define TCP_RD_FSR			0
#define TCP_WR_DATA			1
#define TCP_PTR_WR			2
#define TCP_SEND_CMD		3
#define TCP_STAT_RD			4
#define TCP_STATUS_CLR		5
#define TCP_BK_TO_START		6
/////socket_write_udp
#define UDP_RD_TX_STAT		0
#define UDP_WR_IP			1
#define UDP_WR_PORT			2
#define UDP_WR_DATA			3
#define UDP_WR_PTR			4
#define UDP_SEND_CMD		5
#define UDP_STAT_RD			6
#define UDP_STATUS_CLR		7
#define UDP_BK_TO_START		8


/////socket_status_tcp
#define STATUS_LISTEN 0x14
#define STATUS_CLOSE 0x00
#define STATUS_INIT 0x013
#define STATUS_ESTABLISHED 0x17


#define SOCKET_0 0
#define SOCKET_1 1
#define SOCKET_2 2
#define SOCKET_3 3
#define SOCKET_4 4
#define SOCKET_5 5
#define SOCKET_6 6
#define SOCKET_7 7

#pragma pack(1)
typedef struct
//-----------------------------------------------------------------------------
{
	DWORD	tx;
	DWORD	rx;
	WORD	crc_err;
}POINT;
#pragma pack()

#pragma pack(1)
typedef struct
//-----------------------------------------------------------------------------
{
	BYTE  ip_addr[4];				//ip_addr
	BYTE  port[2];					//port
	BYTE  len[2];					//len data packet
	BYTE  data[MAX_SIZE_BUF_SPI];   //data
	BYTE  enable;					//socket_enable
	
	BYTE  r_status;					//rd status
	BYTE  w_status;					//wr status
	
	WORD ptr_rx_buf;				//ptr rd w5500
	WORD ptr_tx_buf;				//ptr wr w5500
	
	POINT counters;					//counters for count tx,rx and crc errors for port
	
	WORD check_connect_cnt;
	WORD time_wait_SEND_OK;
}ETH_HDR;
#pragma pack()

extern ETH_HDR eth_sock[MAX_SOCKETS];


#endif