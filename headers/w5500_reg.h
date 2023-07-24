#ifndef W5500_REG_H_
#define W5500_REG_H_

#define EMPTY 0
#define BUSY 1


#define READ_DATA 1
#define WRITE_DATA 2

//address COMMON
#define ADDR_COM_MODE					0x0000
#define ADDR_COM_GATEWAY_ADDR_0			0x0001
#define ADDR_COM_GATEWAY_ADDR_1			0x0002
#define ADDR_COM_GATEWAY_ADDR_2			0x0003
#define ADDR_COM_GATEWAY_ADDR_3			0x0004
#define ADDR_COM_MASK_ADDR_0			0x0005
#define ADDR_COM_MASK_ADDR_1			0x0006
#define ADDR_COM_MASK_ADDR_2			0x0007
#define ADDR_COM_MASK_ADDR_3			0x0008
#define ADDR_COM_MAC_ADDR_0				0x0009
#define ADDR_COM_MAC_ADDR_1				0x000A
#define ADDR_COM_MAC_ADDR_2				0x000B
#define ADDR_COM_MAC_ADDR_3				0x000C
#define ADDR_COM_MAC_ADDR_4				0x000D
#define ADDR_COM_MAC_ADDR_5				0x000E
#define ADDR_COM_SRC_IP_ADR_0			0x000F
#define ADDR_COM_SRC_IP_ADR_1			0x0010
#define ADDR_COM_SRC_IP_ADR_2			0x0011
#define ADDR_COM_SRC_IP_ADR_3			0x0012
#define ADDR_COM_IRQ_LOW_LEVEL_TIMER_0	0x0013
#define ADDR_COM_IRQ_LOW_LEVEL_TIMER_1	0x0014
#define ADDR_COM_IRQ					0x0015
#define ADDR_COM_IRQ_MASK				0x0016
#define ADDR_COM_SOCK_IRQ				0x0017
#define ADDR_COM_SOCK_IRQ_MASK			0x0018
#define ADDR_COM_RETRY_TIME_0			0x0019
#define ADDR_COM_RETRY_TIME_1			0x001A
#define ADDR_COM_RETRY_COUNT			0x001B
#define ADDR_COM_PPP_LCP_REQ_TIMER		0x001C
#define ADDR_COM_PPP_LCP_MAGIC_NUMBER	0x001D
#define ADDR_COM_PPP_DEST_MAC_ADDR_0	0x001E
#define ADDR_COM_PPP_DEST_MAC_ADDR_1	0x001F
#define ADDR_COM_PPP_DEST_MAC_ADDR_2	0x0020
#define ADDR_COM_PPP_DEST_MAC_ADDR_3	0x0021
#define ADDR_COM_PPP_DEST_MAC_ADDR_4	0x0022
#define ADDR_COM_PPP_DEST_MAC_ADDR_5	0x0023
#define ADDR_COM_PPP_SESSION_IDENT_0	0x0024
#define ADDR_COM_PPP_SESSION_IDENT_1	0x0025
#define ADDR_COM_PPP_MAX_SEGMENT_SIZE_0	0x0026
#define ADDR_COM_PPP_MAX_SEGMENT_SIZE_1	0x0027
#define ADDR_COM_UNREACHABLE_IP_ADDR_0	0x0028
#define ADDR_COM_UNREACHABLE_IP_ADDR_1	0x0029
#define ADDR_COM_UNREACHABLE_IP_ADDR_2	0x002A
#define ADDR_COM_UNREACHABLE_IP_ADDR_3	0x002B
#define ADDR_COM_UNREACHABLE_PORT_0		0x002C
#define ADDR_COM_UNREACHABLE_PORT_1		0x002D
#define ADDR_COM_PHY_CONFIGURATION		0x002E
#define ADDR_COM_CHIP_VERSION			0x0039

