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

func_st w5500_init_reg(void)
{
	if(ip_init()	)	{return ERROR;}
	if(mask_init()	)	{return ERROR;}
	if(gw_init()	)	{return ERROR;}		
	if(mac_init()	)	{return ERROR;}
	
	return SUCCESS;
}

func_st mac_init(void)
{
	spi_write_timeout(ADDR_COM_MAC_ADDR_0,COMMON_REGISTER,cfg.com_network.mac_addr,sizeof(cfg.com_network.mac_addr),10);
	return SUCCESS;
}
func_st gw_init(void)
{
	spi_write_timeout(ADDR_COM_GATEWAY_ADDR_0,COMMON_REGISTER,cfg.com_network.ip_gate,sizeof(cfg.com_network.ip_gate),10);
	return SUCCESS;
}
func_st mask_init(void)
{
	spi_write_timeout(ADDR_COM_MASK_ADDR_0,COMMON_REGISTER,cfg.com_network.ip_mask,sizeof(cfg.com_network.ip_mask),10);
	return SUCCESS;
}
func_st ip_init(void)
{
	spi_write_timeout(ADDR_COM_SRC_IP_ADR_0,COMMON_REGISTER,cfg.com_network.ip_addr,sizeof(cfg.com_network.ip_addr),10);
	return SUCCESS;
}

WORD w5500_process (BYTE spi_mode, BYTE sock_numb, BYTE *buf)
{
	static BYTE w5500_st=0;
	WORD rtrn=0;
	if(!cmd_spi_wait){w5500_st=0;return 2;} //if 20 mS no answer, break process
	switch (w5500_st)
	{
		case NULLS:
			w5500_st=spi_mode;
			cmd_spi_wait=time_50ms;
			return 0;
		break;
		case SPI_PROCESS:
								if(spi_process(addr_w5500,cb_w5500,ptr_buf,len_buf,cmd)){w5500_st=spi_mode;cmd=0;}
		break;
		case MODE_OP_READ_UDP:
								rtrn=w5500_cmd_read_socket_udp(sock_numb,buf);
								if(rtrn)	{w5500_st=0;spi_mode=0;return (rtrn);}
								w5500_st=SPI_PROCESS;
		break;
		case MODE_OP_WRITE_UDP:
								if(w5500_write_socket_udp(sock_numb,buf))	{w5500_st=0;spi_mode=0;return 1;}
								w5500_st=SPI_PROCESS;
		break;
		case MODE_OP_READ_TCP:
								rtrn=w5500_cmd_read_socket_tcp(sock_numb,buf);
								if(rtrn)	{w5500_st=0;spi_mode=0;return (rtrn);}
								w5500_st=SPI_PROCESS;
		break;
		case MODE_OP_WRITE_TCP:
								if(w5500_write_socket_tcp(sock_numb,buf))	{w5500_st=0;spi_mode=0;return 1;}
								w5500_st=SPI_PROCESS;
		break;
		case MODE_OP_SOCK_TCP_CH:
								rtrn = w5500_ch_sock(sock_numb);
								if(rtrn)	{w5500_st=0;spi_mode=0;return 1;}
								w5500_st=SPI_PROCESS;
		break;
		case MODE_OP_GIVE_STAT:
								w5500_give_stat(sock_numb);
								w5500_st=SPI_PROCESS;
		break;
	}
	return 0;
}

