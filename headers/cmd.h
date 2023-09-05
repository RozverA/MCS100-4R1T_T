#ifndef CMD_H_
#define CMD_H_

#define MBAP_HDR_LEN 6
#define size_data_sock(r) ((port_udp[r].len[0]<<8) | (port_udp[r].len[1]))

typedef struct{
 	BYTE 	transaction_n[2];
	BYTE 	protocol[2];
	BYTE	len[2];
}MBAP_HDR;
typedef struct
{
	DWORD rsv:			1;
	DWORD cfg_1_init:	1;
	DWORD cfg_2_init:	1;
	DWORD common_sock:	1;
	DWORD socket:		4;
	DWORD ip_init_er:	1;
	DWORD mask_init_er:	1;
	DWORD gw_init_er:	1;
	DWORD mac_init_er:	1;
	//12bit
} ERRORS;

extern ERRORS err_dword;

extern void cmd_process(void);
extern void cmd_common_process (void);
extern void cmd_usart_process (void);

#endif /* CMD_H_ */