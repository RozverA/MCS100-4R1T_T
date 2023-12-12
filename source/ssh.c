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
			
			ssh.conn_st = 109;
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
	BYTE tmp;
	static BYTE N[] = {0xA1,0xD4,0x6F,0xBA,0x23,0x18,0xF8,0xDC,0xEF,0x16,0xC2,0x80,0x94,0x8B,0x1C,0xF2,0x79,0x66,0xB9,0xB4,0x72,0x25,0xED,0x29,0x89,0xF8,0xD7,0x4B,0x45,0xBD,0x36,0x04,0x9C,0x0A,0xAB,0x5A,0xD0,0xFF,0x00,0x35,0x53,0xBA,0x84,0x3C,0x8E,0x12,0x78,0x2F,0xC5,0x87,0x3B,0xB8,0x9A,0x3D,0xC8,0x4B,0x88,0x3D,0x25,0x66,0x6C,0xD2,0x2B,0xF3,0xAC,0xD5,0xB6,0x75,0x96,0x9F,0x8B,0xEB,0xFB,0xCA,0xC9,0x3F,0xDD,0x92,0x7C,0x74,0x42,0xB1,0x78,0xB1,0x0D,0x1D,0xFF,0x93,0x98,0xE5,0x23,0x16,0xAA,0xE0,0xAF,0x74,0xE5,0x94,0x65,0x0B,0xDC,0x3C,0x67,0x02,0x41,0xD4,0x18,0x68,0x45,0x93,0xCD,0xA1,0xA7,0xB9,0xDC,0x4F,0x20,0xD2,0xFD,0xC6,0xF6,0x63,0x44,0x07,0x40,0x03,0xE2,0x11};
//read messege
	key_size = num_aus_byte(DW_LEN, &eth_sock[SSH_SOCK_VAL].data[6],L_SIDE);
	if (key_size > 256)	{return;}
	memcpy( &cli_key[0], &eth_sock[SSH_SOCK_VAL].data[10], key_size);	
//KEX exchange	
	//kex host key
		BYTE kex_len;
		ptr_cnt = data_pack(ptr_cnt, SERV_HOST_KEY, sizeof(SERV_HOST_KEY), NOHEX);//7-RSA
		tmp = E;
		ptr_cnt = data_pack(ptr_cnt, &tmp, sizeof(E), NOHEX); //6 - "010001"
		ptr_cnt = data_pack(ptr_cnt, N, sizeof(N), HEX);
		tmp = ptr_cnt - 6;
		ssh_len_w(tmp,ssh.messege[6]);
	//kex host signature
		ptr_cnt += DW_LEN;
		ptr_cnt = data_pack(ptr_cnt,SERV_HOST_KEY, sizeof(SERV_HOST_KEY), NOHEX);
		//place for host signature data
	
	


//padding
	wr_header(ptr_cnt, DH_KEY_EXCHANGE_REPLY);
	eth_sock[SSH_SOCK_VAL].len[0] = ptr_cnt >> 8;
	eth_sock[SSH_SOCK_VAL].len[1] = ptr_cnt;
	memcpy(&eth_sock[SSH_SOCK_VAL].data[0], &ssh.messege[0], ptr_cnt);
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
