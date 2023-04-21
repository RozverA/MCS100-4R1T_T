#ifndef SYS_CLOCK_H_
#define SYS_CLOCK_H_


#define NVM_DFLL48M_COARSE_CAL_POS   58
#define NVM_DFLL48M_COARSE_CAL_SIZE  6
#define NVM_DFLL48M_FINE_CAL_POS     64
#define NVM_DFLL48M_FINE_CAL_SIZE    10
#define NVM_READ_CAL(cal) ((*((uint32_t *)NVMCTRL_OTP4 + NVM_##cal##_POS / 32)) >> (NVM_##cal##_POS % 32)) & ((1 << NVM_##cal##_SIZE) - 1)
#define MAIN_CLK_FREQ	(48000000u)
#define MAIN_CLK_DEVIDER	1


extern void sys_clock_init_USE_OSC8M(void);
extern void sys_clock_init_USE_DFLL48M(void);

#endif /* SYS_CLOCK_H_ */