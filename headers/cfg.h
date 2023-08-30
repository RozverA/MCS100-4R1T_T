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
typedef struct
//-----------------------------------------------------------------------------
{
	BYTE  en;			//1 //on/off
	BYTE  mode;			//2	//TCP_GATE/UDP_GATE/TCP_IP/UDP_IP
	
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
	DWORD				time;			//8
	COMMON_CFG			com_network;	//16+8 24
	SOCKET_RS485_CFG	sock_rs485[4];	//12*4	48+24 72	
	WORD				in_numb;		//74						//<<HOZ>> number
	BYTE				reserv[180];				//256-76 180
	WORD				cs; 			//76	
}CFG_1;
#pragma pack()

#pragma pack(1)
typedef struct             // size 256 bytes
//-----------------------------------------------------------------------------
{
	BYTE   mac_addr[6];			//6
	WORD   len_serial;			//8
	BYTE   serial[16];			//24
	BYTE   date_of_creat[4];	//28
	WORD   len_info;			//30
	char   info[224];						//256-32  224
	WORD  cs;					//32
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

#define VERSION "0.0.0.01"		//8 b
#define MODEL	"_MCS100-4R1T"	//12 b

#endif /* CFG_H_ */