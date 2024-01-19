#ifndef ALG_H_
#define ALG_H_

#define B_LEN			1
#define W_LEN			2
#define DW_LEN_BIT		(W_LEN * 8)
#define DW_LEN			4
#define DW_LEN_BIT		(DW_LEN * 8)

#define L_SIDE			1 //1234 - 1.2.3.4
#define R_SIDE			0 //1234 - 4.3.2.1

#define LEN_ERR         101
#define SIDE_ERR        102

#define OK	1

#define HEX	0
#define NOHEX 1

	extern DWORD num_aus_byte(BYTE len, BYTE* src, BYTE side);
	extern void random_gen(BYTE size,BYTE* ptr);
	extern BYTE num_to_byte(DWORD num, BYTE len, BYTE* dst, BYTE side);
	extern BYTE SRAV(BYTE len, BYTE* buf1, BYTE* buf2);

#endif