#include "def.h"

SSH_STR ssh;
BYTE err_ssh_point = 0;

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
			write_KEX_init();
			WR_TGL(SSH_SOCK_VAL);
			ssh.conn_st = SSH_KEY_EXC_REPLY;
		break;
		case SSH_KEY_EXC_REPLY:
			write_KEX_init();
			WR_TGL(SSH_SOCK_VAL);
			ssh.conn_st = 109;
		break;
		
		
		default:
			ssh.conn_st = 109;
		break;
	}
	
}

void random_gen(BYTE size,BYTE* ptr)
{
	BYTE tc3 = 0;
	for (BYTE i = 0; i < size; i++) 	{*ptr++ = ((tc3_cnt + 3) ^ (i + 1)) % 0xFF;}
}

DWORD len_write(DWORD* ptr_cnt, DWORD len)
{
	for (BYTE i = 0; i < 4; i++)	{ssh.messege[*ptr_cnt] = len >> (24 - (i * 8)); *ptr_cnt += 1;}
	return (*ptr_cnt);
}

BYTE num_to_byte(DWORD num, BYTE len, BYTE* dst, BYTE side) //
{
	if((len != W_LEN)    &&  (len != DW_LEN))         {return LEN_ERR;}
	if((side != L_SIDE)  &&  (side != R_SIDE))     {return SIDE_ERR;}

	for (BYTE i = 0; i < len;i++)
	{
		if(side)    {*dst = num >> (( 8 * (len - 1 ) ) - ( i * 8 ));}
		else        {*dst = num >> ( i * 8 );}
		dst++;
	}
	return SUCCES;
}

void write_KEX_init()
{
	DWORD ptr_cnt = 0;
	DWORD str_len = 0;
	
	ssh.messege[5] = 20;										//Message Code: Key Exchange init
	random_gen(16, &ssh.messege[SSH_PADDING_LEN+1]);			//Cookie
	ptr_cnt = SSH_PADDING_LEN + COOKIE_LEN + 1;//21				//pointer for write messege
	
//key exchange
	str_len = sizeof(KEX) - 1;
	if( num_to_byte(str_len, DW_LEN, &ssh.messege[ptr_cnt], L_SIDE) ){err_ssh_point = 1; return;}		ptr_cnt += DW_LEN;
	memcpy(&ssh.messege[ptr_cnt],KEX,str_len);															ptr_cnt += str_len;

//server host key alg
	str_len = sizeof(SERV_HOST_KEY) - 1;
	if( num_to_byte(str_len, DW_LEN, &ssh.messege[ptr_cnt], L_SIDE) ){err_ssh_point = 2; return;}		ptr_cnt += DW_LEN;
	memcpy(&ssh.messege[ptr_cnt],SERV_HOST_KEY,str_len);												ptr_cnt += str_len;

//encryption alg cl to serv
	str_len = sizeof(ENCRYPTION_CLI_SRV) - 1;
	if( num_to_byte(str_len, DW_LEN, &ssh.messege[ptr_cnt], L_SIDE) ){err_ssh_point = 3; return;}		ptr_cnt += DW_LEN;
	memcpy(&ssh.messege[ptr_cnt],ENCRYPTION_CLI_SRV,str_len);											ptr_cnt += str_len;

//encryption alg serv to cl
	str_len = sizeof(ENCRYPTION_SRV_CLI) - 1;
	if( num_to_byte(str_len, DW_LEN, &ssh.messege[ptr_cnt], L_SIDE) ){err_ssh_point = 4; return;}		ptr_cnt += DW_LEN;
	memcpy(&ssh.messege[ptr_cnt],ENCRYPTION_SRV_CLI,str_len);											ptr_cnt += str_len;

//mac alg cli to serv
	str_len = sizeof(MAC_CLI_SRV) - 1;
	if( num_to_byte(str_len, DW_LEN, &ssh.messege[ptr_cnt], L_SIDE) ){err_ssh_point = 5; return;}		ptr_cnt += DW_LEN;
	memcpy(&ssh.messege[ptr_cnt],MAC_CLI_SRV,str_len);													ptr_cnt += str_len;

//mac alg serv to cli
	str_len = sizeof(MAC_SRV_CLI) - 1;
	if( num_to_byte(str_len, DW_LEN, &ssh.messege[ptr_cnt], L_SIDE) ){err_ssh_point = 6; return;}		ptr_cnt += DW_LEN;
	memcpy(&ssh.messege[ptr_cnt],MAC_SRV_CLI,str_len);													ptr_cnt += str_len;

//compression alg cli to serv
	str_len = sizeof(COPRESS_CLI_SRV) - 1;
	if( num_to_byte(str_len, DW_LEN, &ssh.messege[ptr_cnt], L_SIDE) ){err_ssh_point = 7; return;}		ptr_cnt += DW_LEN;
	memcpy(&ssh.messege[ptr_cnt],COPRESS_CLI_SRV,str_len);												ptr_cnt += str_len;

//compression alg serv to cli
	str_len = sizeof(COPRESS_SRV_CLI) - 1;
	if( num_to_byte(str_len, DW_LEN, &ssh.messege[ptr_cnt], L_SIDE) ){err_ssh_point = 8; return;}		ptr_cnt += DW_LEN;
	memcpy(&ssh.messege[ptr_cnt],COPRESS_SRV_CLI,str_len);												ptr_cnt += str_len;

//languages
	str_len = 0;
	if( num_to_byte(str_len, DW_LEN, &ssh.messege[ptr_cnt], L_SIDE) ){err_ssh_point = 9; return;}		ptr_cnt += DW_LEN;
	if( num_to_byte(str_len, DW_LEN, &ssh.messege[ptr_cnt], L_SIDE) ){err_ssh_point = 10; return;}		ptr_cnt += DW_LEN;
	ptr_cnt = len_write(&ptr_cnt, str_len); //cli to ser
	ptr_cnt = len_write(&ptr_cnt, str_len); //serv to cli

//fist KEX
	ssh.messege[ptr_cnt] = str_len;										ptr_cnt++; 

//reserved
	str_len = 0;//4
	if( num_to_byte(str_len, DW_LEN, &ssh.messege[ptr_cnt], L_SIDE) ){err_ssh_point = 1; return;}		ptr_cnt += DW_LEN;
	ptr_cnt = len_write(&ptr_cnt, str_len);

//padding
	random_gen(5, &ssh.messege[ptr_cnt]);								ptr_cnt += SSH_PADDING_LEN;//padding string	

//len write	
	for (BYTE i = 0; i < 4; i++)	{ssh.messege[i] = (ptr_cnt - 4) >> (24 - (i * 8));}

//padding
	ssh.messege[4] = SSH_PADDING_LEN;
	
//eth sock len write
	eth_sock[SSH_SOCK_VAL].len[0] = ptr_cnt >> 8;
	eth_sock[SSH_SOCK_VAL].len[1] = ptr_cnt;
	//copy messege aus buffer
	memcpy(&eth_sock[SSH_SOCK_VAL].data[0], &ssh.messege[0], ptr_cnt);
}

void write_KEX_reply()
{
	//parse
	//forming messege
	
	
}