#include "def.h"

w5500 chip;

WORD addr_w5500=0;
WORD cb_w5500=0;
BYTE *ptr_buf;
WORD len_buf=0;
BYTE cmd;
BYTE wbuf_w55[10];

W5500_MODE w5500_mode;

volatile WORD sizert=0;

BYTE w5500_init_reg(void)
{
	pin_ctrl(W55,PWR,SET);

	if(ip_init()	)	{err_dword.ip_init_er   = 1;	return ERROR;}
	if(mask_init()	)	{err_dword.mask_init_er = 1;	return ERROR;}
	if(gw_init()	)	{err_dword.gw_init_er   = 1;	return ERROR;}		
	if(mac_init()	)	{err_dword.mac_init_er  = 1;	return ERROR;}
		
	return SUCCESS;
}

BYTE mac_init(void)
{
	spi_write_timeout(ADDR_COM_MAC_ADDR_0,COMMON_REGISTER,cfg_2.mac_addr,sizeof(cfg_2.mac_addr),10);
	return SUCCESS;
}
BYTE gw_init(void)
{
	spi_write_timeout(ADDR_COM_GATEWAY_ADDR_0,COMMON_REGISTER,cfg_1.com_network.ip_gate,sizeof(cfg_1.com_network.ip_gate),10);
	return SUCCESS;
}
BYTE mask_init(void)
{
	spi_write_timeout(ADDR_COM_MASK_ADDR_0,COMMON_REGISTER,cfg_1.com_network.ip_mask,sizeof(cfg_1.com_network.ip_mask),10);
	return SUCCESS;
}
BYTE ip_init(void)
{
	spi_write_timeout(ADDR_COM_SRC_IP_ADR_0,COMMON_REGISTER,cfg_1.com_network.ip_addr,sizeof(cfg_1.com_network.ip_addr),10);
	return SUCCESS;
}

WORD w5500_process (BYTE spi_mode, BYTE sock_numb)
{
	static BYTE w5500_st=0;
	WORD rtrn=0;
	if(!cmd_spi_wait){w5500_st=0;return 2;} //if 20 mS no answer, break process
		
		
	switch (w5500_st)
	{
		case NULLS:
								w5500_st=spi_mode;
								cmd_spi_wait=time_20ms;
								return PROC_WAIT;
		case SPI_PROCESS:
								if(spi_process(addr_w5500,cb_w5500,ptr_buf,len_buf,cmd)){w5500_st=spi_mode;cmd=0;}
		break;
		case MODE_OP_READ_UDP:
								rtrn=w5500_cmd_read_socket_udp(sock_numb);
								if(rtrn)	{w5500_st=NULLS;spi_mode=0;return (rtrn);}
								w5500_st=SPI_PROCESS;
		break;
		case MODE_OP_WRITE_UDP:
								if(w5500_write_socket_udp(sock_numb))	{w5500_st=NULLS;spi_mode=0;return PROC_OK;}
								w5500_st=SPI_PROCESS;
		break;
		case MODE_OP_READ_TCP:
								rtrn=w5500_cmd_read_socket_tcp(sock_numb);
								if(rtrn)	
								{
																										if(rtrn > 2	)	
																										{rtrn = rtrn;}
								w5500_st=NULLS;spi_mode=0;return (rtrn);}
								w5500_st=SPI_PROCESS;
		break;
		case MODE_OP_WRITE_TCP:
								if(w5500_write_socket_tcp(sock_numb))	{w5500_st=NULLS;spi_mode=0;return PROC_OK;}
								w5500_st=SPI_PROCESS;
		break;
		default:
								w5500_st=0; return PROC_ER;
		break;
	}
	return PROC_WAIT;
}

