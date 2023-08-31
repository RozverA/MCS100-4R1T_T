#include "def.h"

BYTE eth_cbuf[MAX_SIZE_BUF_SPI];
ETH_HDR eth_sock[MAX_SOCKETS];

void eth_init(void)
{
	w5500_init_reg();
	
	//common socket for settings
	socket_init(SOCKET_0,cfg_1.com_network.src_port,UDP);

	//port socket
	if(cfg_1.sock_rs485[0].en==TRUE) {socket_init(SOCKET_1,cfg_1.sock_rs485[0].src_port,cfg_1.sock_rs485[0].mode);}
	if(cfg_1.sock_rs485[1].en==TRUE) {socket_init(SOCKET_2,cfg_1.sock_rs485[1].src_port,cfg_1.sock_rs485[1].mode);}
	if(cfg_1.sock_rs485[2].en==TRUE) {socket_init(SOCKET_3,cfg_1.sock_rs485[2].src_port,cfg_1.sock_rs485[2].mode);}
	if(cfg_1.sock_rs485[3].en==TRUE) {socket_init(SOCKET_4,cfg_1.sock_rs485[3].src_port,cfg_1.sock_rs485[3].mode);}

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
				
				if ((cfg_1.sock_rs485[w5500_mode.numb_socket].mode == TCP) && (rtrn))	{w5500_mode.mode_op=MODE_OP_WRITE_TCP;}
				w5500_mode.numb_socket = rtrn;
				break;
			}

			check_sockets_process((BYTE*)&w5500_mode);//select sockets for read			
			eth_st=READ_PROCESS;
		break;
		case READ_PROCESS:
			rtrn=w5500_process (w5500_mode.mode_op,w5500_mode.numb_socket,eth_cbuf);
			if(rtrn==ERROR){eth_st=0;break;}
			if(rtrn)
			{eth_parse(w5500_mode.numb_socket,eth_cbuf,rtrn); eth_st=0;}
		break;
		case WRITE_PROCESS:
			rtrn=w5500_process (w5500_mode.mode_op,w5500_mode.numb_socket,eth_cbuf);
			if(rtrn){eth_st=0;}
		break;
		default:
			eth_st = CHECK;
		break;

	}
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
	if (cfg_1.sock_rs485[index-1].mode == TCP)	{w5500_mode.mode_op=MODE_OP_READ_TCP;} 
	else										{w5500_mode.mode_op=MODE_OP_READ_UDP;}
	return;
}

void eth_parse (BYTE numb_sock,BYTE *buf,WORD size)
{	
	WORD default_mtu;
	BYTE ptr_port=0;
	
	default_mtu=DEFAULT_MTU_UDP;
	
	if ((cfg_1.sock_rs485[numb_sock - 1].mode == TCP) && (numb_sock))
	{
		default_mtu=DEFAULT_MTU_TCP;
		ptr_port=SKIP_HDR;
	}

	if(size>default_mtu){size=default_mtu;}
		
	eth_sock[numb_sock].counters.rx++;
	
	if(eth_sock[numb_sock].r_status == 1){ return;}
	
	memcpy((BYTE*)&eth_sock[numb_sock]+ptr_port,buf,size);
	eth_sock[numb_sock].r_status = 1;
	
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
