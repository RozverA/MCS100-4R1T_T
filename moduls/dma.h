#ifndef DMA_H_
#define DMA_H_

typedef struct {
	__IO DMAC_BTCTRL_Type          BTCTRL;      /**< \brief Offset: 0x00 (R/W 16) Block Transfer Control */
	__IO DMAC_BTCNT_Type           BTCNT;       /**< \brief Offset: 0x02 (R/W 16) Block Transfer Count */
	__IO DMAC_SRCADDR_Type         SRCADDR;     /**< \brief Offset: 0x04 (R/W 32) Block Transfer Source Address */
	__IO DMAC_DSTADDR_Type         DSTADDR;     /**< \brief Offset: 0x08 (R/W 32) Block Transfer Destination Address */
	__IO DMAC_DESCADDR_Type        DESCADDR;    /**< \brief Offset: 0x0C (R/W 32) Next Descriptor Address */
} Descriptor;

void dma_init();

extern Descriptor DESCR;

#endif /* DMA_H_ */