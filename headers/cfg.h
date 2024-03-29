#ifndef CFG_H_
#define CFG_H_

//define
//status	
	#define CFG_OK  1
	#define CFG_ERR 0
//parameters
	#define VERSION "0.0.0.04"		//8 b logins and logs
	#define VERSION_B 0x0004		//8 b logins and logs
	#define MODEL	"_MCS100-4R1T"	//12 b
//funx
	#define cfg_drop() {cfg_default();cfg_save();}



#pragma pack(1)
typedef struct
//-----------------------------------------------------------------------------
{
	WORD  type_ip;      //2  
	WORD  src_port;		//4
	BYTE  ip_addr[4];	//8
	BYTE  ip_mask[4];	//12
	BYTE  ip_gate[4];	//16

}COMMON_CFG;
#pragma pack()

#pragma pack(1)
typedef struct
//-----------------------------------------------------------------------------
{
	BYTE  en;			//1 //on/off
	BYTE  mode;			//2	//TCP/UDP
	BYTE  pl;			//2	//GATE/MBUS
	
	WORD  src_port;		//4	//my port
	DWORD baud;			//8	
	BYTE  bsize :4;		
	BYTE  parity:2;
	BYTE  stop  :2;		//9
	BYTE        :8;		//10

	WORD  tout;			//12 //timeout to port
}SOCKET_RS485_CFG;
#pragma pack()

#pragma pack(1)
typedef struct
{
  BYTE  en;
  BYTE reserve;
  DWORD ip[4];
}access_rs485;
#pragma pack()

#pragma pack(1)
typedef struct             // size 256 bytes
//-----------------------------------------------------------------------------
{
	DWORD				guid;			//4       		  
	char				name[32];		//32		
	COMMON_CFG			com_network;	//16		
	SOCKET_RS485_CFG	sock_rs485[4];	//12*4(112)		
	WORD				in_numb;		//2			//<<HOZ>> number
	access_rs485		access[4];
	BYTE				reserv[76];					
	WORD				cs; 			//2		
}CFG_1;
#pragma pack()

typedef struct
{
	char data_day[2];
	char data_mount[2];
	char data_year[4];
}SERVICE_DATE;

#pragma pack(1)
typedef struct             // size 256 bytes
//-----------------------------------------------------------------------------
{
	BYTE	mac_addr[6];
	char	serial[32];
	char	date_of_creat[8];
	char	version_boot[32];
	char	info[176];
	WORD	cs;
}CFG_2;
#pragma pack()

//extern
////struct
	extern CFG_1 cfg_1;
	extern CFG_1 cfg_1_tmp;
	extern CFG_2 cfg_2;     //no change cfg
////funx
	extern void cfg_init(void);
	extern WORD cfg_read(void);
	extern void cfg_default(void);
	extern WORD cfg_save(void);

void cfg_check();

#endif /* CFG_H_ */