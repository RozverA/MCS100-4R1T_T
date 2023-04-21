#include "def.h"




func_st socket_init (BYTE numb, WORD src_port, BYTE mode)
{
	BYTE buf[2];
	
	//mode
	if(mode==UDP_MODE){buf[0]=SR_00_P2;}
	if(mode==TCP_MODE){buf[0]=SR_00_P1;}	
		
	spi_write_timeout(ADDR_SOC_MODE,(SOCKET_REGISTER | SOCKET(numb)),buf,1,10);
	
	//src_port
	buf[0]=((src_port & 0xFF00)>>8);
	buf[1]=( src_port & 0x00FF);
	spi_write_timeout(ADDR_SOC_SRC_PORT0,(SOCKET_REGISTER | SOCKET(numb)),buf,2,10);
	
	
	//socket_0_open
	buf[0]=CMD_OPEN;
	spi_write_timeout(ADDR_SOC_COMMAND,(SOCKET_REGISTER | SOCKET(numb)),buf,1,10);
	
	if (mode == TCP_MODE)
		{
			//socket_listen
			buf[0]=CMD_LISTEN;
			spi_write_timeout(ADDR_SOC_COMMAND,(SOCKET_REGISTER | SOCKET(numb)),buf,1,10);
	 	}
		
	return SUCCESS;
	
} 