WORD w5500_cmd_read_socket_udp (BYTE sock_numb)
{
	static BYTE numb_static_r=0;
	static BYTE st_cmd_w5500=0;
	static WORD size=0;
	BYTE cnt=0;
	
	if(numb_static_r!=sock_numb){numb_static_r=sock_numb;st_cmd_w5500=0;}
	
	switch(st_cmd_w5500)
	{
		case UDP_GIVE_LEN:
			addr_w5500=ADDR_SOC_RX_RECEIVED_SIZE_0;//addr in w5500 1,2
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);//cb 3
			ptr_buf=(BYTE*)&chip.sockReg[sock_numb].R017_Sn_RX_RSR_26_27;//4 data (point write data aus w5500)
			len_buf=2;//len
			cmd=READ_DATA;//mode
			st_cmd_w5500++;//"next"
								
			size=0;
		break;	
		case UDP_PART_RD:					
			sizert=(*(BYTE*)&chip.sockReg[sock_numb].R017_Sn_RX_RSR_26_27.case1<<8) | (*(BYTE*)&chip.sockReg[sock_numb].R017_Sn_RX_RSR_26_27.case2);//check len
			if(sizert!=0x0000)//back & return
			{
				addr_w5500=eth_sock[sock_numb].ptr_rx_buf;
				cb_w5500=SOCKET_RX_BUFFER | SOCKET(sock_numb);
				ptr_buf=(BYTE*) & eth_sock[sock_numb];
				len_buf=sizert;
				eth_sock[sock_numb].ptr_rx_buf=eth_sock[sock_numb].ptr_rx_buf+len_buf;
				size=len_buf;
				if(len_buf>DEFAULT_MTU_UDP){size=PROC_ER;}							
				cmd=READ_DATA;
				st_cmd_w5500++;
				break;
			}
			st_cmd_w5500--;
			return PROC_ER;
		break;
		case UDP_PTR_MOVE:
			wbuf_w55[cnt]=(eth_sock[sock_numb].ptr_rx_buf>>8);			cnt++;//move to next part messege data 1
			wbuf_w55[cnt]=(BYTE)eth_sock[sock_numb].ptr_rx_buf;			cnt++;//2d byte data 2
			addr_w5500=ADDR_SOC_RX_READ_PTR_0;//reg RX_RD(28)
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=wbuf_w55;/*(записать в него смещение)*/
			len_buf=cnt;
			cmd=WRITE_DATA;
			st_cmd_w5500++;
		break;
		case UDP_RCV_CMD:
			wbuf_w55[cnt]=CMD_RECV;					cnt++;//перейти в режим приема(возобновить прием)
			addr_w5500=ADDR_SOC_COMMAND;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=wbuf_w55;
			len_buf=cnt;
			cmd=WRITE_DATA;
			st_cmd_w5500++;
		break;	
		case UDP_BK_START:
			st_cmd_w5500=0;//сброс параметров
			if((size > USART_BUF_SIZE) && (sock_numb))	 {return PROC_ER;}
			return (size);
		break;
		default:
				st_cmd_w5500=0; return PROC_ER;
		break;
	}
	return PROC_WAIT;
}

WORD w5500_write_socket_udp (BYTE sock_numb)
{
	static BYTE st_wr_w5500 = 0;
	static BYTE numb_static = 0;
	WORD cnt = 0;
	
	if(numb_static != sock_numb) {numb_static = sock_numb; st_wr_w5500 = 0;}

	switch(st_wr_w5500)
	{
		case UDP_RD_TX_STAT:																	//read reg tx status
			addr_w5500=ADDR_SOC_TX_FREE_SIZE_0;						
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=(BYTE*)&chip.sockReg[sock_numb].R014_Sn_TX_FSR_20_21;
			len_buf=6;
			cmd=READ_DATA;
			st_wr_w5500++;
		break;
		case UDP_WR_IP:																	//write ip
			addr_w5500=ADDR_SOC_D_IP_ADDR0;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=(BYTE*)&eth_sock[sock_numb].ip_addr;
			len_buf=4;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case UDP_WR_PORT:																	//write port
			addr_w5500=ADDR_SOC_D_PORT0;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=(BYTE*)&eth_sock[sock_numb].port;
			len_buf=2;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case UDP_WR_DATA:																	//write data
			addr_w5500=eth_sock[sock_numb].ptr_tx_buf;
			cb_w5500=SOCKET_TX_BUFFER | SOCKET(sock_numb);
			ptr_buf=(BYTE*)&eth_sock[sock_numb].data;
			len_buf=(eth_sock[sock_numb].len[0] << 8) | (eth_sock[sock_numb].len[1]);
			eth_sock[sock_numb].ptr_tx_buf=eth_sock[sock_numb].ptr_tx_buf+len_buf;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case UDP_WR_PTR:																	//write ptr
			wbuf_w55[cnt]=(eth_sock[sock_numb].ptr_tx_buf>>8);		cnt++;
			wbuf_w55[cnt]=(BYTE)eth_sock[sock_numb].ptr_tx_buf;		cnt++;
						
			addr_w5500=ADDR_SOC_TX_WRITE_PTR_0;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=wbuf_w55;
			len_buf=cnt;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case UDP_SEND_CMD:																	//write cmd SEND
			wbuf_w55[cnt]=CMD_SEND;				cnt++;
						
			addr_w5500=ADDR_SOC_COMMAND;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=wbuf_w55;
			len_buf=cnt;
			cmd=WRITE_DATA;
			st_wr_w5500++;
			eth_sock[sock_numb].time_wait_SEND_OK=time_10ms;
		break;
		case UDP_STAT_RD:																//read status_SEND_OK
			addr_w5500=ADDR_SOC_INT;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=(BYTE*)&chip.sockReg[sock_numb].R03_Sn_IR_02;
			len_buf=1;
			cmd=READ_DATA;
			st_wr_w5500++;
		break;
		case UDP_STATUS_CLR:															//clear status SEND_OK
			if(chip.sockReg[sock_numb].R03_Sn_IR_02.SEND_OK==1)
			{
				wbuf_w55[cnt]=SR_2C_SEND_OK;			cnt++;
				addr_w5500=ADDR_SOC_INT;
				cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
				ptr_buf=wbuf_w55;
				len_buf=cnt;
				cmd=WRITE_DATA;
				st_wr_w5500++;
				break;
			}
			st_wr_w5500--;
			if(!eth_sock[sock_numb].time_wait_SEND_OK)
			{
				st_wr_w5500=0; 
				return PROC_ER;
			}
		break;
		case 8:
				st_wr_w5500=0;
				return PROC_OK;
		break;
		default:
				st_wr_w5500=0; 
				return PROC_ER;
		break;
	}
	return PROC_WAIT;
}

