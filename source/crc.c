#include "def.h"

WORD crc16_ccit(BYTE *buf,WORD size)
{
	WORD crc = 0xFFFF;
	BYTE i;

	while (size--)
	{
		crc ^= *buf++ << 8;

		for (i = 0; i < 8; i++)
		crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
	}
	return ((crc>>8) | (crc<<8));
}

WORD crc16_mbus( BYTE *buf, WORD size )
{
	 WORD n;
	 uint8_t i;
	 WORD crc = 0xFFFF;
	 for ( n = 0; n < size; n++ )
	 {
		 crc ^= buf[n];
		 for ( i = 8; i != 0; i-- )
		 {
			 if ( crc & 0x0001 )
			 {
				 crc >>= 1;
				 crc ^= 0xA001;
			 }
			 else
			 {
				 crc >>= 1;
			 }
		 }
	 }
	 return crc;
}

uint_least32_t crc32(unsigned char *buf, size_t len)
{
	uint_least32_t crc_table[256];
	uint_least32_t crc; int i, j;
	
	for (i = 0; i < 256; i++)
	{
		crc = i;
		for (j = 0; j < 8; j++)
		crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;

		crc_table[i] = crc;
	};

	crc = 0xFFFFFFFFUL;
	
	while (len--)
	{
		if(!(len % 32768)) {wdt_reset();}
		crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
	}
	
	return crc ^ 0xFFFFFFFFUL;
}