#ifndef CMD_H_
#define CMD_H_

#define MBAP_HDR_LEN 6
#define size_data_sock(r) ((port_udp[r].len[0]<<8) | (port_udp[r].len[1]))

typedef struct{
 	WORD 	transaction_n;
	WORD 	protocol;
	WORD	len;
}MBAP_HDR;

extern MBAP_HDR hdr_struct;
extern WORD prs(BYTE* mess,WORD size);
extern WORD add_hdr(BYTE* buf, WORD mess_size);

extern void cmd_process(void);
extern void cmd_common_process (void);
extern void cmd_usart_process (void);

#endif /* CMD_H_ */