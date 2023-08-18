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