//address SOCKET
#define ADDR_SOC_MODE					0x0000
#define ADDR_SOC_COMMAND				0x0001
#define ADDR_SOC_INT					0x0002
#define ADDR_SOC_STATUS					0x0003
#define ADDR_SOC_SRC_PORT0				0x0004
#define ADDR_SOC_SRC_PORT1				0x0005
#define ADDR_SOC_D_HARD_ADDR0			0x0006
#define ADDR_SOC_D_HARD_ADDR1			0x0007
#define ADDR_SOC_D_HARD_ADDR2			0x0008
#define ADDR_SOC_D_HARD_ADDR3			0x0009
#define ADDR_SOC_D_HARD_ADDR4			0x000A
#define ADDR_SOC_D_HARD_ADDR5			0x000B
#define ADDR_SOC_D_IP_ADDR0				0x000C
#define ADDR_SOC_D_IP_ADDR1				0x000D
#define ADDR_SOC_D_IP_ADDR2				0x000E
#define ADDR_SOC_D_IP_ADDR3				0x000F
#define ADDR_SOC_D_PORT0				0x0010
#define ADDR_SOC_D_PORT1				0x0011
#define ADDR_SOC_MAX_SEGM_SIZE0			0x0012
#define ADDR_SOC_MAX_SEGM_SIZE1			0x0013
#define ADDR_SOC_IP_TOS					0x0015
#define ADDR_SOC_IP_TTL					0x0016
#define ADDR_SOC_RX_BUF_SIZE			0x001E
#define ADDR_SOC_TX_BUF_SIZE			0x001F
#define ADDR_SOC_TX_FREE_SIZE_0			0x0020
#define ADDR_SOC_TX_FREE_SIZE_1			0x0021
#define ADDR_SOC_TX_READ_PTR_0			0x0022
#define ADDR_SOC_TX_READ_PTR_1			0x0023
#define ADDR_SOC_TX_WRITE_PTR_0			0x0024
#define ADDR_SOC_TX_WRITE_PTR_1			0x0025
#define ADDR_SOC_RX_RECEIVED_SIZE_0		0x0026
#define ADDR_SOC_RX_RECEIVED_SIZE_1		0x0027
#define ADDR_SOC_RX_READ_PTR_0			0x0028
#define ADDR_SOC_RX_READ_PTR_1			0x0029
#define ADDR_SOC_RX_WRITE_PTR_0			0x002A
#define ADDR_SOC_RX_WRITE_PTR_1			0x002B
#define ADDR_SOC_IRQ_MASK_1				0x002C
#define ADDR_SOC_FRAG_OFFSET_IP_HEADER0	0x002D
#define ADDR_SOC_FRAG_OFFSET_IP_HEADER1	0x002E
#define ADDR_SOC_KEEP_ALIVE_TIMER		0x002F

//Control register
#define LEN_N_BYTES			(0x00)
#define LEN_1_BYTES			(0x01)
#define LEN_2_BYTES			(0x02)
#define LEN_4_BYTES			(0x03)

#define READ_REG			(0x00<<2)
#define WRITE_REG			(0x01<<2)

#define COMMON_REGISTER		(0x00<<3)
#define SOCKET_REGISTER		(0x01<<3)
#define SOCKET_TX_BUFFER	(0x02<<3)
#define SOCKET_RX_BUFFER	(0x03<<3)

#define SOCKET(n)			(n<<5)

//////////////////COMMON_REGISTER_ADDRESS_0x0000////////////////
#define CR_00_FARP			(0x01<<1)   // Force ARP
#define CR_00_PPPoE			(0x01<<3)   // PPPoe mode
#define CR_00_PINGBLOCK		(0x01<<4)   // Ping Block
#define CR_00_WOL			(0x01<<5)   // WAKE OF LAN
#define CR_00_RST			(0x01<<7)   // RESET
//////////////////COMMON_REGISTER_ADDRESS_0x0015////////////////
#define CR_15_MP			(0x01<<4)   // MAGIC PACKET
#define CR_15_PPPOE			(0x01<<5)   // PPPOE CONNECTION CLOSE
#define CR_15_UNREACH		(0x01<<6)   // DESTINATION UNREACHABLE
#define CR_15_CONFLICT		(0x01<<7)   // IP CONFLICT
//////////////////COMMON_REGISTER_ADDRESS_0x0015////////////////
#define CR_16_IMR_IR4		(0x01<<4)   // MAGIC PACKET Interrupt Mask#define CR_16_IMR_IR5		(0x01<<5)   // PPPOE CLOSE Interrupt Mask
#define CR_16_IMR_IR6		(0x01<<6)   // DESTINATION UNREACHABLE Interrupt Mask
#define CR_16_IMR_IR7		(0x01<<7)   // IP CONFLICT Interrupt Mask
//////////////////COMMON_REGISTER_ADDRESS_0x0017////////////////
#define CR_17_S0_INT		(0x01<<0)   // Socket Interrupt Register
#define CR_17_S1_INT		(0x01<<1)   // Socket Interrupt Register
#define CR_17_S2_INT		(0x01<<2)   // Socket Interrupt Register
#define CR_17_S3_INT		(0x01<<3)   // Socket Interrupt Register
#define CR_17_S4_INT		(0x01<<4)   // Socket Interrupt Register
#define CR_17_S5_INT		(0x01<<5)   // Socket Interrupt Register
#define CR_17_S6_INT		(0x01<<6)   // Socket Interrupt Register
#define CR_17_S7_INT		(0x01<<7)   // Socket Interrupt Register
//////////////////COMMON_REGISTER_ADDRESS_0x0018////////////////
#define CR_18_S0_IMR		(0x01<<0)   // Socket Interrupt Mask Register
#define CR_18_S1_IMR		(0x01<<1)   // Socket Interrupt Mask Register
#define CR_18_S2_IMR		(0x01<<2)   // Socket Interrupt Mask Register
#define CR_18_S3_IMR		(0x01<<3)   // Socket Interrupt Mask Register
#define CR_18_S4_IMR		(0x01<<4)   // Socket Interrupt Mask Register
#define CR_18_S5_IMR		(0x01<<5)   // Socket Interrupt Mask Register
#define CR_18_S6_IMR		(0x01<<6)   // Socket Interrupt Mask Register
#define CR_18_S7_IMR		(0x01<<7)   // Socket Interrupt Mask Register
//////////////////COMMON_REGISTER_ADDRESS_0x002E////////////////
#define CR_2E_LNK			(0x01<<0)   // LINK STATUS
#define CR_2E_SPD			(0x01<<1)   // SPEED STATUS
#define CR_2E_DPX			(0x01<<2)   // DUPLEX STATUS
#define CR_2E_OPMDC_1		(0x01<<3)   // OPERATION MODE CONFIGURATION
#define CR_2E_OPMDC_2		(0x01<<4)   // OPERATION MODE CONFIGURATION
#define CR_2E_OPMDC_3		(0x01<<5)   // OPERATION MODE CONFIGURATION
#define CR_2E_OPMD			(0x01<<6)   // CONFIGURE PHY OPERATION MODE
#define CR_2E_RST			(0x01<<7)   // RESET


