#ifndef CMD_H_
#define CMD_H_


#define size_data_sock(r) ((port_udp[r].len[0]<<8) | (port_udp[r].len[1]))



extern void cmd_process(void);
extern void cmd_common_process (void)
extern void cmd_usart_process (void);
#endif /* CMD_H_ */