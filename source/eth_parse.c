#include "def.h"

BYTE eth_cbuf[MAX_SIZE_BUF_SPI];
volatile WORD size_rd=0;
TEST test;
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
	
	//telnet socket
	socket_init(SOCKET_5,TELNET_PORT,TCP_MODE);
}


#define CHECK 0
#define READ_PROCESS 1
#define WRITE_PROCESS 2
#define TCP_SOCK_PROCESS 3

void eth_process(void)
{
	static BYTE eth_st=0;
	WORD rtrn=0;
	
	switch(eth_st)
	{
		case CHECK:				
			rtrn=check_data_wr_process(eth_cbuf);// copy to port_udp ;rtrn - sock numb;			
			if(rtrn!=TMAX_SOCKETS)								
			{
				eth_st=WRITE_PROCESS;
				if (rtrn == 5)														{w5500_mode.mode_op=MODE_OP_READ_TCP;}//telnet
				else if (cfg.sock_rs485[w5500_mode.numb_socket-1].mode == TCP_MODE)	{w5500_mode.mode_op=MODE_OP_WRITE_TCP;}//подозрительно сопоставить numbsock  с rs485
				else																{w5500_mode.mode_op=MODE_OP_WRITE_UDP;}//cfg port
				w5500_mode.numb_socket=rtrn;
				break;
			}
			//write-tcp_check
			if ((cfg.sock_rs485[ch_sock].mode == TCP_MODE) || (ch_sock == 4))//0-3 485 | 4 - telnet
			{
				ch_pause++;
				if (ch_pause>CH_TIMER)
				{
					ch_pause = 0;								//check drop
					eth_st = TCP_SOCK_PROCESS;					//check set
					w5500_mode.mode_op = MODE_OP_SOCK_TCP_CH;	//check status tcp port
					w5500_mode.numb_socket = ch_sock;			//set port fur set
					ch_sock++;
					if (ch_sock == 5){ch_sock = 0;}
					return;
				}
			}
			else {ch_sock++;if (ch_sock == 5){ch_sock = 0;}	}	
			//tcp_check - read						
			check_sockets_process((BYTE*)&w5500_mode);          //select sockets for read
			eth_st=READ_PROCESS;
		break;
		case READ_PROCESS:
			rtrn=w5500_process (w5500_mode.mode_op,w5500_mode.numb_socket,eth_cbuf);
			if(rtrn==2){eth_st=0;break;}
			if(rtrn   ){eth_udp_parse(w5500_mode.numb_socket,eth_cbuf,rtrn); eth_st=0;}
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
				if(cfg.sock_rs485[3].en==FALSE) {index=0;return;}
				w5500_mode.numb_socket=SOCKET_4;
		break;
		case 5:
				w5500_mode.numb_socket=SOCKET_5;
		break;
	}
	if(w5500_mode.numb_socket == 0)										{w5500_mode.mode_op=MODE_OP_READ_UDP;/*cfg port 5000*/}
	else if (w5500_mode.numb_socket == 5)								{w5500_mode.mode_op=MODE_OP_READ_TCP;/*telnet port 23*/} 
	else if (cfg.sock_rs485[w5500_mode.numb_socket].mode == TCP_MODE)	{w5500_mode.mode_op=MODE_OP_READ_TCP;/*rs485 port 5001-5004;*/} 
	else																{w5500_mode.mode_op=MODE_OP_READ_UDP;/*rs485 port 5001-5004*/}
	index++;
	if(index==TMAX_SOCKETS){index=0;}
	return;
}


void eth_udp_parse (BYTE numb_sock,BYTE *buf,WORD size)
{	
	if(size < LEN_HDR)		{return;} //size_ip+size_port+cs
	WORD default_mtu;
	BYTE* ptr_port_udp;
	if (numb_sock == 0)
	{	
		default_mtu=DEFAULT_MTU_UDP;
		ptr_port_udp=(BYTE*)&port_udp[numb_sock];
	}
	else if (numb_sock == 5)
	{
		default_mtu=DEFAULT_MTU_TCP;
		ptr_port_udp=((BYTE*)&port_udp[numb_sock]);
		ptr_port_udp=ptr_port_udp+8;
	}
	else if (cfg.sock_rs485[numb_sock-1].mode == TCP_MODE)
	{
		default_mtu=DEFAULT_MTU_TCP;
		ptr_port_udp=((BYTE*)&port_udp[numb_sock]);
		ptr_port_udp=ptr_port_udp+8;
	}
	else
	{
		default_mtu=DEFAULT_MTU_UDP;
		ptr_port_udp=(BYTE*)&port_udp[numb_sock];
	}
	if(size>default_mtu){size=default_mtu;}
	memcpy(ptr_port_udp,buf,size);
	port_udp[numb_sock].r_status=1;
}



BYTE check_data_wr_process (BYTE *data_buf)
{
	BYTE sock_numb=0;
	WORD size=0;		

	for(sock_numb=0;sock_numb<TMAX_SOCKETS;sock_numb++)
	{
		if(port_udp[sock_numb].w_status==1)
		{
			size=((port_udp[sock_numb].len[0]<<8) | (port_udp[sock_numb].len[1]))+LEN_HDR;
			if(size>DEFAULT_MTU_TCP){size=DEFAULT_MTU_TCP;}
			memcpy(data_buf,(BYTE*)&port_udp[sock_numb],size);
			port_udp[sock_numb].w_status=0;
			return sock_numb;
		}
	}
	return (MAX_SOCKETS);	
}