WORD w5500_cmd_read_socket_udp (BYTE numb, BYTE *buf)
{
	static BYTE numb_static_r=0;
	static BYTE st_cmd_w5500=0;
	static WORD size=0;
	BYTE cnt=0;
	if(numb_static_r!=numb){numb_static_r=numb;st_cmd_w5500=0;}
	
	switch(st_cmd_w5500)
	{
		case UDP_GIVE_LEN:
			addr_w5500=ADDR_SOC_RX_RECEIVED_SIZE_0;//addr in w5500 1,2
			cb_w5500=SOCKET_REGISTER | SOCKET(numb);//cb 3
			ptr_buf=(BYTE*)&chip.sockReg[numb].R017_Sn_RX_RSR_26_27;//4 data (point write data aus w5500)
			len_buf=2;//len
			cmd=READ_DATA;//mode
			st_cmd_w5500++;//"next"
								
			size=0;
		break;	
		case UDP_PART_RD:					
			sizert=(*(BYTE*)&chip.sockReg[numb].R017_Sn_RX_RSR_26_27.case1<<8) | (*(BYTE*)&chip.sockReg[numb].R017_Sn_RX_RSR_26_27.case2);//check len
			if(sizert!=0x0000)//back & return
			{
				addr_w5500=eth_sock[numb].ptr_rx_buf;
				cb_w5500=SOCKET_RX_BUFFER | SOCKET(numb);
				ptr_buf=buf;
				len_buf=(*(BYTE*)&chip.sockReg[numb].R017_Sn_RX_RSR_26_27.case1<<8 | *(BYTE*)&chip.sockReg[numb].R017_Sn_RX_RSR_26_27.case2);
				eth_sock[numb].ptr_rx_buf=eth_sock[numb].ptr_rx_buf+len_buf;
				if(len_buf>DEFAULT_MTU_UDP){size=2;/*min value '8'*/}else{size=len_buf;}							
				cmd=READ_DATA;
				st_cmd_w5500++;
				break;
			}
			st_cmd_w5500--;
			return 2;
		break;
		case UDP_PTR_MOVE:
			wbuf_w55[cnt]=(eth_sock[numb].ptr_rx_buf>>8);			cnt++;//move to next part messege data 1
			wbuf_w55[cnt]=(BYTE)eth_sock[numb].ptr_rx_buf;			cnt++;//2d byte data 2
			addr_w5500=ADDR_SOC_RX_READ_PTR_0;//reg RX_RD(28)
			cb_w5500=SOCKET_REGISTER | SOCKET(numb);
			ptr_buf=wbuf_w55;/*(записать в него смещение)*/
			len_buf=cnt;
			cmd=WRITE_DATA;
			st_cmd_w5500++;
		break;
		case UDP_RCV_CMD:
			wbuf_w55[cnt]=CMD_RECV;					cnt++;//перейти в режим приема(возобновить прием)
			addr_w5500=ADDR_SOC_COMMAND;
			cb_w5500=SOCKET_REGISTER | SOCKET(numb);
			ptr_buf=wbuf_w55;
			len_buf=cnt;
			cmd=WRITE_DATA;
			st_cmd_w5500++;
			
			//if(0<numb<5){/*counters.soket[numb - 1].rx += 1;*/}
		break;	
		case UDP_BK_START:
			st_cmd_w5500=0;//сброс параметров
			return (size);
		break;
	}
	return 0;
}

WORD w5500_write_socket_udp (BYTE numb, BYTE *buf)
{
	static BYTE st_wr_w5500=0;
	static BYTE numb_static=0;
	WORD cnt=0;
	WORD size=0;
	
	if(numb_static!=numb){numb_static=numb;st_wr_w5500=0;}
	if(st_wr_w5500==0)
	{
		memcpy((BYTE*)&eth_sock[numb],buf,8);
		size=(eth_sock[numb].len[0] << 8) | (eth_sock[numb].len[1]);
		memcpy((BYTE*)&eth_sock[numb].data,&buf[8],size);
	}
	switch(st_wr_w5500)
	{
		case 0:																	//read reg tx status
			addr_w5500=ADDR_SOC_TX_FREE_SIZE_0;						
			cb_w5500=SOCKET_REGISTER | SOCKET(numb);
			ptr_buf=(BYTE*)&chip.sockReg[numb].R014_Sn_TX_FSR_20_21;
			len_buf=6;
			cmd=READ_DATA;
			st_wr_w5500++;
		break;
		case 1:																	//write ip
			addr_w5500=ADDR_SOC_D_IP_ADDR0;
			cb_w5500=SOCKET_REGISTER | SOCKET(numb);
			ptr_buf=(BYTE*)&eth_sock[numb].ip_addr;
			len_buf=4;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case 2:																	//write port
			addr_w5500=ADDR_SOC_D_PORT0;
			cb_w5500=SOCKET_REGISTER | SOCKET(numb);
			ptr_buf=(BYTE*)&eth_sock[numb].port;
			len_buf=2;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case 3:																	//write data
			addr_w5500=eth_sock[numb].ptr_tx_buf;
			cb_w5500=SOCKET_TX_BUFFER | SOCKET(numb);
			ptr_buf=(BYTE*)&eth_sock[numb].data;
			len_buf=(eth_sock[numb].len[0] << 8) | (eth_sock[numb].len[1]);
			eth_sock[numb].ptr_tx_buf=eth_sock[numb].ptr_tx_buf+len_buf;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case 4:																	//write ptr
			wbuf_w55[cnt]=(eth_sock[numb].ptr_tx_buf>>8);		cnt++;
			wbuf_w55[cnt]=(BYTE)eth_sock[numb].ptr_tx_buf;		cnt++;
						
			addr_w5500=ADDR_SOC_TX_WRITE_PTR_0;
			cb_w5500=SOCKET_REGISTER | SOCKET(numb);
			ptr_buf=wbuf_w55;
			len_buf=cnt;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case 5:																	//write cmd SEND
			wbuf_w55[cnt]=CMD_SEND;				cnt++;
						
			addr_w5500=ADDR_SOC_COMMAND;
			cb_w5500=SOCKET_REGISTER | SOCKET(numb);
			ptr_buf=wbuf_w55;
			len_buf=cnt;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case 6:																//read status_SEND_OK
			addr_w5500=ADDR_SOC_INT;
			cb_w5500=SOCKET_REGISTER | SOCKET(numb);
			ptr_buf=(BYTE*)&chip.sockReg[numb].R03_Sn_IR_02;
			len_buf=1;
			cmd=READ_DATA;
			st_wr_w5500++;
		break;
		case 7:																//clear status SEND_OK
			if(chip.sockReg[numb].R03_Sn_IR_02.SEND_OK==1)
			{
				wbuf_w55[cnt]=SR_2C_SEND_OK;			cnt++;
				addr_w5500=ADDR_SOC_INT;
				cb_w5500=SOCKET_REGISTER | SOCKET(numb);
				ptr_buf=wbuf_w55;
				len_buf=cnt;
				cmd=WRITE_DATA;
				st_wr_w5500++;
				break;
			}
			st_wr_w5500--;
		break;
		case 8:
			st_wr_w5500=0;
			return 1;
		break;
	}
	return 0;
}

