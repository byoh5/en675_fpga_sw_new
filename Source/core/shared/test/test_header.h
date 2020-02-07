#ifndef __TEST_HEADER_H__
#define __TEST_HEADER_H__

////////////////////////////////////////////////////////////////////////////////

typedef struct {
	BYTE TxData[1536];
	struct {
		BYTE buffer[1536];
	} *RxData;

	UINT idx;
	UINT err;
	UINT flag;
} EthTestStr_nonos;

extern void test_eth_init(void);
extern void test_eth_start(void);

////////////////////////////////////////////////////////////////////////////////

typedef enum {
	eDMAtest_DDRtoDDR,
	eDMAtest_DDRtoSRAM,
	eDMAtest_SRAMtoDDR,
	eDMAtest_SRAMtoSRAM,
} DMAtestTarget;

typedef struct {
	BYTE *arrSrc;
	BYTE *arrDst;
	ULONG u64BufSize;

	UINT u32TestSize;
	UINT u32Move;
	UINT cMode;
	UINT u32CH;

	DMAtestTarget target;
	UINT flag;
} DmaTestStr_nonos;

extern void test_dma_init(ULONG bufsize, UINT testsize, UINT move, char mode, UINT ch, DMAtestTarget target);
extern void test_dma_start(void);

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

#endif // __TEST_HEADER_H__
