#ifndef __WINBOND_W25Q64JVIQ__H__
#define __WINBOND_W25Q64JVIQ__H__

////////////////////////////////////////////////////////////////////////////////
// RTL(200130_1405) - single(o) dual(o) quad(?)
////////////////////////////////////////////////////////////////////////////////

#define W25Q64JVIQ_SIZE							(8*1024*1024)

#define W25Q64JVIQ_IOMODE						SFLS_SINGLE // SFLS_SINGLE // SFLS_DUAL // SFLS_QUAD
#define W25Q64JVIQ_ENABLE_QE					0

#if (W25Q64JVIQ_IOMODE==SFLS_SINGLE)
#define W25Q64JVIQ_RDLTC						1
#define W25Q64JVIQ_READ_GAP						8
#define W25Q64JVIQ_IOR_CMD						SFLS_SINGLE
#define W25Q64JVIQ_IOR_ADR						SFLS_SINGLE
#define W25Q64JVIQ_IOR_DAT						SFLS_SINGLE
#define W25Q64JVIQ_IOW_CMD						SFLS_SINGLE
#define W25Q64JVIQ_IOW_ADR						SFLS_SINGLE
#define W25Q64JVIQ_IOW_DAT						SFLS_SINGLE
#define W25Q64JVIQ_IOWE_CMD						SFLS_SINGLE
#define W25Q64JVIQ_IORS_CMD						SFLS_SINGLE
#define W25Q64JVIQ_IORS_DAT						SFLS_SINGLE
#define W25Q64JVIQ_CMD_READ						0x0B
#define W25Q64JVIQ_CMD_WRITE					0x02
#elif (W25Q64JVIQ_IOMODE==SFLS_DUAL)
#define W25Q64JVIQ_RDLTC						1
#define W25Q64JVIQ_READ_GAP						7
#define W25Q64JVIQ_IOR_CMD						SFLS_SINGLE
#define W25Q64JVIQ_IOR_ADR						SFLS_DUAL
#define W25Q64JVIQ_IOR_DAT						SFLS_DUAL
#define W25Q64JVIQ_IOW_CMD						SFLS_SINGLE
#define W25Q64JVIQ_IOW_ADR						SFLS_SINGLE
#define W25Q64JVIQ_IOW_DAT						SFLS_SINGLE
#define W25Q64JVIQ_IOWE_CMD						SFLS_SINGLE
#define W25Q64JVIQ_IORS_CMD						SFLS_SINGLE
#define W25Q64JVIQ_IORS_DAT						SFLS_SINGLE
#define W25Q64JVIQ_CMD_READ						0xBB
#define W25Q64JVIQ_CMD_WRITE					0x02
#elif (W25Q64JVIQ_IOMODE==SFLS_QUAD)
#define W25Q64JVIQ_RDLTC						1
#define W25Q64JVIQ_READ_GAP						7
#define W25Q64JVIQ_IOR_CMD						SFLS_SINGLE
#define W25Q64JVIQ_IOR_ADR						SFLS_QUAD
#define W25Q64JVIQ_IOR_DAT						SFLS_QUAD
#define W25Q64JVIQ_IOW_CMD						SFLS_SINGLE
#define W25Q64JVIQ_IOW_ADR						SFLS_SINGLE
#define W25Q64JVIQ_IOW_DAT						SFLS_QUAD
#define W25Q64JVIQ_IOWE_CMD						SFLS_SINGLE
#define W25Q64JVIQ_IORS_CMD						SFLS_SINGLE
#define W25Q64JVIQ_IORS_DAT						SFLS_SINGLE
#define W25Q64JVIQ_CMD_READ						0xEB
#define W25Q64JVIQ_CMD_WRITE					0x32
#endif

#define W25Q64JVIQ_CMD_FAST_READ				W25Q64JVIQ_CMD_READ
#define W25Q64JVIQ_CMD_PAGE_PROGRAM				W25Q64JVIQ_CMD_WRITE
#define W25Q64JVIQ_CMD_WRITE_ENABLE				0x06
#define W25Q64JVIQ_CMD_READ_STATUS1				0x05
#define W25Q64JVIQ_CMD_READ_STATUS2				0x35
#define W25Q64JVIQ_CMD_READ_STATUS3				0x15
#define W25Q64JVIQ_CMD_WRITE_STATUS1			0x01
#define W25Q64JVIQ_CMD_WRITE_STATUS2			0x31
#define W25Q64JVIQ_CMD_WRITE_STATUS3			0x11
#define W25Q64JVIQ_CMD_READ_SFDP				0x5A
#define W25Q64JVIQ_CMD_SECTOR_ERASE				0x20
#define W25Q64JVIQ_CMD_32KB_ERASE				0x52
#define W25Q64JVIQ_CMD_64KB_ERASE				0xD8
#define W25Q64JVIQ_CMD_CHIP_ERASE				0x60

extern void SflsW25q64jviq_Init1(void);
extern void SflsW25q64jviq_Init2(void);
extern void SflsW25q64jviq_WriteEnable(void);
extern void SflsW25q64jviq_SecterErase(UINT addr);
extern void SflsW25q64jviq_32kErase(UINT addr);
extern void SflsW25q64jviq_64kErase(UINT addr);
extern void SflsW25q64jviq_ChipErase(UINT addr);
extern void SflsW25q64jviq_ReadSFDP(BYTE *sfdp_bin);
extern BYTE SflsW25q64jviq_ReadStatus1(void);
extern BYTE SflsW25q64jviq_ReadStatus2(void);
extern BYTE SflsW25q64jviq_ReadStatus3(void);
extern void SflsW25q64jviq_WriteStatus1(BYTE status);
extern void SflsW25q64jviq_WriteStatus2(BYTE status);
extern void SflsW25q64jviq_WriteStatus3(BYTE status);
extern void SflsW25q64jviq_EnterQE(void);
extern void SflsW25q64jviq_ExitQE(void);
extern UINT SflsW25q64jviq_IsQE(void);
extern void SflsW25q64jviq_EnterProtection(void);
extern void SflsW25q64jviq_ExitProtection(void);
extern UINT SflsW25q64jviq_IsProtection(void);

#if (W25Q64JVIQ_ENABLE_QE==1)
#if (W25Q64JVIQ_IOMODE!=SFLS_QUAD)
#error "QE mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (W25Q64JVIQ_IOMODE==SFLS_QUAD)
#if (W25Q64JVIQ_ENABLE_QE==0)
#error "IO mode is QUAD but failed to activate. Set the serial flash memory to QE mode."
#endif
#endif

#endif // __WINBOND_W25Q64JVIQ__H__