WORD w5500_cmd_read_socket_tcp (BYTE sock_numb, BYTE *buf)
{
	static BYTE st_cmd_w5500=0;
	static WORD size=0;
	static WORD ptrWR = 0;
	static WORD ptrRD = 0;
	BYTE cnt=0;
	switch(st_cmd_w5500)
	{
		case TCP_GIVE_LEN:
			addr_w5500=ADDR_SOC_RX_RECEIVED_SIZE_0;//addr
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);//bsb
			ptr_buf=(BYTE*)&chip.sockReg[sock_numb].R017_Sn_RX_RSR_26_27;//data
			len_buf=6;
			cmd=READ_DATA;//mode
			st_cmd_w5500 = TCP_PART_RD;//"next"
			size=0; 
		break;	
		case TCP_PART_RD:	
			ptrWR = (*(BYTE*)&chip.sockReg[sock_numb].R019_Sn_RX_WR_2A_2B.case1<<8) | (*(BYTE*)&chip.sockReg[sock_numb].R019_Sn_RX_WR_2A_2B.case2);//получить WR
			ptrRD = (*(BYTE*)&chip.sockReg[sock_numb].R018_Sn_RX_RD_28_29.case1<<8) | (*(BYTE*)&chip.sockReg[sock_numb].R018_Sn_RX_RD_28_29.case2);//получить RD
			if(ptrWR != ptrRD)//back & return
			{
				addr_w5500=eth_sock[sock_numb].ptr_rx_buf;//addr start messege
				cb_w5500=SOCKET_RX_BUFFER | SOCKET(sock_numb);//bsb sock RX
				ptr_buf=buf;
				len_buf = ptrWR - ptrRD;
				eth_sock[sock_numb].ptr_rx_buf=eth_sock[sock_numb].ptr_rx_buf+len_buf;
				if(len_buf>DEFAULT_MTU_TCP){size=2;/*min value '8'*/}else{size=len_buf;}							
				cmd=READ_DATA;
				st_cmd_w5500 = TCP_DROP_PTR;
				break;
			}
			st_cmd_w5500--;
			return 2;
		break;
		case TCP_DROP_PTR:
			wbuf_w55[0]=(ptrWR>>8);
			wbuf_w55[1]=(ptrWR);
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
			//if(0<sock_numb<5){counters.soket[sock_numb - 1].rx += 1;}
		break;
		case TCP_BK_START:
			st_cmd_w5500=0;//сброс параметров
			return (size);
		break;
	}
	return 0;
}

