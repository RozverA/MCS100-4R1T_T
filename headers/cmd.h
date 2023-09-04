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
	BYTE w5500_init:	1;
	BYTE cfg_1_init:	1;
	BYTE cfg_2_init:	1;
	BYTE rsv:			1;
	BYTE socket:		4;
	BYTE ip_init_er:	1;
	BYTE mask_init_er:	1;
	BYTE gw_init_er:	1;
	BYTE mac_init_er:	1;
	//12
} ERRORS;

extern void cmd_process(void);
extern void cmd_common_process (void);
extern void cmd_usart_process (void);

#endif /* CMD_H_ */