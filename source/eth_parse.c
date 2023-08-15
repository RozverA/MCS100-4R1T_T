#include "def.h"

BYTE eth_cbuf[MAX_SIZE_BUF_SPI];
volatile WORD size_rd=0;
WORD ch_pause = 0;
BYTE ch_sock = 1;

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

void eth_process(void)
{
	static BYTE eth_st=0;
	WORD rtrn = 0;
	
	switch(eth_st)
	{
		case CHECK:				
			//write check
			rtrn = check_data_wr_process(eth_cbuf);// copy to u_port ;rtrn - sock numb;			
			if(rtrn != MAX_SOCKETS)								
			{
				eth_st = WRITE_PROCESS;
				if (cfg.sock_rs485[w5500_mode.numb_socket].mode == TCP_MODE)	{w5500_mode.mode_op=MODE_OP_WRITE_TCP;}
				else                                                       		{w5500_mode.mode_op=MODE_OP_WRITE_UDP;}
				w5500_mode.numb_socket = rtrn;
				break;
			}
			//check TCP reload necessity
			if (ch_pause < eth_wait) 
			{
				low_prioriti_cmd(&eth_st);
			}
			check_sockets_process((BYTE*)&w5500_mode);//select sockets for read			
			eth_st=READ_PROCESS;
		break;
		case READ_PROCESS:
			rtrn=w5500_process (w5500_mode.mode_op,w5500_mode.numb_socket,eth_cbuf);
			if(rtrn==ERROR){eth_st=0;break;}
			if(rtrn)
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
		case GIVE_STAT:
			rtrn=w5500_process (w5500_mode.mode_op,w5500_mode.numb_socket,eth_cbuf);
		break;

	}
}

void low_prioriti_cmd(BYTE* eth_st)
{
	timer_upd(&ch_pause);
	if (ch_sock < 4)		{tcp_check(ch_sock, eth_st);}
	else if (ch_sock < 4)	{w5500_st_upd(ch_sock - 4);}
	else					{chip_stat();}
	ch_sock++;
	if (ch_sock == 9)	{ch_sock = 0;}
}

// check disconnection tcp 
void tcp_check(BYTE ch_sock, BYTE* eth_st)
{
	if (cfg.sock_rs485[ch_sock].mode != TCP_MODE) {return;}
		
	*eth_st = TCP_SOCK_PROCESS;					//check set mode
	w5500_mode.mode_op = MODE_OP_SOCK_TCP_CH;	//w5500 mode - check TCP connect
	w5500_mode.numb_socket = ch_sock;			//set port fur set
	
	return;
}

// set time for low priority cmd
void timer_upd (BYTE timer)
{
	if ( eth_wait > TIMER_LMT)	{timer = eth_wait - TIMER_LMT;	return;}//check read timeout
	else						{timer = eth_wait + 500;	return;}
}
				
void w5500_st_upd(BYTE sock_numb)
{
	if (cfg.sock_rs485[ch_sock].mode != UDP_MODE) {return;}
	w5500_mode.mode_op = GIVE_STAT;			//collect status port
	w5500_mode.numb_socket = sock_numb;		//set port fur set
}

void check_sockets_process (BYTE *buf)
{
	static BYTE index=MAX_SOCKETS;
	
	index++;
	if(index>MAX_SOCKETS){index=0;}
		
	if(!index)	
	{
		w5500_mode.numb_socket	=SOCKET_0; 
		w5500_mode.mode_op		=MODE_OP_READ_UDP; 
		return;
	}
			
	if(cfg.sock_rs485[index - 1].en==FALSE) {return;} 
	w5500_mode.numb_socket = index;
	
	
	if (cfg.sock_rs485[index - 1].mode == TCP_MODE)	{w5500_mode.mode_op=MODE_OP_READ_TCP;} 
	else											{w5500_mode.mode_op=MODE_OP_READ_UDP;}
	
	return;
}

void eth_udp_parse (BYTE numb_sock,BYTE *buf,WORD size)
{	
	WORD default_mtu;
	BYTE* ptr_port_udp;
	
	if(size < LEN_HDR)		{return;} //size_ip+size_port
	if (numb_sock == 0)
	{	
		default_mtu=DEFAULT_MTU_UDP;
		ptr_port_udp=(BYTE*)&eth_sock[numb_sock];
	}
	else if (cfg.sock_rs485[numb_sock - 1].mode == TCP_MODE)
	{
		default_mtu=DEFAULT_MTU_TCP;
		ptr_port_udp=((BYTE*) & eth_sock[numb_sock]);
		ptr_port_udp=ptr_port_udp + SKIPP_HADER;
	}
	else
	{
		default_mtu=DEFAULT_MTU_UDP;
		ptr_port_udp=(BYTE*)&eth_sock[numb_sock];
	}
	if(size>default_mtu){size=default_mtu;}
	memcpy(ptr_port_udp,buf,size);
	eth_sock[numb_sock].r_status = 1;
}

BYTE check_data_wr_process (BYTE *data_buf)
{
	BYTE sock_numb=0;
	WORD size=0;		

	for( sock_numb = 0; sock_numb < MAX_SOCKETS; sock_numb++ )
	{
		if( eth_sock[sock_numb].w_status == 1 )
		{
			size=( ( eth_sock[sock_numb].len[0] << 8 ) | ( eth_sock[sock_numb].len[1] ) ) + LEN_HDR;
			if( size > DEFAULT_MTU_TCP ) { size = DEFAULT_MTU_TCP; }
			memcpy( data_buf, (BYTE*) & eth_sock[sock_numb], size );
			eth_sock[sock_numb].w_status = 0;
			return sock_numb;
		}
	}
	return (MAX_SOCKETS);	
}
