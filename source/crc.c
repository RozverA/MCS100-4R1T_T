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
	static const uint16_t table[2] = { 0x0000, 0xA001 };
	uint16_t crc = 0xFFFF;
	unsigned int i = 0;
	char bit = 0;
	unsigned int xor = 0;

	for( i = 0; i < size; i++ )
	{
		crc ^= buf[i];

		for( bit = 0; bit < 8; bit++ )
		{
			xor = crc & 0x01;
			crc >>= 1;
			crc ^= table[xor];
		}
	}

	return crc;
}