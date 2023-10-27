#ifndef SSH_H_
#define SSH_H_

#define SSH_INIT			0
#define SSH_WAIT_PR			(SSH_INIT + 1)
#define SSH_KEY_EXC_INIT	(SSH_WAIT_PR + 1)
#define SSH_ESTABLISH		(SSH_KEY_EXC_INIT + 1)
#define SSH_BACK			(SSH_ESTABLISH + 1)

//algoritms
//#define short_var	1
#define protocol_name			"SSH-2.0-MCS100-4R1T"

#define long_var				2
#ifdef short_var
	#define KEX					"curve25519-sha256"
	#define SERV_HOST_KEY		"ecdsa-sha2-nistp256"
	#define ENCRYPTION_CLI_SRV	"aes128-cbc"
	#define ENCRYPTION_SRV_CLI	"aes128-cbc"
	#define MAC_CLI_SRV			"hmac-sha1"
	#define MAC_SRV_CLI			"hmac-sha1"
	#define COPRESS_CLI_SRV		"none"
	#define COPRESS_SRV_CLI		"none"
#endif
#ifdef long_var
	#define KEX					"curve25519-sha256,curve25519-sha256@libssh.org,diffie-hellman-group14-sha256,diffie-hellman-group14-sha1,diffie-hellman-group1-sha1,diffie-hellman-group16-sha512"
	#define SERV_HOST_KEY		"ecdsa-sha2-nistp256,ecdsa-sha2-nistp384,ecdsa-sha2-nistp521,ssh-rsa"
	#define ENCRYPTION_CLI_SRV	"aes128-cbc,3des-cbc,aes128-ctr,aes256-ctr,aes256-cbc,3des-ctr"
	#define ENCRYPTION_SRV_CLI	"aes128-cbc,3des-cbc,aes128-ctr,aes256-ctr,aes256-cbc,3des-ctr"
	#define MAC_CLI_SRV			"hmac-sha1"
	#define MAC_SRV_CLI			"hmac-sha1"
	#define COPRESS_CLI_SRV		"none"
	#define COPRESS_SRV_CLI		"none"
#endif
	//no used
	#define LANG_CLI_SRV		""
	#define LANG_SRV_CLI		""


#define COOKIE_LEN			16
#define SSH_PADDING_LEN		5

#define SSH_STR_LEN			1

extern BYTE ssh_process();
extern BYTE write_key_exchange();
extern BYTE write_key_exchange_mimik();

typedef struct {
	BYTE conn_st;
	BYTE messege[512];
}SSH_STR;

extern SSH_STR ssh;

#endif