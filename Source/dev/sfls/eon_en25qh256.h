#ifndef __EON_EN25QH256__H__
#define __EON_EN25QH256__H__

////////////////////////////////////////////////////////////////////////////////
// RTL(200130_1405) - single(o) dual(o) quad(x) quad-qpi(o)
////////////////////////////////////////////////////////////////////////////////

#define EN25QH256_SIZE				(32*1024*1024)

#define EN25QH256_IOMODE			SFLS_QUAD // SFLS_SINGLE // SFLS_DUAL // SFLS_QUAD
#define EN25QH256_ENABLE_QPI		1
#define EN25QH256_ENABLE_QE			1
#define EN25QH256_ENABLE_4B			1

#if (EN25QH256_IOMODE==SFLS_SINGLE)
#define EN25QH256_RDLTC				1
#define EN25QH256_READ_GAP			8
#define EN25QH256_IOR_CMD			SFLS_SINGLE
#define EN25QH256_IOR_ADR			SFLS_SINGLE
#define EN25QH256_IOR_DAT			SFLS_SINGLE
#define EN25QH256_IOW_CMD			SFLS_SINGLE
#define EN25QH256_IOW_ADR			SFLS_SINGLE
#define EN25QH256_IOW_DAT			SFLS_SINGLE
#define EN25QH256_IOWE_CMD			SFLS_SINGLE
#define EN25QH256_IORS_CMD			SFLS_SINGLE
#define EN25QH256_IORS_DAT			SFLS_SINGLE
#define EN25QH256_CMD_WRITE			0x02
#define EN25QH256_CMD_READ			0x0B
#elif (EN25QH256_IOMODE==SFLS_DUAL)
#define EN25QH256_RDLTC				1
#define EN25QH256_READ_GAP			11
#define EN25QH256_IOR_CMD			SFLS_SINGLE
#define EN25QH256_IOR_ADR			SFLS_SINGLE
#define EN25QH256_IOR_DAT			SFLS_DUAL
#define EN25QH256_IOW_CMD			SFLS_SINGLE
#define EN25QH256_IOW_ADR			SFLS_SINGLE
#define EN25QH256_IOW_DAT			SFLS_SINGLE
#define EN25QH256_IOWE_CMD			SFLS_SINGLE
#define EN25QH256_IORS_CMD			SFLS_SINGLE
#define EN25QH256_IORS_DAT			SFLS_SINGLE
#define EN25QH256_CMD_WRITE			0x02
#define EN25QH256_CMD_READ			0x3B
#elif (EN25QH256_IOMODE==SFLS_QUAD)
#if (EN25QH256_ENABLE_QPI==0)
#error "error: EN25QH256_IOMODE==SFLS_QUAD, QPI=0, test error"
#define EN25QH256_RDLTC				2
#define EN25QH256_READ_GAP			6
#define EN25QH256_IOR_CMD			SFLS_SINGLE
#define EN25QH256_IOR_ADR			SFLS_QUAD
#define EN25QH256_IOR_DAT			SFLS_QUAD
#define EN25QH256_IOW_CMD			SFLS_SINGLE
#define EN25QH256_IOW_ADR			SFLS_SINGLE
#define EN25QH256_IOW_DAT			SFLS_SINGLE
#define EN25QH256_IOWE_CMD			SFLS_SINGLE
#define EN25QH256_IORS_CMD			SFLS_SINGLE
#define EN25QH256_IORS_DAT			SFLS_SINGLE
#define EN25QH256_CMD_WRITE			0x02
#define EN25QH256_CMD_READ			0xEB
#else
#define EN25QH256_RDLTC				2
#define EN25QH256_READ_GAP			6
#define EN25QH256_IOR_CMD			SFLS_QUAD
#define EN25QH256_IOR_ADR			SFLS_QUAD
#define EN25QH256_IOR_DAT			SFLS_QUAD
#define EN25QH256_IOW_CMD			SFLS_QUAD
#define EN25QH256_IOW_ADR			SFLS_QUAD
#define EN25QH256_IOW_DAT			SFLS_QUAD
#define EN25QH256_IOWE_CMD			SFLS_QUAD
#define EN25QH256_IORS_CMD			SFLS_QUAD
#define EN25QH256_IORS_DAT			SFLS_QUAD
#define EN25QH256_CMD_WRITE			0x02
#define EN25QH256_CMD_READ			0xEB
#endif
#endif

#define EN25QH256_CMD_FAST_READ		EN25QH256_CMD_READ
#define EN25QH256_CMD_PAGE_PROGRAM	EN25QH256_CMD_WRITE
#define EN25QH256_CMD_WRITE_ENABLE	0x06
#define EN25QH256_CMD_READ_STATUS	0x05
#define EN25QH256_CMD_WRITE_STATUS	0x01
#define EN25QH256_CMD_READ_IRF		0x2B
#define EN25QH256_CMD_READ_SFDP		0x5A
#define EN25QH256_CMD_ENTER_QPI		0x38
#define EN25QH256_CMD_EXIT_QPI		0xFF
#define EN25QH256_CMD_EN4B			0xB7
#define EN25QH256_CMD_EX4B			0xE9
#define EN25QH256_CMD_SECTOR_ERASE	0x20
#define EN25QH256_CMD_32KB_ERASE	0x52
#define EN25QH256_CMD_64KB_ERASE	0xD8
#define EN25QH256_CMD_CHIP_ERASE	0x60

extern void SflsEn25qh256_Init1(void);
extern void SflsEn25qh256_Init2(void);
extern void SflsEn25qh256_WriteEnable(void);
extern void SflsEn25qh256_SecterErase(UINT addr);
extern void SflsEn25qh256_32kErase(UINT addr);
extern void SflsEn25qh256_64kErase(UINT addr);
extern void SflsEn25qh256_ChipErase(UINT addr);
extern BYTE SflsEn25qh256_ReadIFR(void);
extern void SflsEn25qh256_ReadSFDP(BYTE *sfdp_bin);
extern BYTE SflsEn25qh256_ReadStatus(void);
extern void SflsEn25qh256_WriteStatus(BYTE status);
extern void SflsEn25qh256_Enter4B(void);
extern void SflsEn25qh256_Exit4B(void);
extern UINT SflsEn25qh256_Is4B(void);
extern void SflsEn25qh256_EnterQPI(void);
extern void SflsEn25qh256_ExitQPI(void);
extern UINT SflsEn25qh256_IsQPI(void);
extern void SflsEn25qh256_EnterQE(void);
extern void SflsEn25qh256_ExitQE(void);
extern UINT SflsEn25qh256_IsQE(void);
extern void SflsEn25qh256_EnterProtection(void);
extern void SflsEn25qh256_ExitProtection(void);
extern UINT SflsEn25qh256_IsProtection(void);

#if (EN25QH256_ENABLE_QPI==1)
#if (EN25QH256_IOMODE!=SFLS_QUAD)
#error "QPI mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (EN25QH256_ENABLE_QE==1)
#if (EN25QH256_IOMODE!=SFLS_QUAD)
#error "QE mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (EN25QH256_IOMODE==SFLS_QUAD)
#if (EN25QH256_ENABLE_QE==0)
#error "IO mode is QUAD mode but failed to activate. Set the serial flash memory to QE mode."
#endif
#endif

#endif // __EON_EN25QH256__H__
