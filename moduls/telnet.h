#ifndef TELNET_H_
#define TELNET_H_

#define SPACE_BAR 0x20

#pragma pack(1)
typedef struct
//-----------------------------------------------------------------------------
{
	WORD last_rx_ptr;
	WORD act_rx_ptr;
	WORD last_tx_ptr;
	WORD act_tx_ptr;
	BYTE buf[128];
	BYTE cnt;
	BYTE len;
	BYTE len_mes;
	BYTE mode;
}TELNET;
#pragma pack()

extern TELNET tel;

#define TEL_SOCK 5
#define TELNET_PORT 23

#define NAME			1
#define IP				1

#define WRONG_OR_HELP	0x00//0

#define PARS_MENU		0x1f//31
#define PARSE_IP		0x20//32

#define TEL_SHOW		0x3D//61//0	
#define SHOW_NAME		TEL_SHOW + NAME//0x3E//62//1	
#define SHOW_IP			TEL_SHOW + IP//0x3F//63//2

#define TEL_CFG			0x97//151//0
#define CONFIG_IP		TEL_CFG + IP//0x99//153//2

BYTE tel_parser(void);
void symbol_cut(BYTE symbol);
void pars_menu();
void de_caps();
void symbol_analysis_cmd(BYTE cnt);
void symbol_analysis_val(BYTE cnt);
void tell_funx(BYTE n_port, BYTE* port_stat);
void tel_in_usart_proccess(BYTE n_port, BYTE* status, BYTE* timer);

#define MAX_LEN_CMD_TEL 6

#endif /* TELNET_H_ */