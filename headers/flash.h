#ifndef FLASH_H_
#define FLASH_H_
//defs
	////login
	#define ADMIN				"admin"
	#define USER				"user"
	#define CHIRIT				"CHIRIT"
	#define ACC1				1
	#define ACC2				2
	#define ACC3				3
	////place in flash
		#define ACCOUNT_PLACE	(DWORD)0x0003F300
		//CFG
		#define CFG_1_ADDR		(DWORD)0x0003F000
		#define CFG_2_ADDR		(DWORD)0x0003F100
		//LOGs
		#define LOGS_ADDR		(DWORD)0x0002C000
		#define CELLS			256
		#define CELL			16
		#define BLOCK			256
//typedefs
	////

	#pragma pack(1)
	typedef struct { DWORD ip; DWORD times; DWORD indx; WORD operat_code; WORD version;}LOG_CELL;//(4;4;2;2;4)16
	#pragma pack()

	#pragma pack(1)
	typedef struct { LOG_CELL cell[CELLS];}LOG_DATA;//(1;2;13;(16*127)2032)2048
	#pragma pack()
	////accounts
	#pragma pack(1)
	typedef struct {char  login[32]; char password[32];}ACCOUNT;
	#pragma pack()

	#pragma pack(1)
	typedef struct {ACCOUNT accnt[2]; BYTE rsv[126]; WORD cs;}ACCOUNTS;
	#pragma pack()
	
#endif /* FLASH_H_ */