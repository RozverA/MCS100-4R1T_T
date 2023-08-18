#ifndef CRC_H_
#define CRC_H_

extern WORD crc16_ccit(BYTE *buf,WORD size);

extern DWORD crc32(BYTE *buf,WORD size);

#endif /* CRC_H_ */