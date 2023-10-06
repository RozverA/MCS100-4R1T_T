#include "def.h"

#define CM2_WHO_ARE_YOU 0x01
#define UID_WHO_ARE_YOU 0x8001


#define CM2_STATUS_PACK 0x02
#define UID_STATUS_PACK 0x8002

void cmd_process(void)
{
	cmd_usart_process ();
}

void cmd_usart_process (void)
{
	static BYTE n_port=1;
	
	for(n_port=1;n_port<5;n_port++)
	{
		usart_process(n_port);
	}
	
	
}

void usart_process (BYTE n_port)
{
	WORD size=0;
	size = usart_read(n_port - 1, port[n_port-1].rbuf, USART_BUF_SIZE);   //give mess size
	if (size != 0)	{usart_write(n_port - 1, port[n_port-1].rbuf, size);}		
}