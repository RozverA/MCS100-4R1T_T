#include "def.h"

BYTE eth_cbuf[MAX_SIZE_BUF_SPI];
volatile WORD size_rd=0;
TEST test;


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

void eth_process(void)
{
	static BYTE eth_st=0;
	WORD rtrn=0;
	
	switch(eth_st)
	{
		case CHECK:				
								rtrn=check_data_wr_process(eth_cbuf);          //check wr_status
								if(rtrn!=MAX_SOCKETS) //если сокет задействован, то изменить статус в структуре w5500
								{
									eth_st=WRITE_PROCESS;
									if (cfg.sock_rs485[w5500_mode.numb_socket].mode == TCP_MODE){w5500_mode.mode_op=MODE_OP_WRITE_TCP;}
									else                                                        {w5500_mode.mode_op=MODE_OP_WRITE_UDP;}
									w5500_mode.numb_socket=rtrn;
									break;
								}
								
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
	}

}



void check_sockets_process (BYTE *buf)
{
	static BYTE index=0;
	W5500_MODE modes;
	
	switch(index)
	{			case 0:
						modes.numb_socket=SOCKET_0;
						test.check_sock_0++;//флаг о прочтении
				break;
				case 1:
						if(cfg.sock_rs485[0].en==FALSE) {index++;return;}
						modes.numb_socket=SOCKET_1;
						test.check_sock_1++;
				break;
				case 2:
						if(cfg.sock_rs485[1].en==FALSE) {index++;return;}			
						modes.numb_socket=SOCKET_2;
						test.check_sock_2++;
				break;
				case 3:
						if(cfg.sock_rs485[2].en==FALSE) {index++;return;}				
						modes.numb_socket=SOCKET_3;
						test.check_sock_3++;
				break;
				case 4:
						if(cfg.sock_rs485[3].en==FALSE) {index=0;return;}
						modes.numb_socket=SOCKET_4;
						test.check_sock_4++;
				break;
	}
	index++;
	if(index==MAX_SOCKETS){index=0;}
	if (cfg.sock_rs485[w5500_mode.numb_socket].mode == TCP_MODE){modes.mode_op=MODE_OP_READ_TCP;}
	else                                                        {modes.mode_op=MODE_OP_READ_UDP;}
	memcpy(buf,(BYTE*)&modes,sizeof(W5500_MODE));
	return;
}


void eth_udp_parse (BYTE numb_sock,BYTE *buf,WORD size)
{	
	if(size < LEN_HDR)		{return;} //size_ip+size_port
		
	switch(numb_sock)
	{
					case SOCKET_0:
									if(size>DEFAULT_MTU_TCP){size=DEFAULT_MTU_TCP;}
									memcpy((BYTE*)&port_udp[0],buf,size);
									port_udp[0].r_status=1;
									test.sock_parse_0++;
					break;
					case SOCKET_1:
									if(size>DEFAULT_MTU_TCP){size=DEFAULT_MTU_TCP;}
									memcpy((BYTE*)&port_udp[1],buf,size);
									port_udp[1].r_status=1;
									test.sock_parse_1++;					
					break;
					case SOCKET_2:
									if(size>DEFAULT_MTU_TCP){size=DEFAULT_MTU_TCP;}
									memcpy((BYTE*)&port_udp[2],buf,size);
									port_udp[2].r_status=1;
									test.sock_parse_2++;						
					break;
					case SOCKET_3:
									if(size>DEFAULT_MTU_TCP){size=DEFAULT_MTU_TCP;}
									memcpy((BYTE*)&port_udp[3],buf,size);
									port_udp[3].r_status=1;
									test.sock_parse_3++;						
					break;
					case SOCKET_4:
									if(size>DEFAULT_MTU_TCP){size=DEFAULT_MTU_TCP;}
									memcpy((BYTE*)&port_udp[4],buf,size);
									port_udp[4].r_status=1;
									test.sock_parse_4++;					
					break;
	}
}



BYTE check_data_wr_process (BYTE *data_buf)
{
	BYTE r=0;
	WORD size=0;		

	for(r=0;r<MAX_SOCKETS;r++)
	{
		if(port_udp[r].w_status==1)
		{
			size=((port_udp[r].len[0]<<8) | (port_udp[r].len[1]))+LEN_HDR;
			if(size>DEFAULT_MTU_TCP){size=DEFAULT_MTU_TCP;}
			memcpy(data_buf,(BYTE*)&port_udp[r],size);
			port_udp[r].w_status=0;
			return r;
		}
	}
	return (MAX_SOCKETS);	
}
