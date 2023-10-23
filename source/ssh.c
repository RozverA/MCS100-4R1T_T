#include "def.h"

SSH_STR ssh;

BYTE ssh_process()
{
	WORD size = 0;
	
	if (eth_sock[SSH_SOCK_VAL].r_status == 0)	{return;}
	
	switch(ssh.conn_st)
	{
		case SSH_INIT:
			ssh.conn_st = SSH_WAIT_PR;
			break;
			case SSH_WAIT_PR:
			memcpy(eth_sock[SSH_SOCK_VAL].data, "SSH-2.0-MCS100-4R1T", 19);
			eth_sock[SSH_SOCK_VAL].data[20] = 10;
			eth_sock[SSH_SOCK_VAL].data[21] = 13;
			eth_sock[SSH_SOCK_VAL].len[0] = 0;
			eth_sock[SSH_SOCK_VAL].len[1] = 22;
			eth_sock[SSH_SOCK_VAL].w_status = 1;
			eth_sock[SSH_SOCK_VAL].r_status = 0;
			
			ssh.conn_st = SSH_KEY_EXC_INIT;
		break;
		case SSH_KEY_EXC_INIT:
			write_key_exchange();
			eth_sock[SSH_SOCK_VAL].w_status = 1;
			eth_sock[SSH_SOCK_VAL].r_status = 0;
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

DWORD len_write(BYTE* ptr_cnt, BYTE len)
{
	for (BYTE i = 0; i < 4; i++)	{ssh.messege[*ptr_cnt] = len >> (24 - (i * 8)); *ptr_cnt += 1;}
	return (*ptr_cnt);
}

BYTE write_key_exchange()
{
	DWORD ptr_cnt = 0;
	BYTE str_len = 0;
	ssh.messege[5] = 20;										//Message Code: Key Exchange init
	random_gen(16, &ssh.messege[SSH_PADDING_LEN+1]);			//Cookie
	ptr_cnt = SSH_PADDING_LEN + COOKIE_LEN + 1;//21					//pointer for write messege
	
	//key exchange
	str_len = 28;	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],"curve25519-sha256@libssh.org",str_len);	ptr_cnt += str_len;
	
	//server host key alg
	str_len = 7;	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],"ssh-rsa",str_len);		ptr_cnt += str_len;
	
	//encryption alg cl to serv
	str_len = 10;	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],"aes256-ctr",str_len);		ptr_cnt += str_len;
	
	//encryption alg serv to cl
	str_len = 10;	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],"aes256-ctr",str_len);		ptr_cnt += str_len;
	
	//mac alg cli to serv
	str_len = 9;	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],"hmac-sha1",str_len);		ptr_cnt += str_len;
	
	//mac alg serv to cli
	str_len = 9;	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],"hmac-sha1",str_len);		ptr_cnt += str_len;
	
	//compression alg cli to serv
	str_len = 4;	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],"none",str_len);			ptr_cnt += str_len;
	
	//compression alg serv to cli
	str_len = 4;	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],"none",str_len);			ptr_cnt += str_len;
	
	//languages
	str_len = 0;
	ptr_cnt = len_write(&ptr_cnt, str_len); //cli to ser
	ptr_cnt = len_write(&ptr_cnt, str_len); //serv to cli
	
	ssh.messege[ptr_cnt] = str_len;							ptr_cnt++; //fist KEX
	
	//reserved
	str_len = 0;//4
	ptr_cnt = len_write(&ptr_cnt, str_len);
	
	//padding
	random_gen(5, &ssh.messege[ptr_cnt]);					ptr_cnt += SSH_PADDING_LEN;//padding string	

	for (BYTE i = 0; i < 4; i++)	{ssh.messege[i] = (ptr_cnt - 4) >> (24 - (i * 8));}
		
	ssh.messege[4] = SSH_PADDING_LEN;
	eth_sock[SSH_SOCK_VAL].len[0] = ptr_cnt >> 8;
	eth_sock[SSH_SOCK_VAL].len[1] = ptr_cnt;
	
	memcpy(&eth_sock[SSH_SOCK_VAL].data[0], &ssh.messege[0], ptr_cnt);
}
