#ifndef ENCOD_K_H_
#define ENCOD_K_H_

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned int   DWORD;

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern void crypted(char* str);
extern void decrypted(char* str);

#endif /* ENCOD_K_H_ */
