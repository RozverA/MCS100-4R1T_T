#include "def.h"

BYTE iap_page[256];
BYTE iap_buf[256];

DWORD iap_addr =  0;
WORD  iap_size =  0;



DWORD iap_key=0;

WORD iap_process(BYTE *buf, WORD size)
//--------------------------------------------------------------------------
{
	DWORD page;
	WORD  wn = 0;
	memcpy(iap_buf,buf,size);
	static BYTE flwr=0;
	
	switch(iap_buf[wn])
	{
		case CM2_R_START_BOOTLOADER :									for(page = IAP_COPY;page<(IAP_COPY+IAP_SIZE);page+=256)
		{
			wdt_reset();
			if(!flash_empty(page,256)){flash_erase_page(page);}
		}
		iap_buf[wn] = CM2_A_BOOTLOADER_READY;		wn += sizeof(BYTE );
		
		break;
		case CM2_R_LOAD_DATA_PAGE   :									iap_buf[wn] = CM2_A_DATA_PAGE_LOAD;			wn += sizeof(BYTE );
			iap_size  =iap_buf[wn] | (iap_buf[wn+1]<<8);		wn += sizeof(WORD );
			iap_addr  =iap_buf[wn] | (iap_buf[wn+1]<<8) | (iap_buf[wn+2]<<16) | (iap_buf[wn+3]<<24);
			wn += sizeof(DWORD);
		
			memcpy(iap_page,iap_buf+wn,iap_size);		wn += iap_size;
		
			flwr=1;
		
		break;
		case CM2_R_UPDATE_DATA_PAGE :  									if(iap_addr < IAP_ADDR			 )	{return;}
			if(iap_addr > (IAP_ADDR+IAP_SIZE))	{return;}
		
			if(flwr)
			{
				flash_write(iap_addr+IAP_SIZE,iap_page,iap_size);
				flwr=0;
			}
			iap_buf[wn] = CM2_A_DATA_PAGE_UPDATE;		wn += sizeof(BYTE );
		break;
		case CM2_R_STOP_BOOTLOADER  :  									iap_key = IAP_VAL;
		
		if(!flash_empty(IAP_KEY,256)){flash_erase_page(IAP_KEY);}
		flash_write(IAP_KEY,&iap_key,4);
		
		iap_buf[wn] = CM2_A_BOOTLOADER_STOP;		wn += sizeof(BYTE );
		reset = TRUE;
		break;
	}
	
	memcpy(buf,iap_buf,wn);
	return	(wn);
}




BYTE* get_word (BYTE *src,WORD *val)
//-----------------------------------------------------------------------------
{
	*val =(*((BYTE*)src)) << 0; src+=sizeof(BYTE);
	*val+=(*((BYTE*)src)) << 8; src+=sizeof(BYTE);
	return(src);
}

BYTE* get_dword(BYTE *src,DWORD *val)
//-----------------------------------------------------------------------------
{
	*val =(*((BYTE*)src)) << 0;  src+=sizeof(BYTE);
	*val+=(*((BYTE*)src)) << 8;  src+=sizeof(BYTE);
	*val+=(*((BYTE*)src)) << 16; src+=sizeof(BYTE);
	*val+=(*((BYTE*)src)) << 24; src+=sizeof(BYTE);
	return(src);
}