#ifndef SPI_H_
#define SPI_H_


#define spi_write_timeout(addr,cb,buf,len,timeout)	{spi_wait=timeout;while(!spi_write(addr, cb, buf, len)){if(!spi_wait){return TIMEOUT;}}}
#define spi_read_timeout(addr,cb,buf,len,timeout)	{spi_wait=timeout;while(!spi_read(addr, cb, buf, len)){if(!spi_wait){return TIMEOUT;}}}
extern void spi_init(void);

extern WORD spi_process (uint16_t addr, uint8_t cb, uint8_t *buf, uint16_t len, BYTE cmd);
extern WORD spi_read(uint16_t addr, uint8_t cb, uint8_t *rx_buf, uint16_t len);
extern WORD spi_write(uint16_t addr, uint8_t cb, uint8_t *tx_buf, uint16_t len);



#define MAX_SIZE_BUF_SPI 1480

typedef struct
{
	BYTE wbuf[MAX_SIZE_BUF_SPI];
	BYTE rbuf[MAX_SIZE_BUF_SPI];

	WORD rn;
	WORD wn;
	WORD wx;


	DWORD DRE_sum;
	DWORD RXC_sum;
}SPI;

extern SPI spi;


#endif /* SPI_H_ */