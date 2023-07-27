#ifndef UDP_H_
#define UDP_H_

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
	
	POINT counters;
}ETH_HDR;
#pragma pack()

extern ETH_HDR eth_sock[MAX_SOCKETS];

#endif /* UDP_H_ */