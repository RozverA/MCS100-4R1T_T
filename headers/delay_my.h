#ifndef DELAY_MY_H_
#define DELAY_MY_H_

#define nop()        { asm volatile("nop"::); }  // 62.5ns (16 MHz)
#define nop_125_ns() { nop(); nop(); }
#define nop_250_ns() { nop_125_ns(); nop_125_ns(); }
#define nop_375_ns() { nop_125_ns(); nop_125_ns(); }
#define nop_500_ns() { nop_250_ns(); nop_250_ns(); }
#define nop_625_ns() { nop_250_ns(); nop_250_ns(); nop_125_ns(); }
#define nop_750_ns() { nop_250_ns(); nop_250_ns(); nop_250_ns(); }
#define nop_1_us()   { nop_500_ns(); nop_500_ns(); }
#define nop_2_us()   { nop_1_us(); nop_1_us(); }
#define nop_3_us()   { nop_1_us(); nop_1_us(); nop_1_us(); }
#define nop_4_us()   { nop_1_us(); nop_1_us(); nop_1_us(); nop_1_us(); }
#define nop_5_us()   { nop_1_us(); nop_1_us(); nop_1_us(); nop_1_us(); nop_1_us(); }

#define nop_100_ms() do										\
{									\
	uint16_t i;						\
	i = 2000;						\
	\
	while (i) { nop_5_us(); i--; }	\
	\
} while(0U)

#define nop_1_ms() do										\
{									\
	uint16_t i;						\
	i = 20;						\
	\
	while (i) { nop_5_us(); i--; }	\
	\
} while(0U)

#define nop_10_ms() do										\
{									\
	uint16_t i;						\
	i = 10;						\
	\
	while (i) { nop_1_ms(); i--; }	\
	\
} while(0U)




#endif /* DELAY_MY_H_ */