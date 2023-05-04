#ifndef ETH_PARSE_H_
#define ETH_PARSE_H_

#define NULLS 0
#define SPI_PROCESS 1
#define MODE_OP_READ_UDP 2
#define MODE_OP_WRITE_UDP 3
#define MODE_OP_READ_TCP 4
#define MODE_OP_WRITE_TCP 5

#define LEN_HDR 8
#define MAX_SOCKETS 5
#define DEFAULT_MTU_UDP 1480 //1472(MTU) + ip + port + data
#define DEFAULT_MTU_TCP 1460


extern void eth_init(void);
extern void eth_process(void);
extern void check_sockets_process (BYTE *buf);
extern BYTE check_data_wr_process (BYTE *data_buf);
extern void eth_udp_parse (BYTE numb_sock,BYTE *buf,WORD size);
extern BYTE* eth_cbuf_ptr();

/////socket_read_udp
#define RD_UDP_GIVE_LEN 0
#define RD_UDP_MES_PART_READ 1
#define RD_UDP_PTR_OFFSET 2
#define RD_UDP_RCV_CMD 3
#define RD_UDP_DROP_TO_START 4
/////socket_read_tcp
#define RD_TCP_GIVE_STATUS 0
#define RD_TCP_STATUS_FORK 1
#define RD_TCP_GIVE_LEN 2
#define RD_TCP_MES_PART_READ 3
#define RD_TCP_PTR_OFFSET 4
#define RD_TCP_GIVE_WR_PTR 5
#define RD_TCP_DROP_TO_START 6


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

typedef struct
{
DWORD check_sock_0;
DWORD check_sock_1;
DWORD check_sock_2;
DWORD check_sock_3;
DWORD check_sock_4;	
	
DWORD sock_parse_0;
DWORD sock_parse_1;
DWORD sock_parse_2;
DWORD sock_parse_3;
DWORD sock_parse_4;

}TEST;

extern TEST test;


#endif