//////////////////SOCKET_REGISTER_ADDRESS_0x0000////////////////
#define SR_00_P0			(0x01<<0)   // ONE OF FOUR CONFIG BIT P3-0 P2-0 P1-0 P0-0 Closed
#define SR_00_P1			(0x01<<0)   // ONE OF FOUR CONFIG BIT P3-0 P2-0 P1-0 P0-1 TCP
#define SR_00_P2			(0x01<<1)   // ONE OF FOUR CONFIG BIT P3-0 P2-0 P1-1 P0-0 UDP
#define SR_00_P3			(0x01<<2)   // ONE OF FOUR CONFIG BIT P3-0 P2-1 P1-0 P0-0 MACRAW
#define SR_00_UCASTB_MIP6B	(0x01<<4)   // UNICAST Blocking in UDP mode/IPv6 packet Blocking in MACRAW mode
#define SR_00_ND_MC_MMB		(0x01<<5)   // Use No Delayed ACK (TCP)/Multicast(UDP)/Multicast Blocking in MACRAW mode
#define SR_00_BCASTB		(0x01<<6)   // Broadcast Blocking in MACRAW and UDP mode#define SR_00_MULTI_MFEN	(0x01<<7)   // Multicasting in UDP mode/MAC Filter Enable in MACRAW mode//////////////////SOCKET_REGISTER_ADDRESS_0x0002////////////////
#define SR_02_CON			(0x01<<0)   // Sn_IR(CON) Interrupt
#define SR_02_DISCON		(0x01<<1)   // Sn_IR(DISCON) Interrupt
#define SR_02_RECV			(0x01<<2)   // Sn_IR(RECV) Interrupt
#define SR_02_TIMEOUT		(0x01<<3)   // Sn_IR(TIMEOUT) Interrupt
#define SR_02_SEND_OK		(0x01<<4)   // Sn_IR(SENDOK) Interrupt
//////////////////SOCKET_REGISTER_ADDRESS_0x002C////////////////
#define SR_2C_CON			(0x01<<0)   // Sn_IR(CON) Interrupt Mask
#define SR_2C_DISCON		(0x01<<1)   // Sn_IR(DISCON) Interrupt Mask
#define SR_2C_RECV			(0x01<<2)   // Sn_IR(RECV) Interrupt Mask
#define SR_2C_TIMEOUT		(0x01<<3)   // Sn_IR(TIMEOUT) Interrupt Mask
#define SR_2C_SEND_OK		(0x01<<4)   // Sn_IR(SENDOK) Interrupt Mask

#define ST_CLOSE		0x00
#define ST_INIT			0x13
#define ST_LISTEN		0x14
#define ST_ESTABLISHED	0x17
#define ST_CLOSE_WAIT	0x1c

#define CMD_OPEN		0x01
#define CMD_LISTEN		0x02
#define CMD_CONNECT		0x04
#define CMD_DISCON		0x08
#define CMD_CLOSE		0x10
#define CMD_SEND		0x20
#define CMD_SEND_MAC	0x21
#define CMD_SEND_KKEP	0x22
#define CMD_RECV		0x40

typedef struct
{	
BYTE mode_op;
BYTE numb_socket;
	
}W5500_MODE;//определяет измененное сосояние 1го сокета

extern W5500_MODE w5500_mode;


