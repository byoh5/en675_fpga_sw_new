#ifndef __MICRON_N25Q128A__H__
#define __MICRON_N25Q128A__H__

////////////////////////////////////////////////////////////////////////////////
// RTL(200130_1405) - single(o) dual(o) quad(o)
////////////////////////////////////////////////////////////////////////////////

#define N25Q128A_SIZE					(16*1024*1024)

#define N25Q128A_IOMODE					SFLS_QUAD // SFLS_SINGLE // SFLS_DUAL // SFLS_QUAD

#if (N25Q128A_IOMODE==SFLS_SINGLE)
#define N25Q128A_RDLTC					1
#define N25Q128A_READ_GAP				8
#define N25Q128A_IOR_CMD				SFLS_SINGLE
#define N25Q128A_IOR_ADR				SFLS_SINGLE
#define N25Q128A_IOR_DAT				SFLS_SINGLE
#define N25Q128A_IOW_CMD				SFLS_SINGLE
#define N25Q128A_IOW_ADR				SFLS_SINGLE
#define N25Q128A_IOW_DAT				SFLS_SINGLE
#define N25Q128A_IOWE_CMD				SFLS_SINGLE
#define N25Q128A_IORS_CMD				SFLS_SINGLE
#define N25Q128A_IORS_DAT				SFLS_SINGLE
#define N25Q128A_CMD_WRITE				0x02
#define N25Q128A_CMD_READ				0x0B
#elif (N25Q128A_IOMODE==SFLS_DUAL)
#define N25Q128A_RDLTC					1
#define N25Q128A_READ_GAP				11
#define N25Q128A_IOR_CMD				SFLS_SINGLE
#define N25Q128A_IOR_ADR				SFLS_DUAL
#define N25Q128A_IOR_DAT				SFLS_DUAL
#define N25Q128A_IOW_CMD				SFLS_SINGLE
#define N25Q128A_IOW_ADR				SFLS_SINGLE
#define N25Q128A_IOW_DAT				SFLS_SINGLE
#define N25Q128A_IOWE_CMD				SFLS_SINGLE
#define N25Q128A_IORS_CMD				SFLS_SINGLE
#define N25Q128A_IORS_DAT				SFLS_SINGLE
#define N25Q128A_CMD_WRITE				0x02
#define N25Q128A_CMD_READ				0xBB
#elif (N25Q128A_IOMODE==SFLS_QUAD)
#define N25Q128A_RDLTC					1
#define N25Q128A_READ_GAP				11
#define N25Q128A_IOR_CMD				SFLS_SINGLE
#define N25Q128A_IOR_ADR				SFLS_QUAD
#define N25Q128A_IOR_DAT				SFLS_QUAD
#define N25Q128A_IOW_CMD				SFLS_SINGLE
#define N25Q128A_IOW_ADR				SFLS_SINGLE
#define N25Q128A_IOW_DAT				SFLS_QUAD
#define N25Q128A_IOWE_CMD				SFLS_SINGLE
#define N25Q128A_IORS_CMD				SFLS_SINGLE
#define N25Q128A_IORS_DAT				SFLS_SINGLE
#define N25Q128A_CMD_WRITE				0x32
#define N25Q128A_CMD_READ				0xEB
#endif

#define N25Q128A_CMD_FAST_READ			N25Q128A_CMD_READ
#define N25Q128A_CMD_PAGE_PROGRAM		N25Q128A_CMD_WRITE
#define N25Q128A_CMD_WRITE_ENABLE		0x06
#define N25Q128A_CMD_READ_STATUS		0x05
#define N25Q128A_CMD_WRITE_STATUS		0x01
#define N25Q128A_CMD_READ_SFDP			0x5A
#define N25Q128A_CMD_SECTOR_ERASE		0x20
#define N25Q128A_CMD_64KB_ERASE			0xD8
#define N25Q128A_CMD_CHIP_ERASE			0xC7

extern void SflsN25q128a_Init1(void);
extern void SflsN25q128a_Init2(void);
extern void SflsN25q128a_WriteEnable(void);
extern void SflsN25q128a_SecterErase(UINT addr);
extern void SflsN25q128a_32kErase(UINT addr);
extern void SflsN25q128a_64kErase(UINT addr);
extern void SflsN25q128a_ChipErase(UINT addr);
extern void SflsN25q128a_ReadSFDP(BYTE *sfdp_bin);
extern BYTE SflsN25q128a_ReadStatus(void);
extern void SflsN25q128a_WriteStatus(BYTE status);
extern void SflsN25q128a_EnterProtection(void);
extern void SflsN25q128a_ExitProtection(void);
extern UINT SflsN25q128a_IsProtection(void);

#endif // __MICRON_N25Q128A__H__
