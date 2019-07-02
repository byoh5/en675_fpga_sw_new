#ifndef __ISSI_IS25LP128__H__
#define __ISSI_IS25LP128__H__

#define IS25LP128_IOMODE			SFLS_SINGLE // SFLS_SINGLE // SFLS_DUAL // SFLS_QUAD
#define IS25LP128_ENABLE_QPI		0

#if (IS25LP128_IOMODE==SFLS_SINGLE)
#define IS25LP128_RDLTC				1
#define IS25LP128_READ_GAP			8
#define IS25LP128_IOR_CMD			SFLS_SINGLE
#define IS25LP128_IOR_ADR			SFLS_SINGLE
#define IS25LP128_IOR_DAT			SFLS_SINGLE
#define IS25LP128_IOW_CMD			SFLS_SINGLE
#define IS25LP128_IOW_ADR			SFLS_SINGLE
#define IS25LP128_IOW_DAT			SFLS_SINGLE
#define IS25LP128_IOWE_CMD			SFLS_SINGLE
#define IS25LP128_IORS_CMD			SFLS_SINGLE
#define IS25LP128_IORS_DAT			SFLS_SINGLE
#define IS25LP128_CMD_PAGE_PROGRAM	0x02
#elif (IS25LP128_IOMODE==SFLS_DUAL)
#define IS25LP128_RDLTC				1
#define IS25LP128_READ_GAP			7
#define IS25LP128_IOR_CMD			SFLS_SINGLE
#define IS25LP128_IOR_ADR			SFLS_DUAL
#define IS25LP128_IOR_DAT			SFLS_DUAL
#define IS25LP128_IOW_CMD			SFLS_SINGLE
#define IS25LP128_IOW_ADR			SFLS_SINGLE
#define IS25LP128_IOW_DAT			SFLS_SINGLE
#define IS25LP128_IOWE_CMD			SFLS_SINGLE
#define IS25LP128_IORS_CMD			SFLS_SINGLE
#define IS25LP128_IORS_DAT			SFLS_SINGLE
#define IS25LP128_CMD_PAGE_PROGRAM	0x02
#elif (IS25LP128_IOMODE==SFLS_QUAD)
#if (IS25LP128_ENABLE_QPI==0)
#define IS25LP128_RDLTC				1
#define IS25LP128_READ_GAP			7
#define IS25LP128_IOR_CMD			SFLS_SINGLE
#define IS25LP128_IOR_ADR			SFLS_QUAD
#define IS25LP128_IOR_DAT			SFLS_QUAD
#define IS25LP128_IOW_CMD			SFLS_SINGLE
#define IS25LP128_IOW_ADR			SFLS_SINGLE
#define IS25LP128_IOW_DAT			SFLS_QUAD
#define IS25LP128_IOWE_CMD			SFLS_SINGLE
#define IS25LP128_IORS_CMD			SFLS_SINGLE
#define IS25LP128_IORS_DAT			SFLS_SINGLE
#define IS25LP128_CMD_PAGE_PROGRAM	0x32
#else
#define IS25LP128_RDLTC				1
#define IS25LP128_READ_GAP			4
#define IS25LP128_IOR_CMD			SFLS_QUAD
#define IS25LP128_IOR_ADR			SFLS_QUAD
#define IS25LP128_IOR_DAT			SFLS_QUAD
#define IS25LP128_IOW_CMD			SFLS_QUAD
#define IS25LP128_IOW_ADR			SFLS_QUAD
#define IS25LP128_IOW_DAT			SFLS_QUAD
#define IS25LP128_IOWE_CMD			SFLS_QUAD
#define IS25LP128_IORS_CMD			SFLS_QUAD
#define IS25LP128_IORS_DAT			SFLS_QUAD
#define IS25LP128_CMD_PAGE_PROGRAM	0x02
#endif
#endif

#define IS25LP128_CMD_READ			0x0B
#define IS25LP128_CMD_DUAL_READ		0xBB
#define IS25LP128_CMD_QUAD_READ		0xEB
//#define IS25LP128_CMD_PAGE_PROGRAM	0x02
#define IS25LP128_CMD_WRITE_ENABLE	0x06
#define IS25LP128_CMD_READ_STATUS	0x05
#define IS25LP128_CMD_ENTER_QPI		0x38
#define IS25LP128_CMD_SECTOR_ERASE	0x20
#define IS25LP128_CMD_32KB_ERASE	0x52
#define IS25LP128_CMD_64KB_ERASE	0xD8
#define IS25LP128_CMD_CHIP_ERASE	0xC7

extern void SflsIs25lp128_Init(void);
extern void SflsIs25lp128_WriteEnable(void);
extern void SflsIs25lp128_SecterErase(UINT addr);
extern void SflsIs25lp128_32kErase(UINT addr);
extern void SflsIs25lp128_64kErase(UINT addr);
extern void SflsIs25lp128_ChipErase(UINT addr);

#if (EN25QH128A_ENABLE_QPI==1)
#if (EN25QH128A_IOMODE!=SFLS_QUAD)
#error "QPI mode is enabled. Set the serial flash memory to quad mode."
#endif
#endif

#endif // __ISSI_IS25LP128__H__
