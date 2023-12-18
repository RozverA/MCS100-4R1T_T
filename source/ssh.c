#include "def.h"

SSH_STR ssh;
BYTE err_ssh_point = 0;
SSH_HEADER hdr;

BYTE ssh_process()
{
	WORD size = 0;
	DWORD ptr_cnt = 0;
	
	if (eth_sock[SSH_SOCK_VAL].r_status == 0)	{return;}
	
	switch(ssh.conn_st)
	{
		case SSH_INIT:
			ssh.conn_st = SSH_WAIT_PR;
		break;
		case SSH_WAIT_PR:
			ptr_cnt = sizeof(protocol_name);
			memcpy(eth_sock[SSH_SOCK_VAL].data, protocol_name, ptr_cnt);
			eth_sock[SSH_SOCK_VAL].data[ptr_cnt] = 10;			ptr_cnt++;
			eth_sock[SSH_SOCK_VAL].data[ptr_cnt] = 13;
			
			eth_sock[SSH_SOCK_VAL].len[0] = 0;
			eth_sock[SSH_SOCK_VAL].len[1] = ptr_cnt;
						
			WR_TGL(SSH_SOCK_VAL);
			
			ssh.conn_st = SSH_KEY_EXC_INIT;
		break;
		case SSH_KEY_EXC_INIT:
			KEX_init();
			WR_TGL(SSH_SOCK_VAL);
			
			ssh.conn_st = SSH_KEY_EXC_REPLY;
		break;
		case SSH_KEY_EXC_REPLY:
			KEX_reply();
			WR_TGL(SSH_SOCK_VAL);
			
			ssh.conn_st = SSH_NEW_KEY;
		break;
		
		
		
		default:
			ssh.conn_st = 109;
		break;
	}
}

void random_gen(BYTE size,BYTE* ptr)//size, place for write
{
	BYTE tc3 = 0;
	for (BYTE i = 0; i < size; i++) 	{*ptr++ = ((tc3_cnt + 3) ^ (i + 1)) % 0xFF;}
}
BYTE num_to_byte(DWORD num, BYTE len, BYTE* dst, BYTE side)//write number as bite line
{
	if((len != W_LEN)    &&  (len != DW_LEN))         {return LEN_ERR;}
	if((side != L_SIDE)  &&  (side != R_SIDE))     {return SIDE_ERR;}

	for (BYTE i = 0; i < len; i++)
	{
		if(side)    {*dst = num >> (( 8 * (len - 1 ) ) - ( i * 8 ));}
		else        {*dst = num >> ( i * 8 );}
		dst++;
	}
	return SUCCES;
}
DWORD num_aus_byte(BYTE len, BYTE* src, BYTE side)//read bite line as number
{
    DWORD num = 0;
    for (BYTE i = 0; i < len;i++)
    {
        if(side)    {num += *src << (((len - 1) * 8) - (i * 8));}
        else        {num += *src << ( i * 8 );}
        src++;
    }
    return num;
}

