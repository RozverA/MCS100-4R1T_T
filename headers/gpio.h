#ifndef GPIO_H_
#define GPIO_H_

void led_init();
//define
	////pin control
		//////device
			#define LED				1
			#define RTS				0
			#define W55				2
		//////device name
			#define PWR				0
			#define PORT485_0		0
			#define PORT485_1		1
			#define PORT485_2		2
			#define PORT485_3		3
		//////work mode
			#define ON				1
			#define OFF				0
			#define TGL				2
			#define STAT			0
			#define SET				1 //Request to send (запрос на отправку) - низкий уровень на этом выходе сигнализирует о готовности USART принимать данные.
			#define CLR				2 //Clear To Send (очистить для отправки)- высокий уровень на входе приостанавливает передачу данных после завершения текущей посылки.
	////funx
		#define CS_set() { PORT_IOBUS->Group[0].OUTSET.reg=PORT_PA24; }
		#define CS_clr() { PORT_IOBUS->Group[0].OUTCLR.reg=PORT_PA24; }
	////warning sys
		#define led_init() {pin_ctrl(LED,PWR,ON); wait(2500);}
		#define warning_led(err_numb) {for (BYTE i = 0; i < err_numb*2; i++) {pin_ctrl(LED,PWR,TGL); wait(3500);}}				
		#define check_cfg_1_err()			{led_init(); if (err_dword.cfg_1_init)			{warning_led(1);} }
		#define check_cfg_2_err()			{led_init(); if (err_dword.cfg_2_init)			{warning_led(2);} }
		#define check_common_sock_err()		{led_init(); if (err_dword.common_sock)			{warning_led(3);} }
		#define check_socket_1_err()		{led_init(); if ((err_dword.socket >> 0) & 1)	{warning_led(4);} }
		#define check_socket_2_err()		{led_init(); if ((err_dword.socket >> 1) & 1)	{warning_led(5);} }
		#define check_socket_3_err()		{led_init(); if ((err_dword.socket >> 2) & 2)	{warning_led(6);} }
		#define check_socket_4_err()		{led_init(); if ((err_dword.socket >> 3) & 3)	{warning_led(7);} }
		#define check_ip_init_err()			{led_init(); if (err_dword.ip_init_er)			{warning_led(8);} }
		#define check_mask_init_err()		{led_init(); if (err_dword.mask_init_er)		{warning_led(9);} }
		#define check_gw_init_err()			{led_init(); if (err_dword.gw_init_er)			{warning_led(10);} }
		#define check_mac_init_err()		{led_init(); if (err_dword.mac_init_er)			{warning_led(11);} }
		#define check_ssh_init_err()		{led_init(); if (err_dword.ssh_init_err)		{warning_led(12);} }
		#define warning() { check_cfg_1_err();  check_cfg_2_err(); check_common_sock_err(); check_socket_1_err(); check_socket_2_err(); check_socket_3_err(); check_socket_4_err(); check_ip_init_err(); check_mask_init_err(); check_gw_init_err(); check_mac_init_err(); check_ssh_init_err()}
//extern
	extern void gpio_init (void);
	extern DWORD pin_ctrl(BYTE device, BYTE  numb, BYTE mod);
	extern void check_gerkon(void);
	
#endif /* GPIO_H_ */