#ifndef __MICRON_N25Q128A__H__
#define __MICRON_N25Q128A__H__

#define N25Q128A_IOMODE				SFLS_QUAD // SFLS_SINGLE // SFLS_DUAL // SFLS_QUAD

#if (N25Q128A_IOMODE==SFLS_SINGLE)
#define N25Q128A_RDLTC				1
#define N25Q128A_READ_GAP			8
#define N25Q128A_IOR_CMD			SFLS_SINGLE
#define N25Q128A_IOR_ADR			SFLS_SINGLE
#define N25Q128A_IOR_DAT			SFLS_SINGLE
#define N25Q128A_IOW_CMD			SFLS_SINGLE
#define N25Q128A_IOW_ADR			SFLS_SINGLE
#define N25Q128A_IOW_DAT			SFLS_SINGLE
#define N25Q128A_IOWE_CMD			SFLS_SINGLE
#define N25Q128A_IORS_CMD			SFLS_SINGLE
#define N25Q128A_IORS_DAT			SFLS_SINGLE
#define N25Q128A_CMD_PAGE_PROGRAM	0x02
#elif (N25Q128A_IOMODE==SFLS_DUAL)
#define N25Q128A_RDLTC				1
#define N25Q128A_READ_GAP			11
#define N25Q128A_IOR_CMD			SFLS_SINGLE
#define N25Q128A_IOR_ADR			SFLS_DUAL
#define N25Q128A_IOR_DAT			SFLS_DUAL
#define N25Q128A_IOW_CMD			SFLS_SINGLE
#define N25Q128A_IOW_ADR			SFLS_SINGLE
#define N25Q128A_IOW_DAT			SFLS_SINGLE
#define N25Q128A_IOWE_CMD			SFLS_SINGLE
#define N25Q128A_IORS_CMD			SFLS_SINGLE
#define N25Q128A_IORS_DAT			SFLS_SINGLE
#define N25Q128A_CMD_PAGE_PROGRAM	0x02
#elif (N25Q128A_IOMODE==SFLS_QUAD)
#define N25Q128A_RDLTC				1
#define N25Q128A_READ_GAP			11
#define N25Q128A_IOR_CMD			SFLS_SINGLE
#define N25Q128A_IOR_ADR			SFLS_QUAD
#define N25Q128A_IOR_DAT			SFLS_QUAD
#define N25Q128A_IOW_CMD			SFLS_SINGLE
#define N25Q128A_IOW_ADR			SFLS_SINGLE
#define N25Q128A_IOW_DAT			SFLS_QUAD
#define N25Q128A_IOWE_CMD			SFLS_SINGLE
#define N25Q128A_IORS_CMD			SFLS_SINGLE
#define N25Q128A_IORS_DAT			SFLS_SINGLE
#define N25Q128A_CMD_PAGE_PROGRAM	0x32
#endif

#define N25Q128A_CMD_READ			0x0B
#define N25Q128A_CMD_DUAL_READ		0xBB
#define N25Q128A_CMD_QUAD_READ		0xEB
//#define N25Q128A_CMD_PAGE_PROGRAM	0x02
#define N25Q128A_CMD_WRITE_ENABLE	0x06
#define N25Q128A_CMD_READ_STATUS	0x05
#define N25Q128A_CMD_SECTOR_ERASE	0x20
#define N25Q128A_CMD_64KB_ERASE		0xD8
#define N25Q128A_CMD_CHIP_ERASE		0xC7

// Enhanced Volatile Configuration Register
#define N25Q128A_CMD_READ_ENHANCED_VOLATILE_CONFIGURATION 0x65
#define N25Q128A_CMD_WRITE_ENHANCED_VOLATILE_CONFIGURATION 0x61

extern void SflsN25q128a_Init(void);
extern void SflsN25q128a_WriteEnable(void);
extern void SflsN25q128a_SecterErase(UINT addr);
extern void SflsN25q128a_32kErase(UINT addr);
extern void SflsN25q128a_64kErase(UINT addr);
extern void SflsN25q128a_ChipErase(UINT addr);

#endif // __MICRON_N25Q128A__H__
