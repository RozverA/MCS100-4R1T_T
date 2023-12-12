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
	DWORD ptr_cnt = 0;
	DWORD str_len = 0;

	random_gen(16, &ssh.messege[SSH_HEADER_LEN]);			//Cookie
	ptr_cnt = SSH_HEADER_LEN + COOKIE_LEN;//21				//pointer for write messeeg
	
//key exchange
	ptr_cnt = data_pack(ptr_cnt,KEX,sizeof(KEX));

//server host key alg
	ptr_cnt = data_pack(ptr_cnt,SERV_HOST_KEY,sizeof(SERV_HOST_KEY));

//encryption alg cl to serv
	ptr_cnt = data_pack(ptr_cnt,ENCRYPTION_CLI_SRV,sizeof(ENCRYPTION_CLI_SRV));

//encryption alg serv to cl
	ptr_cnt = data_pack(ptr_cnt,ENCRYPTION_SRV_CLI,sizeof(ENCRYPTION_SRV_CLI));

//mac alg cli to serv
	ptr_cnt = data_pack(ptr_cnt,MAC_CLI_SRV,sizeof(MAC_CLI_SRV));

//mac alg serv to cli
	ptr_cnt = data_pack(ptr_cnt,MAC_SRV_CLI,sizeof(MAC_SRV_CLI));

//compression alg cli to serv
	ptr_cnt = data_pack(ptr_cnt,COPRESS_CLI_SRV,sizeof(COPRESS_CLI_SRV));

//compression alg serv to cli
	ptr_cnt = data_pack(ptr_cnt,COPRESS_SRV_CLI,sizeof(COPRESS_SRV_CLI));

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
	random_gen(padding_len, &ssh.messege[ptr_cnt]);			ptr_cnt += padding_len;//padding string	

//len write	
	wr_header(ptr_cnt, KEY_EXCHANGE_INIT);

//eth sock len write
	eth_sock[SSH_SOCK_VAL].len[0] = ptr_cnt >> 8;
	eth_sock[SSH_SOCK_VAL].len[1] = ptr_cnt;
	//copy messege aus buffer
	memcpy(&eth_sock[SSH_SOCK_VAL].data[0], &ssh.messege[0], ptr_cnt);
}

void KEX_reply()
{
	if (eth_sock[SSH_SOCK_VAL].data[5] != DH_KEY_EXCHANGE_INIT)	{return ERROR;}
//define
	BYTE cli_mess_len;
	BYTE key_size;
	BYTE cli_key[128];
	DWORD str_len;
	DWORD ptr_cnt = SSH_HEADER_LEN;
//read messege
	cli_mess_len = num_aus_byte(DW_LEN, &ssh.messege[0], L_SIDE);
	key_size = num_aus_byte(DW_LEN, &eth_sock[SSH_SOCK_VAL].data[6],L_SIDE);
	//copy public key from socket buf and read key len(num aus bute)
	BYTE test  =  num_aus_byte( DW_LEN, eth_sock[SSH_SOCK_VAL].data[6], L_SIDE);
	memcpy( &cli_key[0], &eth_sock[SSH_SOCK_VAL].data[10], test);	

//KEX exchange	
	BYTE kex_len;
	ptr_cnt = data_pack(ptr_cnt, SERV_HOST_KEY, sizeof(SERV_HOST_KEY));//7-RSA
	BYTE a = E;
	ptr_cnt = data_pack(ptr_cnt, &a, sizeof(E)); //6 - "010001"
	//ptr_cnt = data_pack()
	

	
	
	

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

DWORD data_pack(DWORD ptr_cnt, BYTE* dst, DWORD len)
{
	len -= 1;
	ssh_len_w(len, &ssh.messege[ptr_cnt]);										ptr_cnt += DW_LEN;
	memcpy(&ssh.messege[ptr_cnt], dst, len);										ptr_cnt += len;
	return ptr_cnt;
}