void KEX_init()//ok
{
	DWORD ptr_cnt =  SSH_HEADER_LEN + COOKIE_LEN;//21				//pointer for write messeeg
	DWORD str_len = 0;

	random_gen(16, &ssh.messege[SSH_HEADER_LEN]);//Cookie
	ptr_cnt = data_pack(ptr_cnt,KEX,sizeof(KEX), NOHEX);
	ptr_cnt = data_pack(ptr_cnt,SERV_HOST_KEY,sizeof(SERV_HOST_KEY), NOHEX);
	ptr_cnt = data_pack(ptr_cnt,ENCRYPTION_CLI_SRV,sizeof(ENCRYPTION_CLI_SRV), NOHEX);
	ptr_cnt = data_pack(ptr_cnt,ENCRYPTION_SRV_CLI,sizeof(ENCRYPTION_SRV_CLI), NOHEX);
	ptr_cnt = data_pack(ptr_cnt,MAC_CLI_SRV,sizeof(MAC_CLI_SRV), NOHEX);
	ptr_cnt = data_pack(ptr_cnt,MAC_SRV_CLI,sizeof(MAC_SRV_CLI), NOHEX);
	ptr_cnt = data_pack(ptr_cnt,COPRESS_CLI_SRV,sizeof(COPRESS_CLI_SRV), NOHEX);
	ptr_cnt = data_pack(ptr_cnt,COPRESS_SRV_CLI,sizeof(COPRESS_SRV_CLI), NOHEX);
//languages
	str_len = 0;
	ssh_len_w(str_len,&ssh.messege[ptr_cnt]);			ptr_cnt += DW_LEN;
	ssh_len_w(str_len,&ssh.messege[ptr_cnt]);			ptr_cnt += DW_LEN;
//fist KEX
	ssh.messege[ptr_cnt] = str_len;				ptr_cnt++;
//reserved
	str_len = 0;//4
	ssh_len_w(str_len,&ssh.messege[ptr_cnt]);			ptr_cnt += DW_LEN;
//padding
	BYTE padding_len = (ptr_cnt + 4) % 8;
	random_gen(padding_len, &ssh.messege[ptr_cnt]);			ptr_cnt += padding_len;
//len write	
	wr_header(ptr_cnt, KEY_EXCHANGE_INIT);
//eth sock len write
	ssh_eth_sock_upd();
}

