#ifndef CYCLE_H_
#define CYCLE_H_

#define UCMD_CH 0
#define UCMD_WR 1
#define UCMD_RD 2

//#define TC3_coeff 0x00FA //250
#define TC3_coeff 0x02EE //750
//#define TC3_coeff 0x01F4 //500
//#define TC3_coeff 0x03E8 //1000
#define TC3_lim (0xFFFF-TC3_coeff)

#endif /* CYCLE_H_ */