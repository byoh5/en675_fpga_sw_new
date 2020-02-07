#ifndef __EON_EN25QH128A__H__
#define __EON_EN25QH128A__H__

////////////////////////////////////////////////////////////////////////////////
// RTL(200130_1405) - single(o) dual(o) quad(o) quad-qpi(o)
////////////////////////////////////////////////////////////////////////////////

#define EN25QH128A_SIZE				(16*1024*1024)

#define EN25QH128A_IOMODE			SFLS_QUAD // SFLS_SINGLE // SFLS_DUAL // SFLS_QUAD
#define EN25QH128A_ENABLE_QPI		1

#if (EN25QH128A_IOMODE==SFLS_SINGLE)
#define EN25QH128A_RDLTC			1
#define EN25QH128A_READ_GAP			8
#define EN25QH128A_IOR_CMD			SFLS_SINGLE
#define EN25QH128A_IOR_ADR			SFLS_SINGLE
#define EN25QH128A_IOR_DAT			SFLS_SINGLE
#define EN25QH128A_IOW_CMD			SFLS_SINGLE
#define EN25QH128A_IOW_ADR			SFLS_SINGLE
#define EN25QH128A_IOW_DAT			SFLS_SINGLE
#define EN25QH128A_IOWE_CMD			SFLS_SINGLE
#define EN25QH128A_IORS_CMD			SFLS_SINGLE
#define EN25QH128A_IORS_DAT			SFLS_SINGLE
#define EN25QH128A_CMD_WRITE		0x02
#define EN25QH128A_CMD_READ			0x0B
#elif (EN25QH128A_IOMODE==SFLS_DUAL)
#define EN25QH128A_RDLTC			1
#define EN25QH128A_READ_GAP			7
#define EN25QH128A_IOR_CMD			SFLS_SINGLE
#define EN25QH128A_IOR_ADR			SFLS_DUAL
#define EN25QH128A_IOR_DAT			SFLS_DUAL
#define EN25QH128A_IOW_CMD			SFLS_SINGLE
#define EN25QH128A_IOW_ADR			SFLS_SINGLE
#define EN25QH128A_IOW_DAT			SFLS_SINGLE
#define EN25QH128A_IOWE_CMD			SFLS_SINGLE
#define EN25QH128A_IORS_CMD			SFLS_SINGLE
#define EN25QH128A_IORS_DAT			SFLS_SINGLE
#define EN25QH128A_CMD_WRITE		0x02
#define EN25QH128A_CMD_READ			0xBB
#elif (EN25QH128A_IOMODE==SFLS_QUAD)
#if (EN25QH128A_ENABLE_QPI==0)
#define EN25QH128A_RDLTC			1
#define EN25QH128A_READ_GAP			7
#define EN25QH128A_IOR_CMD			SFLS_SINGLE
#define EN25QH128A_IOR_ADR			SFLS_QUAD
#define EN25QH128A_IOR_DAT			SFLS_QUAD
#define EN25QH128A_IOW_CMD			SFLS_SINGLE
#define EN25QH128A_IOW_ADR			SFLS_SINGLE
#define EN25QH128A_IOW_DAT			SFLS_SINGLE
#define EN25QH128A_IOWE_CMD			SFLS_SINGLE
#define EN25QH128A_IORS_CMD			SFLS_SINGLE
#define EN25QH128A_IORS_DAT			SFLS_SINGLE
#define EN25QH128A_CMD_WRITE		0x02
#define EN25QH128A_CMD_READ			0xEB
#else
#define EN25QH128A_RDLTC			2
#define EN25QH128A_READ_GAP			6
#define EN25QH128A_IOR_CMD			SFLS_QUAD
#define EN25QH128A_IOR_ADR			SFLS_QUAD
#define EN25QH128A_IOR_DAT			SFLS_QUAD
#define EN25QH128A_IOW_CMD			SFLS_QUAD
#define EN25QH128A_IOW_ADR			SFLS_QUAD
#define EN25QH128A_IOW_DAT			SFLS_QUAD
#define EN25QH128A_IOWE_CMD			SFLS_QUAD
#define EN25QH128A_IORS_CMD			SFLS_QUAD
#define EN25QH128A_IORS_DAT			SFLS_QUAD
#define EN25QH128A_CMD_WRITE		0x02
#define EN25QH128A_CMD_READ			0xEB
#endif
#endif

#define EN25QH128A_CMD_FAST_READ	EN25QH128A_CMD_READ
#define EN25QH128A_CMD_PAGE_PROGRAM	EN25QH128A_CMD_WRITE
#define EN25QH128A_CMD_WRITE_ENABLE	0x06
#define EN25QH128A_CMD_READ_STATUS	0x05
#define EN25QH128A_CMD_WRITE_STATUS	0x01
#define EN25QH128A_CMD_READ_SFDP	0x5A
#define EN25QH128A_CMD_ENTER_QPI	0x38
#define EN25QH128A_CMD_EXIT_QPI		0xFF
#define EN25QH128A_CMD_SECTOR_ERASE	0x20
#define EN25QH128A_CMD_32KB_ERASE	0x52
#define EN25QH128A_CMD_64KB_ERASE	0xD8
#define EN25QH128A_CMD_CHIP_ERASE	0x60

extern void SflsEn25qh128a_Init1(void);
extern void SflsEn25qh128a_Init2(void);
extern void SflsEn25qh128a_WriteEnable(void);
extern void SflsEn25qh128a_SecterErase(UINT addr);
extern void SflsEn25qh128a_32kErase(UINT addr);
extern void SflsEn25qh128a_64kErase(UINT addr);
extern void SflsEn25qh128a_ChipErase(UINT addr);
extern void SflsEn25qh128a_ReadSFDP(BYTE *sfdp_bin);
extern BYTE SflsEn25qh128a_ReadStatus(void);
extern void SflsEn25qh128a_WriteStatus(BYTE status);
extern void SflsEn25qh128a_EnterQPI(void);
extern void SflsEn25qh128a_ExitQPI(void);
extern UINT SflsEn25qh128a_IsQPI(void);
extern void SflsEn25qh128a_EnterProtection(void);
extern void SflsEn25qh128a_ExitProtection(void);
extern UINT SflsEn25qh128a_IsProtection(void);

#if (EN25QH128A_ENABLE_QPI==1)
#if (EN25QH128A_IOMODE!=SFLS_QUAD)
#error "QPI mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#endif // __EON_EN25QH128A__H__
