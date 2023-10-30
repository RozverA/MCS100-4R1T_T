#ifndef SSH_H_
#define SSH_H_

#define SSH_INIT			0
#define SSH_WAIT_PR			(SSH_INIT + 1)
#define SSH_KEY_EXC_INIT	(SSH_WAIT_PR + 1)
#define SSH_KEY_EXC_REPLY	(SSH_KEY_EXC_INIT + 1)
#define SSH_NEW_KEY			(SSH_KEY_EXC_REPLY + 1)

//algoritms
	#define protocol_name		"SSH-2.0-MCS100-4R1T"

	#define KEX					"curve25519-sha256,curve25519-sha256@libssh.org,diffie-hellman-group14-sha256,diffie-hellman-group14-sha1,diffie-hellman-group1-sha1,diffie-hellman-group16-sha512"
	#define SERV_HOST_KEY		"ecdsa-sha2-nistp256,ecdsa-sha2-nistp384,ecdsa-sha2-nistp521,ssh-rsa"
	#define ENCRYPTION_CLI_SRV	"aes128-cbc,3des-cbc,aes128-ctr,aes256-ctr,aes256-cbc,3des-ctr"
	#define ENCRYPTION_SRV_CLI	"aes128-cbc,3des-cbc,aes128-ctr,aes256-ctr,aes256-cbc,3des-ctr"
	#define MAC_CLI_SRV			"hmac-sha1"
	#define MAC_SRV_CLI			"hmac-sha1"
	#define COPRESS_CLI_SRV		"none"
	#define COPRESS_SRV_CLI		"none"
	//no used
	#define LANG_CLI_SRV		""
	#define LANG_SRV_CLI		""
	
//mod
	#define B_LEN        1
	#define W_LEN        2
	#define DW_LEN       4

	#define L_SIDE       1 //1234 - 1.2.3.4
	#define R_SIDE      0 //1234 - 4.3.2.1

	#define LEN_ERR         101
	#define SIDE_ERR        102

	#define SUCCES          0

	extern BYTE num_to_byte(DWORD num, BYTE len, BYTE* dst, BYTE side);
//mod end


#define		COOKIE_LEN			16
#define		SSH_PADDING_LEN		5
#define		SSH_STR_LEN			1

extern BYTE ssh_process();
extern void write_KEX_init();

typedef struct {
	BYTE conn_st;
	BYTE messege[512];
}SSH_STR;

extern SSH_STR ssh;

#endif