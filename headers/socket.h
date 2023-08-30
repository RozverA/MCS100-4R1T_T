#ifndef SOCKET_H_
#define SOCKET_H_

extern func_st socket_init (BYTE numb, WORD src_port, BYTE mode);
//TCP_GATE/UDP_GATE/TCP_IP/UDP_IP
#define UDP_GATE	1
#define TCP_GATE	2

#define UDP_IP		3
#define TCP_IP		4

#endif