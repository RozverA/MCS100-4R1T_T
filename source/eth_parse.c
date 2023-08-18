#include "def.h"

BYTE eth_cbuf[MAX_SIZE_BUF_SPI];
WORD time_ch_pause = 0;


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
				w5500_mode.mode_op=MODE_OP_WRITE_UDP;
				
				if ((cfg.sock_rs485[w5500_mode.numb_socket].mode == TCP_MODE) && (rtrn))	{w5500_mode.mode_op=MODE_OP_WRITE_TCP;}
				w5500_mode.numb_socket = rtrn;
				break;
			}
			//check TCP reload necessity
			 
			if(eth_st==ch_process()){break;}
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

BYTE ch_process (void)
{
	static BYTE ch_sock = 0;
	BYTE eth_st;
	
	if (time_ch_pause > time_eth_wait){return;}
		
	time_ch_pause = time_eth_wait + 1000;
	
	timer_eth_wait_upd();
	tcp_check(ch_sock, &eth_st);
	w5500_st_upd(ch_sock);
	chip_stat();
	
	ch_sock++;
	if (ch_sock == 4)	{ch_sock = 0;}
	return eth_st;
}


void timer_eth_wait_upd(void)
{
	if(time_eth_wait<0x00000F00){return;}
		
	time_eth_wait=0;
	time_ch_pause=0;
	vars.time[0]=0;
	vars.time[1]=0;
	vars.time[2]=0;	
	vars.time[3]=0;			
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

				
void w5500_st_upd(BYTE sock_numb)
{
	if (cfg.sock_rs485[sock_numb].mode != UDP_MODE) {return;}
	w5500_mode.mode_op = GIVE_STAT;			//collect status port
	w5500_mode.numb_socket = sock_numb;		//set port fur set
}

BYTE check_sockets_process (BYTE *buf)
{
	static BYTE index = MAX_SOCKETS;
	
	index++;
	if(index > MAX_SOCKETS){index=0;}
	
	if(!index)	
	{
		w5500_mode.numb_socket	= SOCKET_0; 
		w5500_mode.mode_op		= MODE_OP_READ_UDP;	
		return;
	}
	
	w5500_mode.numb_socket = index;
	if (cfg.sock_rs485[index-1].mode == TCP_MODE)	{w5500_mode.mode_op=MODE_OP_READ_TCP;} 
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
		ptr_port_udp=ptr_port_udp + SKIP_HDR;
	}
	else
	{
		default_mtu=DEFAULT_MTU_UDP;
		ptr_port_udp=(BYTE*)&eth_sock[numb_sock];
	}
	if(size>default_mtu){size=default_mtu;}
	memcpy(ptr_port_udp,buf,size);
	eth_sock[numb_sock].r_status = 1;
	eth_sock[numb_sock].counters.rx++;
}

BYTE check_data_wr_process (BYTE *data_buf)
{
	BYTE numb_sock=0;
	WORD size=0;		

	for( numb_sock = 0; numb_sock < MAX_SOCKETS; numb_sock++ )
	{
		if( eth_sock[numb_sock].w_status == 1 )
		{
			size=( ( eth_sock[numb_sock].len[0] << 8 ) | ( eth_sock[numb_sock].len[1] ) ) + LEN_HDR;
			if( size > DEFAULT_MTU_TCP ) { size = DEFAULT_MTU_TCP; }
			memcpy( data_buf, (BYTE*) & eth_sock[numb_sock], size );
			eth_sock[numb_sock].w_status = 0;
			eth_sock[numb_sock].counters.tx++;
			return numb_sock;
		}
	}
	return (MAX_SOCKETS);	
}
