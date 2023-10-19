#ifndef SSH_H_
#define SSH_H_

#define SSH_INIT			0
#define SSH_WAIT_PR			(SSH_INIT + 1)
#define SSH_KEY_EXC_INIT	(SSH_WAIT_PR + 1)
#define SSH_ESTABLISH		(SSH_KEY_EXC_INIT + 1)
#define SSH_BACK			(SSH_ESTABLISH + 1)

#define COOKIE_LEN			16
#define SSH_PADDING_LEN		5

#define SSH_STR_LEN			1

extern BYTE ssh_process();
extern BYTE write_key_exchange();

typedef struct {
	BYTE conn_st;
	BYTE messege[512];
}SSH_STR;

extern SSH_STR ssh;

#endif