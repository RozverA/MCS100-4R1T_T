#include "def.h"



WORD flash_empty(DWORD addr,WORD size)
{
	DWORD  v    = 0;
	for(;size;addr+=4,size-=4)
	{
		v = *(__IO DWORD*)(addr);
		if(v != 0xFFFFFFFF)
		{
			return(0);
		}
	}


	return(1);
}



void flash_erase_page(DWORD addr)
//-----------------------------------------------------------------------------
{
	while (NVMCTRL->INTFLAG.bit.READY == 0);//wait
	NVMCTRL->STATUS.reg = NVMCTRL_STATUS_MASK;
	*(__IO DWORD *)(addr)=0xFFFFFFFF;//dummy data to automatically set ADDR register
	//NVMCTRL->ADDR.reg = (uint32_t)dst>>2;
	NVMCTRL->CTRLA.reg=NVMCTRL_CTRLA_CMDEX_KEY|NVMCTRL_CTRLA_CMD_ER;
	while (NVMCTRL->INTFLAG.bit.READY == 0);//wait
}



void flash_write(DWORD addr,BYTE *buf,WORD size)
//-----------------------------------------------------------------------------
{
	WORD wn   = 0;
	DWORD len =0;
	DWORD cnt =0;
	
	NVMCTRL->CTRLB.bit.MANW=0;

	while(size > 0)
	{
		len=(64 >> 2) < size ? (64 >> 2) : size;
		
		size-=len;
		
		wdt_reset();
		
		NVMCTRL->CTRLA.reg=NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_PBC;
		while(NVMCTRL->INTFLAG.bit.READY == 0) { ; }

		for(wn=0;wn<len;wn+=4,addr+=4)
		{
			*(__IO DWORD*)(addr) = *(DWORD*)&buf[wn+cnt];
		}
		
		cnt=cnt+len;
		NVMCTRL->CTRLA.reg=NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_WP;
		while (NVMCTRL->INTFLAG.bit.READY == 0) { ; }
	}
}



WORD flash_copy(DWORD dst,DWORD src,DWORD size)
//--------------------------------------------------------------------------
{
	WORD wn   = 0;
	DWORD len =0;
	DWORD cnt =0;
	
	NVMCTRL->CTRLB.bit.MANW=0;

	while(size > 0)
	{
		len=(64 >> 2) < size ? (64 >> 2) : size;
		
		size-=len;
		wdt_reset();
		NVMCTRL->CTRLA.reg=NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_PBC;
		while(NVMCTRL->INTFLAG.bit.READY == 0) { ; }

		for(wn=0;wn<len;wn+=4,dst+=4,src+=4)
		{
			
			*(__IO DWORD*)(dst) = *(__IO DWORD*)(src);
		}
		
		cnt=cnt+len;
		NVMCTRL->CTRLA.reg=NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_WP;
		while (NVMCTRL->INTFLAG.bit.READY == 0) { ; }
	}
	return 0;
}


WORD flash_read(DWORD addr,void* buf,WORD size)
//--------------------------------------------------------------------------
{
	WORD  cnt  = 0;
	WORD* ptr = (WORD*)buf;
	
	for(cnt=0;cnt<size;addr+=2,cnt+=2)
	{
		*ptr = *(__IO uint16_t*)(addr);
		ptr++;
		wdt_reset();
	}
	return(cnt);
}

