#include "def.h"

BYTE eth_cbuf[MAX_SIZE_BUF_SPI];
volatile WORD size_rd=0;
BYTE ch_pause = 0;
BYTE ch_sock = 1;
#define CH_TIMER 75

void eth_init(void)
{
	w5500_init_reg();
	
	//common socket for settings
	socket_init(SOCKET_0,cfg.com_network.src_port,UDP_MODE);

	//port socket
	if(cfg.sock_rs485[0].en==TRUE) {socket_init(SOCKET_1,cfg.sock_rs485[0].src_port,cfg.sock_rs485[0].mode);}
	if(cfg.sock_rs485[1].en==TRUE) {socket_init(SOCKET_2,cfg.sock_rs485[1].src_port,cfg.sock_rs485[1].mode);}
	if(cfg.sock_rs485[2].en==TRUE) {socket_init(SOCKET_3,cfg.sock_rs485[2].src_port,cfg.sock_rs485[2].mode);}
	if(cfg.sock_rs485[3].en==TRUE) {socket_init(SOCKET_4,cfg.sock_rs485[3].src_port,cfg.sock_rs485[3].mode);}
}


#define CHECK 0
#define READ_PROCESS 1
#define WRITE_PROCESS 2
#define TCP_SOCK_PROCESS 3

void eth_process(void)
{
	static BYTE eth_st=0;
	WORD rtrn = 0;
	
	switch(eth_st)
	{
		case CHECK:				
			//write check
			rtrn = check_data_wr_process(eth_cbuf);// copy to port_udp ;rtrn - sock numb;			
			if(rtrn != MAX_SOCKETS)								
			{
				eth_st = WRITE_PROCESS;
				if (cfg.sock_rs485[w5500_mode.numb_socket].mode == TCP_MODE)	{w5500_mode.mode_op=MODE_OP_WRITE_TCP;}
				else                                                       		{w5500_mode.mode_op=MODE_OP_WRITE_UDP;}
				w5500_mode.numb_socket = rtrn;
				break;
			}
					//if (rtrn == 5)	{rtrn = 0;} //for fix error: select memory area outside struct (cfg.sock_rs485[max = 4(0-3)])
			//check TCP reload necessity
			if (cfg.sock_rs485[ch_sock].mode == TCP_MODE)//!
			{
				ch_pause++;
				if (ch_pause>CH_TIMER)
				{
					ch_pause = 0;								//check drop
					eth_st = TCP_SOCK_PROCESS;					//check set
					w5500_mode.mode_op = MODE_OP_SOCK_TCP_CH;	//check status tcp port
					w5500_mode.numb_socket = ch_sock;			//set port fur set
					ch_sock++;
					if (ch_sock == 4){ch_sock = 0;}
					return;
				}
			}
			//drop check number
			else 
			{
				ch_sock++;
				if (ch_sock == 4){ch_sock = 0;}
			}	
			 //select sockets for read			
			check_sockets_process((BYTE*)&w5500_mode);         
			eth_st=READ_PROCESS;
		break;
		case READ_PROCESS:
			rtrn=w5500_process (w5500_mode.mode_op,w5500_mode.numb_socket,eth_cbuf);
			if(rtrn==2){eth_st=0;break;}
			if(rtrn   )
			{eth_udp_parse(w5500_mode.numb_socket,eth_cbuf,rtrn); eth_st=0;}
		break;
		case WRITE_PROCESS:
			rtrn=w5500_process (w5500_mode.mode_op,w5500_mode.numb_socket,eth_cbuf);
			if(rtrn){eth_st=0;}
		break;
		case TCP_SOCK_PROCESS:
			rtrn=w5500_process (w5500_mode.mode_op,w5500_mode.numb_socket,eth_cbuf);
			if(rtrn){eth_st=0;break;}
		break;
	}
}



void check_sockets_process (BYTE *buf)
{
	static BYTE index=0;
	
	switch(index)
	{			
		case 0:
				w5500_mode.numb_socket=SOCKET_0;
		break;
		case 1:
				if(cfg.sock_rs485[0].en==FALSE) {index++;return;}
				w5500_mode.numb_socket=SOCKET_1;
		break;
		case 2:
				if(cfg.sock_rs485[1].en==FALSE) {index++;return;}			
				w5500_mode.numb_socket=SOCKET_2;
		break;
		case 3:
				if(cfg.sock_rs485[2].en==FALSE) {index++;return;}				
				w5500_mode.numb_socket=SOCKET_3;
		break;
		case 4:
				if(cfg.sock_rs485[3].en==FALSE) {index++;return;}
				w5500_mode.numb_socket=SOCKET_4;
		break;
	}
	if		(w5500_mode.numb_socket == 0)								{w5500_mode.mode_op=MODE_OP_READ_UDP;/*modes.mode_op=MODE_OP_READ_UDP;*/}
	else if (cfg.sock_rs485[w5500_mode.numb_socket].mode == TCP_MODE)	{w5500_mode.mode_op=MODE_OP_READ_TCP;/*modes.mode_op=MODE_OP_READ_TCP;*/} 
	else																{w5500_mode.mode_op=MODE_OP_READ_UDP;/*modes.mode_op=MODE_OP_READ_UDP;*/}
	index++;
	if(index==MAX_SOCKETS){index=0;}
	return;
}


void eth_udp_parse (BYTE numb_sock,BYTE *buf,WORD size)
{	
	if(size < LEN_HDR)		{return;} //size_ip+size_port
	WORD default_mtu;
	BYTE* ptr_port_udp;
	if (numb_sock == 0)
	{	
		default_mtu=DEFAULT_MTU_UDP;
		ptr_port_udp=(BYTE*)&u_port[numb_sock];
	}
	else if (cfg.sock_rs485[numb_sock-1].mode == TCP_MODE)
	{
		default_mtu=DEFAULT_MTU_TCP;
		ptr_port_udp=((BYTE*)&u_port[numb_sock]);
		ptr_port_udp=ptr_port_udp+8;
	}
	else
	{
		default_mtu=DEFAULT_MTU_UDP;
		ptr_port_udp=(BYTE*)&u_port[numb_sock];
	}
	if(size>default_mtu){size=default_mtu;}
	memcpy(ptr_port_udp,buf,size);
	u_port[numb_sock].r_status=1;
}



BYTE check_data_wr_process (BYTE *data_buf)
{
	BYTE sock_numb=0;
	WORD size=0;		

	for(sock_numb=0;sock_numb<MAX_SOCKETS;sock_numb++)
	{
		if(u_port[sock_numb].w_status==1)
		{
			size=((u_port[sock_numb].len[0]<<8) | (u_port[sock_numb].len[1]))+LEN_HDR;
			if(size>DEFAULT_MTU_TCP){size=DEFAULT_MTU_TCP;}
			memcpy(data_buf,(BYTE*)&u_port[sock_numb],size);
			u_port[sock_numb].w_status=0;
			return sock_numb;
		}
	}
	return (MAX_SOCKETS);	
}
