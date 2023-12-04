#ifndef SSH_H_
#define SSH_H_

#define SSH_INIT			0
#define SSH_WAIT_PR			(SSH_INIT + 1)
#define SSH_KEY_EXC_INIT	(SSH_WAIT_PR + 1)
#define SSH_KEY_EXC_REPLY	(SSH_KEY_EXC_INIT + 1)
#define SSH_NEW_KEY			(SSH_KEY_EXC_REPLY + 1)

//algoritms
	#define protocol_name		"SSH-2.0-MCS100-4R1T"

	#define KEX					"diffie-hellman-group1-sha1"
	#define SERV_HOST_KEY		"ssh-rsa"
	#define ENCRYPTION_CLI_SRV	"3des-cbc"
	#define ENCRYPTION_SRV_CLI	"3des-cbc"
	#define MAC_CLI_SRV			"hmac-sha1"
	#define MAC_SRV_CLI			"hmac-sha1"
	#define COPRESS_CLI_SRV		"none"
	#define COPRESS_SRV_CLI		"none"
	//no used
	#define LANG_CLI_SRV		""
	#define LANG_SRV_CLI		""
	//KEX code
	#define KEY_EXCHANGE_INIT		0x14
	#define DH_KEY_EXCHANGE_INIT	0x1E
	#define DH_KEY_EXCHANGE_REPLY   0x1F
//mod
	#define B_LEN			1
	#define W_LEN			2
	#define DW_LEN_BIT		(W_LEN * 8)
	#define DW_LEN			4
	#define DW_LEN_BIT		(DW_LEN * 8)

	#define L_SIDE			1 //1234 - 1.2.3.4
	#define R_SIDE			0 //1234 - 4.3.2.1

	#define LEN_ERR         101
	#define SIDE_ERR        102

	#define SUCCES          0

	
	#define ssh_len_w(num, dst)		(num_to_byte(num, DW_LEN, dst, L_SIDE))
	#define ssh_len_r(src)			(num_aus_byte(DW_LEN, src,L_SIDE))
	
	extern DWORD num_aus_byte(BYTE len, BYTE* src, BYTE side);
	extern void random_gen(BYTE size,BYTE* ptr);
	extern BYTE num_to_byte(DWORD num, BYTE len, BYTE* dst, BYTE side);
	
//mod end
#define		COOKIE_LEN			16
#define		SSH_PADDING_LEN		5
#define		SSH_STR_LEN			1

extern BYTE ssh_process();
extern void KEX_init();

typedef struct {
	BYTE conn_st;
	BYTE messege[512];
}SSH_STR;

extern SSH_STR ssh;

#endif