WORD w5500_write_socket_tcp (BYTE numb, BYTE *buf)
{
	static BYTE st_wr_w5500=0;
	static BYTE numb_static=0;
	WORD cnt=0;
	WORD size=0;
	
	if(numb_static!=numb){numb_static=numb;st_wr_w5500=0;}
	if(st_wr_w5500==0)
	{
		memcpy((BYTE*)&eth_sock[numb],buf,8);
		size=(eth_sock[numb].len[0] << 8) | (eth_sock[numb].len[1]);
		memcpy((BYTE*)&eth_sock[numb].data,&buf[8],size);
	}
	switch(st_wr_w5500)
	{
		case 0:																	//read reg tx status FSR
			addr_w5500=ADDR_SOC_TX_FREE_SIZE_0;
			cb_w5500=SOCKET_REGISTER | SOCKET(numb);
			ptr_buf=(BYTE*)&chip.sockReg[numb].R014_Sn_TX_FSR_20_21;
			len_buf=6;
			cmd=READ_DATA;
			st_wr_w5500 = 3;
		break;
		case 3:																	//write data
			addr_w5500=eth_sock[numb].ptr_tx_buf;
			cb_w5500=SOCKET_TX_BUFFER | SOCKET(numb);
			ptr_buf=(BYTE*)&eth_sock[numb].data;
			len_buf=(eth_sock[numb].len[0] << 8) | (eth_sock[numb].len[1]);
			eth_sock[numb].ptr_tx_buf=eth_sock[numb].ptr_tx_buf+len_buf;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case 4:																	//write ptr
			wbuf_w55[cnt]=(eth_sock[numb].ptr_tx_buf>>8);		cnt++;
			wbuf_w55[cnt]=(BYTE)eth_sock[numb].ptr_tx_buf;		cnt++;
			addr_w5500=ADDR_SOC_TX_WRITE_PTR_0;
			cb_w5500=SOCKET_REGISTER | SOCKET(numb);
			ptr_buf=wbuf_w55;
			len_buf=cnt;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case 5:																	//write cmd SEND
			wbuf_w55[cnt]=CMD_SEND;				cnt++;
			addr_w5500=ADDR_SOC_COMMAND;
			cb_w5500=SOCKET_REGISTER | SOCKET(numb);
			ptr_buf=wbuf_w55;
			len_buf=cnt;
			cmd=WRITE_DATA;
			st_wr_w5500++;
		break;
		case 6:																//read status_SEND_OK
			addr_w5500=ADDR_SOC_INT;
			cb_w5500=SOCKET_REGISTER | SOCKET(numb);
			ptr_buf=(BYTE*)&chip.sockReg[numb].R03_Sn_IR_02;
			len_buf=1;
			cmd=READ_DATA;
			st_wr_w5500++;
		break;
		case 7:																//clear status SEND_OK
			if(chip.sockReg[numb].R03_Sn_IR_02.SEND_OK==1)
			{
				wbuf_w55[cnt]=SR_2C_SEND_OK;			cnt++;
				addr_w5500=ADDR_SOC_INT;
				cb_w5500=SOCKET_REGISTER | SOCKET(numb);
				ptr_buf=wbuf_w55;
				len_buf=cnt;
				cmd=WRITE_DATA;
				st_wr_w5500++;
				//if(0<numb<5){counters.soket[numb - 1].tx += 1;}
				break;
			}
			st_wr_w5500--;
		break;
		case 8:
			st_wr_w5500=0;
			return 1;
		break;
	}
	return 0;
}

WORD w5500_ch_sock(BYTE sock_numb)
{
	static BYTE st_cmd_w5500=0;
	static WORD size=0;
	BYTE cnt=0;
	switch(st_cmd_w5500)
	{
		case 0://READ
			addr_w5500=ADDR_SOC_STATUS;//addr in w5500 
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);//bsb
			ptr_buf=(BYTE*)&chip.sockReg[sock_numb].R04_Sn_SR_03.Status;//place for write result
			len_buf=1;
			cmd=READ_DATA;//mode
			st_cmd_w5500 = 1;//"next"
			size=0; 
		break;
		case 1://FORK
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
				case ST_LISTEN:
					st_cmd_w5500=0;
				return 1;
				case ST_ESTABLISHED:
					st_cmd_w5500=0;
				return 1;
				default:
					st_cmd_w5500=0;
				return 1;
			}
			addr_w5500=ADDR_SOC_COMMAND;
			cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);
			ptr_buf=wbuf_w55;
			len_buf=1;
			cmd=WRITE_DATA;
			st_cmd_w5500 = 0;
		break;
	}
	return 0;
}

void w5500_give_stat(BYTE sock_numb)
{
	static BYTE st_cmd_w5500=0;
	static WORD size=0;
	BYTE cnt=0;
	switch(st_cmd_w5500)
	{
		case 0://READ
		addr_w5500=ADDR_SOC_STATUS;//addr in w5500
		cb_w5500=SOCKET_REGISTER | SOCKET(sock_numb);//bsb
		ptr_buf=(BYTE*)&chip.sockReg[sock_numb].R04_Sn_SR_03.Status;//place for write result
		len_buf=1;
		cmd=READ_DATA;//mode
		st_cmd_w5500 = 1;//"next"
		size=0;
		break;
	}
}