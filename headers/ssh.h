#ifndef SSH_H_
#define SSH_H_

#define SSH_INIT			0
#define SSH_WAIT_PR			(SSH_INIT + 1)
#define SSH_KEY_EXC_INIT	(SSH_WAIT_PR + 1)
#define SSH_KEY_EXC_REPLY	(SSH_KEY_EXC_INIT + 1)
#define SSH_NEW_KEY			(SSH_KEY_EXC_REPLY + 1)
#define SSH_STABLE			(SSH_NEW_KEY + 1)

//algoritms
	#define protocol_name		"SSH-2.0-SMTech_rz109"

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
	#define KEY_EXCHANGE_INIT		20//14
	#define DH_KEY_EXCHANGE_INIT	30//1E
	#define DH_KEY_EXCHANGE_REPLY   31//1F
	//rsa
	#define RSA "rsa"
	//#define RSA_KEY
	
	#define E 0x01000100
	//#define D "589552BB4F2F023ADDDD5586D0C8FD857512D82080436678D07F984A29D892D31F1F7000FC5A39A0F73E27D885E47249A4148C8A5653EF69F91F8F736BA9F84841C2D99CD8C24DE8B72B5C9BE0EDBE23F93D731749FEA9CFB4A48DD2B7F35A2703E74AA2D4DB7DE9CEEA7D763AF0ADA7AC176C4E9A22C4CDA65CEC0C65964401"
	//#define P "CD083568D2D46C44C40C1FA0101AF2155E59C70B08423112AF0C1202514BBA5210765E29FF13036F56C7495894D80CF8C3BAEE2839BACBB0B86F6A2965F60DB1"
	//#define Q "CA0EEEA5E710E8E9811A6B846399420E3AE4A4C16647E426DDF8BBBCB11CD3F35CE2E4B6BCAD07AE2C0EC2ECBFCC601B207CDD77B5673E16382B1130BF465261"
	//#define DP "0D0E21C07BF434B4A83B116472C2147A11D8EB98A33CFBBCF1D275EF19D815941622435AAF3839B6C432CA53CE9E772CFBE1923A937A766FD93E96E6EDEC1DF1"
	//#define DQ "269CEBE6305DFEE4809377F078C814E37B45AE6677114DFC4F76F5097E1F3031D592567AC55B9B98213B40ECD54A4D2361F5FAACA1B1F51F71E4690893C4F081"
	//#define QP "97AC5BB885ABCA314375E9E4DB1BA4B2218C90619F61BD474F5785075ECA81750A735199A8C191FE2D3355E7CF601A70E5CABDE0E02C2538BB9FB4871540B3C1"
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
	
	#define HEX	0
	#define NOHEX 1
	
	#define SUCCES          0

	
	#define ssh_len_w(num, dst)		(num_to_byte(num, DW_LEN, dst, L_SIDE))
	#define ssh_len_r(src)			(num_aus_byte(DW_LEN, src,L_SIDE))
	#define sock1()					(eth_sock[SSH_SOCK_VAL].len[0] = ptr_cnt >> 8)
	#define sock2()					(eth_sock[SSH_SOCK_VAL].len[1] = ptr_cnt)
	#define ssh_eth_sock_upd() (sock1(), sock2(), memcpy(&eth_sock[SSH_SOCK_VAL].data[0], &ssh.messege[0], ptr_cnt))
	
	extern DWORD num_aus_byte(BYTE len, BYTE* src, BYTE side);
	extern void random_gen(BYTE size,BYTE* ptr);
	extern BYTE num_to_byte(DWORD num, BYTE len, BYTE* dst, BYTE side);
	
//mod end
#define		COOKIE_LEN			16
#define		SSH_PADDING_LEN		5
#define		SSH_STR_LEN			1
#define		SSH_HEADER_LEN		6

extern BYTE ssh_process();
extern void KEX_init();
extern DWORD data_pack(DWORD ptr_cnt, BYTE* dst, DWORD len, BYTE offset);

typedef struct {
	BYTE conn_st;
	BYTE messege[512];
}SSH_STR;

typedef struct {
	DWORD	pack_len;//packet len
	BYTE	padding_len;
	BYTE	messege_code;
}SSH_HEADER;

extern SSH_STR ssh;
extern SSH_HEADER;

#endif