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