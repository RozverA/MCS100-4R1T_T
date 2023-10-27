#include "def.h"

SSH_STR ssh;

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

DWORD len_write(DWORD* ptr_cnt, DWORD len)
{
	for (BYTE i = 0; i < 4; i++)	{ssh.messege[*ptr_cnt] = len >> (24 - (i * 8)); *ptr_cnt += 1;}
	return (*ptr_cnt);
}

BYTE write_key_exchange()
{
	DWORD ptr_cnt = 0;
	DWORD str_len = 0;
	
	ssh.messege[5] = 20;										//Message Code: Key Exchange init
	random_gen(16, &ssh.messege[SSH_PADDING_LEN+1]);			//Cookie
	ptr_cnt = SSH_PADDING_LEN + COOKIE_LEN + 1;//21				//pointer for write messege
	
	//key exchange
	
	str_len = sizeof(KEX) - 1;	
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],KEX,str_len);						ptr_cnt += str_len;
	
	//server host key alg
	str_len = sizeof(SERV_HOST_KEY) - 1;	
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],SERV_HOST_KEY,str_len);			ptr_cnt += str_len;
	
	//encryption alg cl to serv
	str_len = sizeof(ENCRYPTION_CLI_SRV) - 1;	
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],ENCRYPTION_CLI_SRV,str_len);		ptr_cnt += str_len;
	
	//encryption alg serv to cl
	str_len = sizeof(ENCRYPTION_SRV_CLI) - 1;	
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],ENCRYPTION_SRV_CLI,str_len);		ptr_cnt += str_len;
	
	//mac alg cli to serv
	str_len = sizeof(MAC_CLI_SRV) - 1;	
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],MAC_CLI_SRV,str_len);				ptr_cnt += str_len;
	
	//mac alg serv to cli
	str_len = sizeof(MAC_SRV_CLI) - 1;	
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],MAC_SRV_CLI,str_len);				ptr_cnt += str_len;
	
	//compression alg cli to serv
	str_len = sizeof(COPRESS_CLI_SRV) - 1;	
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],COPRESS_CLI_SRV,str_len);			ptr_cnt += str_len;
	
	//compression alg serv to cli
	str_len = sizeof(COPRESS_SRV_CLI) - 1;	
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],COPRESS_SRV_CLI,str_len);			ptr_cnt += str_len;
	
	//languages
	str_len = 0;
	ptr_cnt = len_write(&ptr_cnt, str_len); //cli to ser
	ptr_cnt = len_write(&ptr_cnt, str_len); //serv to cli
	
	ssh.messege[ptr_cnt] = str_len;									ptr_cnt++; //fist KEX
	
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


BYTE write_key_exchange_mimik()
{
	DWORD ptr_cnt = 0;
	DWORD str_len = 0;
	
	ssh.messege[5] = 20;										//Message Code: Key Exchange init
	random_gen(16, &ssh.messege[SSH_PADDING_LEN+1]);			//Cookie
	ptr_cnt = SSH_PADDING_LEN + COOKIE_LEN + 1;//21				//pointer for write messege
	
	//key exchange
	
	str_len = sizeof(KEX) - 1;
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],KEX,str_len);						ptr_cnt += str_len;
	
	//server host key alg
	str_len = sizeof(SERV_HOST_KEY) - 1;
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],SERV_HOST_KEY,str_len);			ptr_cnt += str_len;
	
	//encryption alg cl to serv
	str_len = sizeof(ENCRYPTION_CLI_SRV) - 1;
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],ENCRYPTION_CLI_SRV,str_len);		ptr_cnt += str_len;
	
	//encryption alg serv to cl
	str_len = sizeof(ENCRYPTION_SRV_CLI) - 1;
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],ENCRYPTION_SRV_CLI,str_len);		ptr_cnt += str_len;
	
	//mac alg cli to serv
	str_len = sizeof(MAC_CLI_SRV) - 1;
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],MAC_CLI_SRV,str_len);				ptr_cnt += str_len;
	
	//mac alg serv to cli
	str_len = sizeof(MAC_SRV_CLI) - 1;
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],MAC_SRV_CLI,str_len);				ptr_cnt += str_len;
	
	//compression alg cli to serv
	str_len = sizeof(COPRESS_CLI_SRV) - 1;
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],COPRESS_CLI_SRV,str_len);			ptr_cnt += str_len;
	
	//compression alg serv to cli
	str_len = sizeof(COPRESS_SRV_CLI) - 1;
	ptr_cnt = len_write(&ptr_cnt, str_len);
	memcpy(&ssh.messege[ptr_cnt],COPRESS_SRV_CLI,str_len);			ptr_cnt += str_len;
	
	//languages
	str_len = 0;
	ptr_cnt = len_write(&ptr_cnt, str_len); //cli to ser
	ptr_cnt = len_write(&ptr_cnt, str_len); //serv to cli
	
	ssh.messege[ptr_cnt] = str_len;									ptr_cnt++; //fist KEX
	
	//reserved
	str_len = 0;//4
	ptr_cnt = len_write(&ptr_cnt, str_len);
	
	//padding
	ssh.messege[ptr_cnt] = 0xd7;									ptr_cnt++;
	ssh.messege[ptr_cnt] = 0x98;									ptr_cnt++;
	ssh.messege[ptr_cnt] = 0x21;									ptr_cnt++;
	ssh.messege[ptr_cnt] = 0xad;									ptr_cnt++;
	ssh.messege[ptr_cnt] = 0xfc;									ptr_cnt++;
	
	
	for (BYTE i = 0; i < 4; i++)	{ssh.messege[i] = (ptr_cnt - 4) >> (24 - (i * 8));}
	
	ssh.messege[4] = SSH_PADDING_LEN;
	eth_sock[SSH_SOCK_VAL].len[0] = ptr_cnt >> 8;
	eth_sock[SSH_SOCK_VAL].len[1] = ptr_cnt;
	
	memcpy(&eth_sock[SSH_SOCK_VAL].data[0], &ssh.messege[0], ptr_cnt);
}