WORD w5500_cmd_read_socket_tcp (BYTE sock_numb)
{
	static BYTE st_cmd_w5500=0;
	static WORD size=0;
	static BYTE status_ded[MAX_SOCKETS_CNT];
	BYTE cnt=0;

	switch(st_cmd_w5500)
	{
		case TCP_GIVE_LEN:
			addr_w5500=ADDR_SOC_RX_RECEIVED_SIZE_0;//addr
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);//bsb
			ptr_buf=(BYTE*)&chip.sockReg[sock_numb].R017_Sn_RX_RSR_26_27;//data
			len_buf=6;
			cmd=READ_DATA;//mode
			st_cmd_w5500 = TCP_PART_RD; //"next"
			size=0; 
			
			eth_sock[sock_numb].check_connect_cnt++;
			if(eth_sock[sock_numb].check_connect_cnt > 5) {st_cmd_w5500 = TCP_STATUS_RD; eth_sock[sock_numb].check_connect_cnt=0;}
		break;	
		case TCP_STATUS_RD:
			addr_w5500=ADDR_SOC_STATUS;//addr in w5500 
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);//bsb
			ptr_buf=(BYTE*)&chip.sockReg[sock_numb].R04_Sn_SR_03.Status;//place for write result
			len_buf=1;
			cmd=READ_DATA;//mode
			st_cmd_w5500 = TCP_FORK;//"next" 
		break;
		case TCP_FORK:
			if(status_ded[sock_numb]==chip.sockReg[sock_numb].R04_Sn_SR_03.Status){st_cmd_w5500 = TCP_PART_RD; return PROC_WAIT;}
			status_ded[sock_numb]=chip.sockReg[sock_numb].R04_Sn_SR_03.Status;
			switch(chip.sockReg[sock_numb].R04_Sn_SR_03.Status)
			{
				case ST_CLOSE:
					wbuf_w55[cnt]=CMD_OPEN;
				break;
				case ST_INIT:
					wbuf_w55[cnt]=CMD_LISTEN;
				break;
				case ST_CLOSE_WAIT:
					wbuf_w55[cnt]=CMD_CLOSE;			
				break;
				default:
					st_cmd_w5500=TCP_PART_RD;
				return PROC_WAIT;
			}
			addr_w5500=ADDR_SOC_COMMAND;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=wbuf_w55;
			len_buf=1;
			cmd=WRITE_DATA;
			st_cmd_w5500=TCP_STATUS_RD;
		break;
		case TCP_PART_RD:	
			sizert=(*(BYTE*)&chip.sockReg[sock_numb].R017_Sn_RX_RSR_26_27.case1<<8) | (*(BYTE*)&chip.sockReg[sock_numb].R017_Sn_RX_RSR_26_27.case2);//check len
			if(sizert!=0x0000)//back & return		
			{
				addr_w5500=eth_sock[sock_numb].ptr_rx_buf;//addr start messege
				cb_w5500=SOCKET_RX_BUFFER | SOCKET(sock_numb);//bsb sock RX
				ptr_buf=(BYTE*) & eth_sock[sock_numb].data;
				len_buf = sizert;
				eth_sock[sock_numb].ptr_rx_buf=eth_sock[sock_numb].ptr_rx_buf+len_buf;
				
				size=len_buf;
				if(len_buf>DEFAULT_MTU_TCP){size=PROC_ER;}
					
				eth_sock[sock_numb].len[0]=	(size & 0xFF00)>>8;
				eth_sock[sock_numb].len[1]=	(size & 0x00FF);							
				cmd=READ_DATA;
				st_cmd_w5500 = TCP_DROP_PTR;
				break;
			}
			st_cmd_w5500=TCP_GIVE_LEN;
			return PROC_ER;
		break;
		case TCP_DROP_PTR:
			wbuf_w55[cnt]=(eth_sock[sock_numb].ptr_rx_buf>>8);			cnt++;//move to next part messege data 1
			wbuf_w55[cnt]=(BYTE)eth_sock[sock_numb].ptr_rx_buf;			cnt++;//2d byte data 2
			addr_w5500=ADDR_SOC_RX_READ_PTR_0;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=wbuf_w55;
			len_buf=2;
			cmd=WRITE_DATA;
			st_cmd_w5500 = TCP_RSV_CMD;//"next" 
		break;
		case TCP_RSV_CMD:
			wbuf_w55[cnt]=CMD_RECV;					cnt++;//go to reciv mode (and rewrite number)
			addr_w5500=ADDR_SOC_COMMAND;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=wbuf_w55;
			len_buf=cnt;
			cmd=WRITE_DATA;
			st_cmd_w5500 = TCP_BK_START;//"next" 
		break;
		case TCP_BK_START:
			st_cmd_w5500=TCP_GIVE_LEN;//сброс параметров
			if(!size)					 {return PROC_ER;}
			if(size > USART_BUF_SIZE)	 {return PROC_ER;}
			return (size);
		break;
		default:
				st_cmd_w5500=0; return PROC_ER;
		break;
	}
	return PROC_WAIT;
}

