#ifndef ETH_PARSE_H_
#define ETH_PARSE_H_

//moduls
	#define ssh_modul					1

//eth define
	//SPI 
		#define NULLS					0
		#define SPI_PROCESS				1
	//w5500 mode
		#define MODE_OP_READ_UDP		2
		#define MODE_OP_WRITE_UDP		3
		#define MODE_OP_READ_TCP		4
		#define MODE_OP_WRITE_TCP		5
	//ports
		#define ZERO_START				(-1)
		#define COMMON_SOCK_CNT			1
		#define COMMON_SOCK_VAL			0
		#define RS485_SOCK_CNT			4
		#define RS485_SOCK_VAL			4
		//#define MAX_SOCKETS				COMMON_SOCK_CNT + RS485_SOCK_CNT
		#define MAX_SOCKETS_CNT			COMMON_SOCK_CNT + RS485_SOCK_CNT
		#define MAX_SOCKETS_VAL			ZERO_START + COMMON_SOCK_CNT + RS485_SOCK_CNT
	//SSH --------------------------------------------------------------------------------------------MODUL
		#ifdef ssh_modul
			#define SSH_SOCK_CNT		1
			#define SSH_SOCK_VAL		5
			#define MAX_SOCKETS_VAL		ZERO_START + COMMON_SOCK_CNT + RS485_SOCK_CNT + SSH_SOCK_CNT
			#define MAX_SOCKETS_CNT		COMMON_SOCK_CNT + RS485_SOCK_CNT + SSH_SOCK_CNT
			#define SOCKET_5			5
			#define MBAP_HDR_LEN		6
		#endif
	//soket work mode
		#define UDP						1
		#define TCP						2
	//protocol
		#define MBUS					2
		#define GATE					1
	//MTU
		#define DEFAULT_MTU_UDP			1480 //1472(MTU) + ip + port + data
		#define DEFAULT_MTU_TCP			1460
	//stages w5500
		////socket_read_udp
			#define UDP_GIVE_LEN		0
			#define UDP_PART_RD			1
			#define UDP_PTR_MOVE		2
			#define UDP_RCV_CMD			3
			#define UDP_BK_START		4
		////socket_read_tcp
			#define TCP_GIVE_LEN		0
			#define TCP_PART_RD			1
			#define TCP_RSV_CMD			2
			#define TCP_DROP_PTR		3
			#define TCP_BK_START		4//
			#define TCP_STATUS_RD		5//
			#define TCP_FORK			6
		////socket_write_tcp
			#define TCP_RD_FSR			0
			#define TCP_WR_DATA			1
			#define TCP_PTR_WR			2
			#define TCP_SEND_CMD		3
			#define TCP_STAT_RD			4
			#define TCP_STATUS_CLR		5
			#define TCP_BK_TO_START		6
		////socket_write_udp
			#define UDP_RD_TX_STAT		0
			#define UDP_WR_IP			1
			#define UDP_WR_PORT			2
			#define UDP_WR_DATA			3
			#define UDP_WR_PTR			4
			#define UDP_SEND_CMD		5
			#define UDP_STAT_RD			6
			#define UDP_STATUS_CLR		7
			#define UDP_BK_TO_START		8
	//stage ethernet
		#define CHECK					0
		#define READ_PROCESS			1
		#define WRITE_PROCESS			2
		#define TCP_SOCK_PROCESS		3
	//numbers sockets
		#define NO_SOKET				109
		#define SOCKET_0				0
		#define SOCKET_1				1
		#define SOCKET_2				2
		#define SOCKET_3				3
		#define SOCKET_4				4
		//#define in SSH MODULE SOCKET	5
		//#define SOCKET_6				6
		//#define SOCKET_7				7
	//socket_status_tcp from w5500
		#define STATUS_LISTEN			0x14
		#define STATUS_CLOSE			0x00
		#define STATUS_INIT				0x13
		#define STATUS_ESTABLISHED		0x17
	//other
		#define SKIP_HDR				8 //TCP connect reciv without UDP HEADER
	//funx
		////inits
			extern void eth_init(void);
			extern BYTE socket_init (BYTE numb, WORD src_port, BYTE mode);
		////processes
			extern void eth_process(void);
			extern void check_sockets_process (BYTE *buf);
			extern BYTE check_data_wr_process (void);
		////other
			extern void eth_parse (BYTE numb_sock,WORD size);
		////toggle
			#define WR_TGL(sock)	{eth_sock[sock].w_status = 1;	eth_sock[sock].r_status = 0;}
	//type defs
		//struct
			#ifdef ssh_modul
				typedef struct{
					BYTE 	transaction_n[2];
					BYTE 	protocol[2];
					BYTE	len[2];
				}MBAP_HDR;
			#endif
		//___________________________________________//
			#pragma pack(1)
			typedef struct
			{
				DWORD	tx;
				DWORD	rx;
				WORD	crc_err;
			}POINT;
			#pragma pack()
		//___________________________________________//
			#pragma pack(1)
			typedef struct
			//-----------------------------------------------------------------------------
			{
				BYTE	ip_addr[4];				//ip_addr
				BYTE	port[2];					//port
				BYTE	len[2];					//len data packet
				BYTE	data[MAX_SIZE_BUF_SPI];   //data
				BYTE	enable;					//socket_enable
	
				BYTE	r_status;					//rd status
				BYTE	w_status;					//wr status
	
				WORD	ptr_rx_buf;				//ptr rd w5500
				WORD	ptr_tx_buf;				//ptr wr w5500
	
				POINT	counters;					//counters for count tx,rx and crc errors for port
	
				WORD	check_connect_cnt;
				WORD	time_wait_SEND_OK;
			}ETH_HDR;
			#pragma pack()
		//extern
			extern ETH_HDR eth_sock[MAX_SOCKETS_CNT];

#endif