void KEX_reply()
{
	if (eth_sock[SSH_SOCK_VAL].data[5] != DH_KEY_EXCHANGE_INIT)	{return ERROR;}
//define
	DWORD key_size;
	BYTE cli_key[256];
	DWORD str_len;
	DWORD ptr_cnt = SSH_HEADER_LEN + DW_LEN;
	DWORD tmp;
	static BYTE N[] = {0xA1,0xD4,0x6F,0xBA,0x23,0x18,0xF8,0xDC,0xEF,0x16,0xC2,0x80,0x94,0x8B,0x1C,0xF2,0x79,0x66,0xB9,0xB4,0x72,0x25,0xED,0x29,0x89,0xF8,0xD7,0x4B,0x45,0xBD,0x36,0x04,0x9C,0x0A,0xAB,0x5A,0xD0,0xFF,0x00,0x35,0x53,0xBA,0x84,0x3C,0x8E,0x12,0x78,0x2F,0xC5,0x87,0x3B,0xB8,0x9A,0x3D,0xC8,0x4B,0x88,0x3D,0x25,0x66,0x6C,0xD2,0x2B,0xF3,0xAC,0xD5,0xB6,0x75,0x96,0x9F,0x8B,0xEB,0xFB,0xCA,0xC9,0x3F,0xDD,0x92,0x7C,0x74,0x42,0xB1,0x78,0xB1,0x0D,0x1D,0xFF,0x93,0x98,0xE5,0x23,0x16,0xAA,0xE0,0xAF,0x74,0xE5,0x94,0x65,0x0B,0xDC,0x3C,0x67,0x02,0x41,0xD4,0x18,0x68,0x45,0x93,0xCD,0xA1,0xA7,0xB9,0xDC,0x4F,0x20,0xD2,0xFD,0xC6,0xF6,0x63,0x44,0x07,0x40,0x03,0xE2,0x11};
	static BYTE F[] = {0x34,0xf6,0x98,0x8e,0x97,0x21,0x33,0xeb,0x9b,0xd4,0xae,0x3e,0xf7,0x38,0xbf,0x69,0x2c,0x08,0xda,0xa2,0x18,0x63,0x4f,0x27,0xf8,0x61,0x2c,0xdf,0x7f,0x63,0xfd,0xcc,0x02,0xfa,0x2d,0x47,0x4a,0x1f,0xa7,0xb5,0xb6,0x0e,0x61,0x0c,0x2d,0x6d,0x0f,0x4b,0xb2,0xe4,0x39,0x73,0x07,0x11,0x36,0x6c,0xda,0x69,0x47,0xff,0x2e,0xe1,0x24,0x07,0x0e,0x7d,0xe4,0x48,0x6b,0x9e,0x31,0x9b,0x4d,0xe5,0x95,0x32,0x93,0x9e,0xfd,0x1f,0x10,0xc2,0x17,0x3f,0x4f,0x8e,0xf7,0xfc,0xbd,0xdc,0x57,0x38,0xe3,0x1a,0xa6,0x1a,0xd5,0x4b,0x1f,0x79,0x23,0xe1,0xb5,0x4b,0x48,0x06,0xa2,0xd3,0x7b,0x6b,0x88,0x3e,0xe7,0xf3,0x7f,0x4c,0x5f,0x4a,0x9c,0xb9,0x04,0xaa,0x6b,0xdf,0x2a,0xa1,0x31,0xed};
	static BYTE HSD[] = {0x00,0x00,0x01,0x80,0x52,0x96,0xf9,0x86,0x59,0xac,0x41,0xa3,0x9f,0x58,0x56,0xfb,0xdc,0x56,0x2f,0x11,0xf7,0x4e,0x63,0x0b,0x7d,0xa4,0x5e,0x07,0xf5,0xc6,0x4b,0xb8,0x31,0x2f,0xe1,0xeb,0xea,0x5c,0x74,0x0b,0x80,0xdd,0xce,0x44,0xd4,0x55,0xd6,0x5e,0x21,0x14,0xcf,0x3a,0xb6,0xdb,0x7a,0x8e,0x7e,0x9c,0x1c,0xa8,0xec,0xa4,0xb2,0xae,0x5b,0xf4,0x22,0xec,0xf5,0x59,0x79,0x56,0x8d,0x0d,0x4a,0xbb,0xd6,0xe8,0xe6,0xfb,0xbc,0xb7,0xdd,0x60,0x22,0x60,0x1b,0x6d,0x51,0xa8,0x63,0x20,0x03,0xea,0x59,0x81,0xb5,0x09,0x7d,0xab,0x07,0xd3,0xc9,0xb0,0x68,0x96,0x38,0x57,0x08,0xd7,0xe7,0x5f,0xc2,0x97,0xee,0xad,0xf5,0xeb,0x89,0x08,0xdb,0x8b,0xc7,0xc1,0x24,0xca,0xe5,0xf6,0x61,0x06,0xba,0xe6,0xcb,0x19,0xfc,0xb7,0xac,0x38,0xa6,0x5e,0xad,0xcf,0x3b,0x06,0xed,0x4e,0x6b,0x4a,0x88,0x23,0x86,0xdc,0xe4,0x2e,0x9a,0xf3,0x85,0x53,0xa3,0x3a,0x0e,0x1c,0xda,0xce,0x66,0xbb,0x1b,0x41,0xd1,0xf8,0x3c,0xfd,0xde,0xeb,0x3f,0x3b,0xeb,0x92,0xe2,0x83,0x75,0xa9,0x77,0x2f,0x4a,0xf1,0x35,0x1f,0x53,0x76,0xd7,0x27,0xb7,0xd3,0xd6,0x06,0x58,0x35,0x86,0xbe,0x0c,0x2c,0x4a,0xbf,0x5b,0x73,0x94,0x90,0xa7,0xf2,0x32,0xc6,0xe7,0xe3,0x5b,0x6f,0x7e,0xdb,0xb9,0x31,0xdf,0x5a,0xf0,0x1a,0x02,0x61,0x56,0xc4,0xb0,0xc6,0x93,0x43,0xb9,0x38,0x74,0x0e,0xc8,0x7a,0x02,0xbc,0x55,0xde,0xcb,0x2f,0x86,0x72,0x8d,0x10,0xfc,0x51,0xd4,0x65,0x61,0xe1,0xb7,0xa4,0x0f,0x52,0x22,0xda,0xa5,0x09,0xa9,0x31,0xe1,0xf8,0xc8,0xe6,0xdd,0x77,0x2b,0x03,0x3e,0x03,0x01,0x80,0x59,0x18,0x70,0x43,0x7e,0xd1,0xf5,0x9b,0x5e,0x15,0x59,0xd9,0xe0,0x3a,0x14,0x0e,0x76,0x8c,0xc1,0xe1,0xdc,0x78,0xd7,0x43,0x93,0x5d,0x59,0x45,0x50,0x39,0xbc,0xe7,0x66,0x36,0x17,0x25,0xc8,0x2b,0xa7,0x6a,0xe9,0xcb,0xf3,0x54,0xe7,0xfe,0x1d,0xf9,0xed,0xed,0x7f,0x3c,0xbc,0x4e,0xe5,0x83,0x0b,0x13,0x32,0x52,0xe4,0x64,0xf6,0x37,0xeb,0x00,0xc2,0x74,0xbb,0x88,0x6e,0xa2,0x13,0x29,0x57,0x47,0xd4,0xd3,0x0f,0x66,0x40,0x06,0x53,0xd2,0xe6,0x7a,0xd0,0x09,0xee,0x93,0x21,0x9a,0x85,0x09,0x6e,0x68,0x95,0xb6,0x32,0x1a,0xcf,0x4b,0x7d,0x55,0x38,0xd1,0x76,0xa8,0x0a,0x49,0x72,0x74};
//read messege
	key_size = num_aus_byte(DW_LEN, &eth_sock[SSH_SOCK_VAL].data[6],L_SIDE);
	if (key_size > 256)	{return;}
	memcpy( &cli_key[0], &eth_sock[SSH_SOCK_VAL].data[10], key_size);	
	
//KEX exchange	 SPESIAL TABULATION
	//kex host key
		BYTE kex_len;
		ptr_cnt = data_pack(ptr_cnt, SERV_HOST_KEY, sizeof(SERV_HOST_KEY), NOHEX);//7-RSA
		tmp = E;
		ptr_cnt = data_pack(ptr_cnt, &tmp, sizeof(E), NOHEX); //6 - "010001"
		ptr_cnt = data_pack(ptr_cnt, N, sizeof(N), HEX);
		tmp = ptr_cnt - 10;
		ssh_len_w(tmp,&ssh.messege[6]);
	ptr_cnt = data_pack(ptr_cnt, F, sizeof(F), HEX);
	//kex host signature
		ssh.messege[ptr_cnt] = 388 + sizeof(SERV_HOST_KEY);					ptr_cnt += DW_LEN;
		ptr_cnt = data_pack(ptr_cnt,SERV_HOST_KEY, sizeof(SERV_HOST_KEY), NOHEX);
		
		//place for host signature data
		BYTE padding_len = (ptr_cnt + 4) % 8;
		random_gen(padding_len, &ssh.messege[ptr_cnt]);						ptr_cnt += padding_len;
	
	


//padding
	wr_header(ptr_cnt, DH_KEY_EXCHANGE_REPLY);
	ssh_eth_sock_upd();
}

void wr_header(DWORD ptr_cnt, BYTE mess_code)
{
	ssh_len_w(ptr_cnt - 4, &hdr.pack_len);
	hdr.padding_len = SSH_PADDING_LEN;
	hdr.messege_code = mess_code;
	memcpy(&ssh.messege[0],&hdr.pack_len,sizeof(SSH_HEADER));
}
DWORD data_pack(DWORD ptr_cnt, BYTE* dst, DWORD len, BYTE offset)
{
	len -= offset;
	ssh_len_w(len, &ssh.messege[ptr_cnt]);										ptr_cnt += DW_LEN;
	memcpy(&ssh.messege[ptr_cnt], dst, len);										ptr_cnt += len;
	return ptr_cnt;
}