WORD w5500_write_socket_tcp (BYTE sock_numb)
{
	static BYTE st_wr_w5500=0;
	static BYTE numb_static=0;
	WORD cnt=0;
	
	if(numb_static!=sock_numb){numb_static=sock_numb;st_wr_w5500=0;}

	switch(st_wr_w5500)
	{
		case TCP_RD_FSR:																	//read reg tx status FSR
			addr_w5500=ADDR_SOC_TX_FREE_SIZE_0;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=(BYTE*)&chip.sockReg[sock_numb].R014_Sn_TX_FSR_20_21;
			len_buf=6;
			cmd=READ_DATA;
			st_wr_w5500++;
		break;
		case TCP_WR_DATA:	
			sizert=	(*(BYTE*)&chip.sockReg[sock_numb].R016_Sn_TX_WR_24_25.case1<<8) | (*(BYTE*)&chip.sockReg[sock_numb].R016_Sn_TX_WR_24_25.case2);				
			
			if(sizert!=eth_sock[sock_numb].ptr_tx_buf){ eth_sock[sock_numb].ptr_tx_buf=sizert; }
																			
			addr_w5500=eth_sock[sock_numb].ptr_tx_buf;
			cb_w5500=SOCKET_TX_BUFFER | SOCKET(sock_numb);
			ptr_buf=(BYTE*)&eth_sock[sock_numb].data;
			len_buf=(eth_sock[sock_numb].len[0] << 8) | (eth_sock[sock_numb].len[1]);
			eth_sock[sock_numb].ptr_tx_buf=eth_sock[sock_numb].ptr_tx_buf+len_buf;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case TCP_PTR_WR:																	//write ptr
			wbuf_w55[cnt]=(eth_sock[sock_numb].ptr_tx_buf>>8);		cnt++;
			wbuf_w55[cnt]=(BYTE)eth_sock[sock_numb].ptr_tx_buf;		cnt++;
			addr_w5500=ADDR_SOC_TX_WRITE_PTR_0;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=wbuf_w55;
			len_buf=cnt;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case TCP_SEND_CMD:																	//write cmd SEND
			wbuf_w55[cnt]=CMD_SEND;				cnt++;
			addr_w5500=ADDR_SOC_COMMAND;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=wbuf_w55;
			len_buf=cnt;
			cmd=WRITE_DATA;
			st_wr_w5500++;
			eth_sock[sock_numb].time_wait_SEND_OK=time_10ms;
		break;
		case TCP_STAT_RD:																//read status_SEND_OK
			addr_w5500=ADDR_SOC_INT;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=(BYTE*)&chip.sockReg[sock_numb].R03_Sn_IR_02;
			len_buf=1;
			cmd=READ_DATA;
			st_wr_w5500++;
		break;
		case TCP_STATUS_CLR:																//clear status SEND_OK
			if(chip.sockReg[sock_numb].R03_Sn_IR_02.SEND_OK==1)
			{
				wbuf_w55[cnt]=SR_2C_SEND_OK;			cnt++;
				addr_w5500=ADDR_SOC_INT;
				cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
				ptr_buf=wbuf_w55;
				len_buf=cnt;
				cmd=WRITE_DATA;
				st_wr_w5500++;
				break;
			}
			st_wr_w5500--;
			if(!eth_sock[sock_numb].time_wait_SEND_OK)
			{
				st_wr_w5500=TCP_RD_FSR; 
				return PROC_ER;
			}
		break;
		case TCP_BK_TO_START:
			st_wr_w5500=TCP_RD_FSR;
			return PROC_OK;
		break;
		default:
			st_wr_w5500=TCP_RD_FSR; return PROC_ER;
		break;
	}
	return PROC_WAIT;
}
