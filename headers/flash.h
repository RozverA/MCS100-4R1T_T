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
		#define ACCOUNT_PLACE	(DWORD)0x0003FB00
		//CFG
		#define CFG_1_ADDR		(DWORD)0x0003F000
		#define CFG_2_ADDR		(DWORD)0x0003F100
		//LOGs
		#define LOGS_ADDR		(DWORD)0x0003F300
//typedefs
	////
	#pragma pack(1)
	typedef struct { BYTE indx; BYTE operat_code; BYTE rsv[2];}CELL_INFO;
	#pragma pack()
	
	#pragma pack(1)
	typedef struct {DWORD ip; DWORD times; CELL_INFO inform;}LOG_CELL;
	#pragma pack()

	#pragma pack(1)
	typedef struct {BYTE ptr; BYTE rsv[7]; LOG_CELL cell[170];}LOG_DATA;
	#pragma pack()
	////accounts
	#pragma pack(1)
	typedef struct {char  login[32]; char password[32];}ACCOUNT;
	#pragma pack()

	#pragma pack(1)
	typedef struct {ACCOUNT accnt[2]; BYTE rsv[126]; WORD cs;}ACCOUNTS;
	#pragma pack()
	
#endif /* FLASH_H_ */