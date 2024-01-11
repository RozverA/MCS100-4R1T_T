#ifndef IAP_H_
#define IAP_H_

//define
	//IAP
		#define IAP_ADDR  (DWORD)0x00004000
		#define IAP_COPY  (DWORD)0x00018000
		#define IAP_SIZE  (DWORD)0x00014000  //80 KB
		#define IAP_KEY   (DWORD)0x0003F200	 	
		#define IAP_VAL   (DWORD)0x43FF56AC
	
	//CM2
		#define CM2_R_START_BOOTLOADER     0x00
		#define CM2_A_BOOTLOADER_START     0x01
		#define CM2_A_BOOTLOADER_READY     0x02
		#define CM2_R_LOAD_DATA_PAGE       0x03
		#define CM2_A_DATA_PAGE_LOAD       0x04
		#define CM2_R_UPDATE_DATA_PAGE     0x05
		#define CM2_A_DATA_PAGE_UPDATE     0x06
		#define CM2_R_STOP_BOOTLOADER      0x07
		#define CM2_A_BOOTLOADER_STOP      0x08
//extern
	//variables
		extern DWORD iap_key;
		extern DWORD iap_addr;
		extern WORD  iap_size;
	//funx
		extern WORD iap_process(BYTE *iap_buf, WORD size);

#endif /* IAP_H_ */