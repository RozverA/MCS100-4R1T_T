#include "def.h"

CH_USART_FLAGS CUF;

BYTE* ptr = &CUF;

BYTE hallo[] = {0x0A,0x0D,0x48,0x61,0x6C,0x6C,0x6F,0x21,0x20,0x49,0x20,0x61,0x6D,0x20,0x4D,0x43,0x53,0x31,0x30,0x30,0x2D,0x34,0x52,0x31,0x0A,0x0D};
BYTE test_mess[] = {0x20,0x52,0x5A,0x2D,0x31,0x30,0x39,0x22,0x74,0x65,0x73,0x74,0x7E,0x6D,0x65,0x73,0x73,0x65,0x67,0x65,0x30,0x22,0x0A,0x0D};//RZ-109"TEST~MESSEGE"
BYTE an_stat[] = {0x0A,0x0D,0x53,0x74,0x61,0x74,0x75,0x73,0x20,0x45,0x54,0x48,0x20,0x3D,0x20,0x43,0x48,0x45,0x43,0x4B,0x0A,0x0D};//2+13+5+2
BYTE ready_s1 = CH_TRUE;
BYTE messN = 0x00;

void test_funx()
{
	check_flag();
	test_funx_1();
	check_flag();
}


void test_funx_1()
{
	if (port[0].wx == port[0].wn & port[0].rx == port[0].rn ) 
	{
		messege();
	}
	else {return;}
}

void messege()
{
	//if(messN==3){messN=0;}
	switch (messN)
	{
		case 0:
			usart_0_write(hallo,sizeof(hallo));//2
			messN++;//next mess
			break;
		case 1:
			usart_0_write(test_mess,sizeof(test_mess));
			messN++;
			break;
		case 2:
			usart_0_write(an_stat,sizeof(an_stat));
			messN++;
			break;
	}
}

void check_flag()
{
	*ptr = USART1_FLAG;
}

//DEZ
// 	volatile size = sizeof(TRUE);//4
// 	static sizeCh = sizeof(CH_TRUE);//1
// 	switch (n_port)
// 	{
// 		case 1:
// 			if (ch == CH_TRUE)
// 			{
// 				test_funx();
// 				break;
// 			}
// 		case 2:
// 			if (ch == CH_TRUE)
// 			{
// 				//usart_1_write(test_mess,8);//2
// 				//ch = CH_FALSE;
// 				break;
// 			}
// 		case 3:
// 			if (ch == CH_TRUE)
// 			{
// 				//usart_2_write(test_mess,8);//3
// 				//ch = CH_FALSE;
// 				break;
// 			}
// 		case 4:
// 			if (ch == CH_TRUE)
// 			{
// 				//usart_3_write(test_mess,8);//4
// 				//ch = CH_FALSE;
// 				break;
// 			}
// 	}