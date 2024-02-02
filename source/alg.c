#include "def.h"


BYTE num_to_byte(DWORD num, BYTE len, BYTE* dst, BYTE side)//write number as bite line
{
	BYTE i = 0;
	if((len != W_LEN)    &&  (len != DW_LEN))         {return LEN_ERR;}
	if((side != L_SIDE)  &&  (side != R_SIDE))     {return SIDE_ERR;}

	for (i = 0; i < len; i++)
	{
		if(side)    {*dst = num >> (( 8 * (len - 1 ) ) - ( i * 8 ));}
		else        {*dst = num >> ( i * 8 );}
		dst++;
	}
	return SUCCESS;
}
DWORD num_aus_byte(BYTE len, BYTE* src, BYTE side)//read bite line as number
{
	DWORD num = 0;
	BYTE i = 0;
	
	for (i = 0; i < len;i++)
	{
		if(side)    {num += *src << (((len - 1) * 8) - (i * 8));}
		else        {num += *src << ( i * 8 );}
		src++;
	}
	return num;
}

BYTE SRAV(BYTE len, BYTE* buf1, BYTE* buf2)//BUF1 = BUF2 ? True-1,F-0;
{	
	BYTE i = 0;
	for(i = 0; i < len; i++)	
	{	
		if(*buf1++ != *buf2++) 		{return FALSE;}	
	}
	return TRUE;	
}