typedef struct
{
	struct
	{
		struct
		{
			BYTE FARP: 1;
			BYTE Reserved: 1;
			BYTE PPPoE: 1;
			BYTE PB: 1;
			BYTE WOL: 1;
			BYTE Reserved1: 1;
			BYTE RST: 1;
		}R01_MR_00;
		
		struct
		{
			BYTE IP_addr_1;
			BYTE IP_addr_2;
			BYTE IP_addr_3;
			BYTE IP_addr_4;
		}R02_GAR_01_04;
		
		struct
		{
			BYTE case_1;
			BYTE case_2;
			BYTE case_3;
			BYTE case_4;
		}R02_1_SUBR_05_08;
		
		struct
		{
			BYTE case_1;
			BYTE case_2;
			BYTE case_3;
			BYTE case_4;
			BYTE case_5;
			BYTE case_6;
		}R03_SHAR_09_0E;
		
		struct
		{
			BYTE case_1;
			BYTE case_2;
			BYTE case_3;
			BYTE case_4;
		}R04_SIPR_0F_12;
		
	}commonReg;
	
	struct
	{
		struct
		{
			BYTE P0: 1;
			BYTE P1: 1;
			BYTE P2: 1;
			BYTE P3: 1;
			BYTE UCASTB: 1;
			BYTE ND_MC_MMB: 1;
			BYTE BCASTB: 1;
			BYTE MULTI_MFEN: 1;
		}R01_Sn_MR_00;
		
		struct
		{
			BYTE Socket_command_01_open;
		}R02_Sn_CR_01;
		
		struct
		{
			BYTE CON: 1;
			BYTE DISCON: 1;
			BYTE RECV: 1;
			BYTE TIMEOUT: 1;
			BYTE SEND_OK: 1;
			BYTE reserv3: 1;
			BYTE reserv2: 1;
			BYTE reserv1: 1;
		}R03_Sn_IR_02;
		
		struct
		{
			BYTE Status;

		}R04_Sn_SR_03;
		
		struct
		{
			BYTE case1;
			BYTE case2;
		}R05_Sn_PORT_04_05;
		
		struct
		{
			BYTE case1;
			BYTE case2;
			BYTE case3;
			BYTE case4;
			BYTE case5;
			BYTE case6;
		}R06_Sn_DHAR_06_0B;
		
		struct
		{
			BYTE case1;
			BYTE case2;
			BYTE case3;
			BYTE case4;
		}R07_Sn_DIPR_0C_0F;
		
		struct
		{
			BYTE case1;
			BYTE case2;
		}R08_Sn_DPORT_10_11;
		
		struct
		{
			BYTE case1;
			BYTE case2;
		}R09_Sn_MSSR_12_13;
		
		struct
		{
			BYTE case1;
			BYTE case2;
		}R014_Sn_TX_FSR_20_21;
		
		struct
		{
			BYTE case1;
			BYTE case2;
		}R015_Sn_TX_RD_22_23;
		
		struct
		{
			BYTE case1;
			BYTE case2;
		}R016_Sn_TX_WR_24_25;
		
		struct
		{
			BYTE case1;
			BYTE case2;
		}R017_Sn_RX_RSR_26_27;
		
		struct
		{
			BYTE case1;
			BYTE case2;
		}R018_Sn_RX_RD_28_29;
		
		struct
		{
			BYTE case1;
			BYTE case2;
		}R019_Sn_RX_WR_2A_2B;
		
		struct
		{
			BYTE CON: 1;
			BYTE DISCON: 1;
			BYTE RECV: 1;
			BYTE TIMEOUT: 1;
			BYTE SEND_OK: 1;
			BYTE reserv3: 1;
			BYTE reserv2: 1;
			BYTE reserv1: 1;
		}R20_Sn_IMR_2C;
		
		struct
		{
			BYTE case1;
			BYTE case2;
		}R21_Sn_FRAG_2D_2E;
		
		struct
		{
			BYTE case1;
		}R22_Sn_KPALVRT_2F;

	}sockReg[8];
}w5500;

extern w5500 chip;




extern func_st w5500_init_reg(void);
extern func_st mac_init(void);
extern func_st mask_init(void);
extern func_st gw_init(void);
extern func_st ip_init(void);
extern WORD w5500_process (BYTE spi_mode, BYTE sock_numb, BYTE *buf);
extern WORD w5500_cmd_read_socket_udp (BYTE numb, BYTE *buf);
extern WORD w5500_write_socket_udp (BYTE numb, BYTE *buf);
extern WORD w5500_cmd_read_socket_tcp (BYTE sock_numb, BYTE *buf);
extern WORD w5500_write_socket_tcp (BYTE numb, BYTE *buf);
extern WORD w5500_ch_sock (BYTE sock_numb, BYTE *buf);
//extern WORD w5500_check_rx_buffer (BYTE numb);


extern WORD spi_mode;


#endif