#ifndef SOCKET_H_
#define SOCKET_H_

extern func_st socket_init (BYTE numb, WORD src_port, BYTE mode);
//TCP_GATE/UDP_GATE/TCP_IP/UDP_IP
#define UDP		1
#define TCP		2

#define MBUS	2
#define GATE	1

#endif