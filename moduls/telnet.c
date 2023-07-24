#include "def.h"

TELNET tel;

void tell_funx(BYTE n_port, BYTE* port_stat)
{
	tel.len = u_port[TEL_SOCK].ptr_rx_buf - tel.last_rx_ptr;
			
	if (tel.len != 2)
	{
		tel.buf[tel.cnt] = u_port[TEL_SOCK].data[0];
		tel.cnt++ ;
		u_port[TEL_SOCK].data[0] = 0x00;
		u_port[TEL_SOCK].len[0] = (tel.len & 0xFF00) >> 8;	u_port[TEL_SOCK].len[1] = tel.len & 0x00FF; //write mess size in port_udp
	} 
			
	else 
	{
		BYTE rtn = tel_parser();
				
// 				tel.buf[tel.cnt] = u_port[TEL_SOCK].data[0]; tel.cnt++;
// 				tel.buf[tel.cnt] = u_port[TEL_SOCK].data[1]; tel.cnt++;
// 				
// 				memcpy (u_port[TEL_SOCK].data,  tel.buf,  tel.cnt);
// 				u_port[TEL_SOCK].len[0] = (tel.cnt & 0xFF00) >> 8;	u_port[TEL_SOCK].len[1] = tel.cnt & 0x00FF;
// 				
// 				memset(tel.buf,0x00,tel.cnt);
// 				tel.cnt = 0;
	}
	u_port[TEL_SOCK].w_status = 1;
	u_port[n_port].r_status = 0;
	*port_stat = UCMD_CH;//return
	tel.last_rx_ptr = u_port[TEL_SOCK].ptr_rx_buf;
}

BYTE tel_parser(void)
{
	//delete space symbol
	symbol_cut(SPACE_BAR);
	de_caps();
	switch(tel.mode)
	{
		case PARS_MENU:
		pars_menu();
		break;
		case PARSE_IP:
		
		break;
		case WRONG_OR_HELP:
		
		break;
	}
	
	return 0;
}

void pars_menu()
{
	pars();
}

void pars()
{
	symbol_analysis_cmd(1);//(val-1)
	symbol_analysis_val(2);//(val-1)
}

void de_caps()
{
	BYTE i = 0;
	while(i < tel.len)
	{
		if (0x41 < tel.buf[i] < 0x5A) {tel.buf[i] = tel.buf[i] + 0x20 ;}
	}
}

void symbol_analysis_cmd(BYTE cnt)
{
	BYTE command = 0;
	BYTE symbol = 0;
	char type_cmd[2][6];
	BYTE dat_cmd[2][2];
	dat_cmd[0][0] = 4;
	dat_cmd[0][1] = TEL_SHOW;
	dat_cmd[1][0] = 6;
	dat_cmd[1][1] = TEL_CFG;
	strcpy(type_cmd[0],"show");
	strcpy(type_cmd[1],"config");
	while (command < cnt)
	{
		while(symbol < MAX_LEN_CMD_TEL)
		{
			if (type_cmd[command][symbol] != tel.buf[symbol]) {break;}
			symbol++;
			if (symbol == dat_cmd[command][0]) 
			{
				tel.len_mes = symbol;
				tel.mode = dat_cmd[command][1];
				return;
			}
		}
	command++;
	}
}

void symbol_cut(BYTE symbol)
{
	BYTE i = 0, j = 0;
	while (i < tel.len)
	{
		if (tel.buf[i] == symbol)
		{
			j = i;
			while (j < (tel.len - 1))
			{
				tel.buf[j] = tel.buf[j + 1];
				j++;
			}
			tel.len--;
		}
		i++;
	}
}

void symbol_analysis_val(BYTE cnt)
{
	BYTE command = 0;
	BYTE symbol = 0;
	char type_cmd[2][6];
	BYTE dat_obj[2][2];
	dat_obj[0][0] = 4;
	dat_obj[0][1] = TEL_SHOW;
	dat_obj[1][0] = 6;
	dat_obj[1][1] = TEL_CFG;
	strcpy(type_cmd[0],"name");
	strcpy(type_cmd[1],"ip");

	while (command < cnt)
	{
		while(symbol < MAX_LEN_CMD_TEL)
		{
			if (type_cmd[command][symbol] != tel.buf[symbol]) {break;}
			symbol++;
			if (symbol == dat_obj[command][0])
			{
				tel.len_mes = symbol;
				tel.mode = dat_obj[command][1];
				return;
			}
		}
		command++;
	}
}