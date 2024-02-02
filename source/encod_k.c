#include "def.h"

#define CRYPTED 	1
#define DECRYPTED 	0

void crypted(char* str)
{
	move(str, CRYPTED);
	wave(str, CRYPTED);
	shell_game(str,CRYPTED);
}

void decrypted(char* str)
{
	shell_game(str,DECRYPTED);
	wave(str, DECRYPTED);
	move(str, DECRYPTED);
}

void wave(char* ptr, char mode)
{
	char plus;
	char sign = -1;
	char i=0;
	char step=0;
	
	
	if (mode == CRYPTED) {sign = 1;}
	for ( i= 0; i < 64; i++)
	{
		step = i % 2;
		plus = ((63 - i) / 2) * sign;
		if (step) { *ptr -= plus; } else {*ptr += plus;}
		ptr++;
	}
}

void shell_game(char* ptr, char mode)
{
	char buf[64];
	char lap = 0;
	char cursour = 0;
	char* debug = ptr;
	BYTE j = 0;
	BYTE i = 0;
	
	memcpy(buf, ptr, 64);
	if (mode == CRYPTED) {mode = 3;} else {mode = 1;}
		
	for (j = 0; j < mode; j++)
	{
		for (i = 0; i < 63 ; i++)
		{
			if (cursour > 63)	{lap++; cursour = lap;}
			*ptr = buf[cursour];
			ptr++;
			cursour += 8;
		}
		lap = 0;
		cursour = 0;
		ptr = debug;
		memcpy(buf, ptr, 64);
	}
}

void move(char* ptr, char mode)
{
	char sign = - 1;
	BYTE i = 0;
	
	
	if (mode == CRYPTED){sign = 1;}
	for (i = 0; i < 63; i++)
	{
		*ptr += 68 * sign;
	}	
}