#ifndef CFG_H_
#define CFG_H_

#define CFG_OK  1
#define CFG_ERR 0

#define CHECK 0
#define READ_PROCESS 1
#define WRITE_PROCESS 2
#define TCP_SOCK_PROCESS 3


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
typedef struct {BYTE  ip[4];}IP_FOR_LIST; 
#pragma pack()

#pragma pack(1)
typedef struct {BYTE  mac[6]; }MAC_FOR_LIST;
#pragma pack()

#pragma pack(1)
typedef struct
//-----------------------------------------------------------------------------
{
	IP_FOR_LIST  ip[5];      //20
	MAC_FOR_LIST  mac[5];    //30
}ACCESS_LIST;
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
typedef struct             // size 256 bytes
//-----------------------------------------------------------------------------
{
	DWORD				guid;			//4       		  
	char				name[32];		//32	36
	COMMON_CFG			com_network;	//16	52
	SOCKET_RS485_CFG	sock_rs485[4];	//12*4	100	
	WORD				in_numb;		//2		102		//<<HOZ>> number
	ACCESS_LIST			access_l;		//50	152
	BYTE				reserv[102];					//256-154 102
	WORD				cs; 			//2		154
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

extern CFG_2 cfg_2;     //no change cfg
extern BYTE	cfg_2_err;	//error counter


extern CFG_1 cfg_1;
extern CFG_1 cfg_1_tmp;

extern void cfg_init(void);
extern WORD cfg_read(void);
extern void cfg_default(void);
extern WORD cfg_save(void);
#define cfg_drop() {cfg_default();cfg_save();}
void cfg_check();

#define VERSION "0.0.0.03"		//8 b
#define MODEL	"_MCS100-4R1T"	//12 b

#endif /* CFG_H_ */