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
	WORD  type_ip;        
	WORD  src_port;   
	BYTE  mac_addr[6];
	BYTE  ip_addr[4]; 
	BYTE  ip_mask[4]; 
	BYTE  ip_gate[4]; 

}COMMON_CFG;
#pragma pack()

#pragma pack(1)
typedef struct
//-----------------------------------------------------------------------------
{
	BYTE  en;         
	BYTE  mode;       
	
	WORD  src_port;   
	DWORD baud;   
	BYTE  bsize :4;
	BYTE  parity:2;
	BYTE  stop  :2;
	BYTE        :8;

	WORD  tout;    
}SOCKET_RS485_CFG;
#pragma pack()



#pragma pack(1)
typedef struct             // size 512 bytes
//-----------------------------------------------------------------------------
{
	DWORD  guid;           		  
					
	DWORD serial_n_H;	//serial number
	DWORD serial_n_L;	//serial number
	
	WORD in_numb;		//хоз номер

	COMMON_CFG com_network;
	SOCKET_RS485_CFG sock_rs485[4];
	BYTE  reserv[426];    
	WORD  cs; 					
}CFG;
#pragma pack()

extern CFG cfg;
extern CFG cfg_tmp;

extern void cfg_init(void);
extern WORD cfg_read(void);
extern void cfg_default(void);
extern WORD cfg_save(void);
#define cfg_drop() {cfg_default();cfg_save();}
void cfg_check();

#define VERSION "0.0.0.01"		//8 b
#define MODEL	"_MCS100-4R1T"	//12 b

#endif /* CFG_H_ */