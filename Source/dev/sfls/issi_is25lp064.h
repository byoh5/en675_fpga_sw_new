#ifndef __ISSI_IS25LP064__H__
#define __ISSI_IS25LP064__H__

////////////////////////////////////////////////////////////////////////////////
// RTL(200130_1405) - single(o) dual(-) quad(-) quad-qpi(-)
////////////////////////////////////////////////////////////////////////////////

#define IS25LP064_SIZE					(8*1024*1024)

#define IS25LP064_IOMODE				SFLS_QUAD // SFLS_SINGLE // SFLS_DUAL // SFLS_QUAD
#define IS25LP064_ENABLE_QPI			0
#define IS25LP064_ENABLE_QE				1

#if (IS25LP064_IOMODE==SFLS_SINGLE)
#define IS25LP064_RDLTC					1
#define IS25LP064_READ_GAP				8
#define IS25LP064_IOR_CMD				SFLS_SINGLE
#define IS25LP064_IOR_ADR				SFLS_SINGLE
#define IS25LP064_IOR_DAT				SFLS_SINGLE
#define IS25LP064_IOW_CMD				SFLS_SINGLE
#define IS25LP064_IOW_ADR				SFLS_SINGLE
#define IS25LP064_IOW_DAT				SFLS_SINGLE
#define IS25LP064_IOWE_CMD				SFLS_SINGLE
#define IS25LP064_IORS_CMD				SFLS_SINGLE
#define IS25LP064_IORS_DAT				SFLS_SINGLE
#define IS25LP064_CMD_WRITE				0x02
#define IS25LP064_CMD_READ				0x0B
#elif (IS25LP064_IOMODE==SFLS_DUAL)
#define IS25LP064_RDLTC					1
#define IS25LP064_READ_GAP				7
#define IS25LP064_IOR_CMD				SFLS_SINGLE
#define IS25LP064_IOR_ADR				SFLS_DUAL
#define IS25LP064_IOR_DAT				SFLS_DUAL
#define IS25LP064_IOW_CMD				SFLS_SINGLE
#define IS25LP064_IOW_ADR				SFLS_SINGLE
#define IS25LP064_IOW_DAT				SFLS_SINGLE
#define IS25LP064_IOWE_CMD				SFLS_SINGLE
#define IS25LP064_IORS_CMD				SFLS_SINGLE
#define IS25LP064_IORS_DAT				SFLS_SINGLE
#define IS25LP064_CMD_WRITE				0x02
#define IS25LP064_CMD_READ				0xBB
#elif (IS25LP064_IOMODE==SFLS_QUAD)
#if (IS25LP064_ENABLE_QPI==0)
#define IS25LP064_RDLTC					1
#define IS25LP064_READ_GAP				7
#define IS25LP064_IOR_CMD				SFLS_SINGLE
#define IS25LP064_IOR_ADR				SFLS_QUAD
#define IS25LP064_IOR_DAT				SFLS_QUAD
#define IS25LP064_IOW_CMD				SFLS_SINGLE
#define IS25LP064_IOW_ADR				SFLS_SINGLE
#define IS25LP064_IOW_DAT				SFLS_QUAD
#define IS25LP064_IOWE_CMD				SFLS_SINGLE
#define IS25LP064_IORS_CMD				SFLS_SINGLE
#define IS25LP064_IORS_DAT				SFLS_SINGLE
#define IS25LP064_CMD_WRITE				0x32
#define IS25LP064_CMD_READ				0xEB
#else
#define IS25LP064_RDLTC					2
#define IS25LP064_READ_GAP				6
#define IS25LP064_IOR_CMD				SFLS_QUAD
#define IS25LP064_IOR_ADR				SFLS_QUAD
#define IS25LP064_IOR_DAT				SFLS_QUAD
#define IS25LP064_IOW_CMD				SFLS_QUAD
#define IS25LP064_IOW_ADR				SFLS_QUAD
#define IS25LP064_IOW_DAT				SFLS_QUAD
#define IS25LP064_IOWE_CMD				SFLS_QUAD
#define IS25LP064_IORS_CMD				SFLS_QUAD
#define IS25LP064_IORS_DAT				SFLS_QUAD
#define IS25LP064_CMD_WRITE				0x02
#define IS25LP064_CMD_READ				0xEB
#endif
#endif

#define IS25LP064_CMD_FAST_READ			IS25LP064_CMD_READ
#define IS25LP064_CMD_PAGE_PROGRAM		IS25LP064_CMD_WRITE
#define IS25LP064_CMD_WRITE_ENABLE		0x06
#define IS25LP064_CMD_READ_STATUS		0x05
#define IS25LP064_CMD_WRITE_STATUS		0x01
#define IS25LP064_CMD_READ_SFDP			0x5A
#define IS25LP064_CMD_ENTER_QPI			0x35
#define IS25LP064_CMD_EXIT_QPI			0xF5
#define IS25LP064_CMD_SECTOR_ERASE		0x20
#define IS25LP064_CMD_32KB_ERASE		0x52
#define IS25LP064_CMD_64KB_ERASE		0xD8
#define IS25LP064_CMD_CHIP_ERASE		0xC7

extern void SflsIs25lp064_Init1(void);
extern void SflsIs25lp064_Init2(void);
extern void SflsIs25lp064_WriteEnable(void);
extern void SflsIs25lp064_SecterErase(UINT addr);
extern void SflsIs25lp064_32kErase(UINT addr);
extern void SflsIs25lp064_64kErase(UINT addr);
extern void SflsIs25lp064_ChipErase(UINT addr);
extern void SflsIs25lp064_ReadSFDP(BYTE *sfdp_bin);
extern BYTE SflsIs25lp064_ReadStatus(void);
extern void SflsIs25lp064_WriteStatus(BYTE status);
extern void SflsIs25lp064_EnterQPI(void);
extern void SflsIs25lp064_ExitQPI(void);
extern UINT SflsIs25lp064_IsQPI(void);
extern void SflsIs25lp064_EnterQE(void);
extern void SflsIs25lp064_ExitQE(void);
extern UINT SflsIs25lp064_IsQE(void);
extern void SflsIs25lp064_EnterProtection(void);
extern void SflsIs25lp064_ExitProtection(void);
extern UINT SflsIs25lp064_IsProtection(void);

#if (IS25LP064_ENABLE_QPI==1)
#if (IS25LP064_IOMODE!=SFLS_QUAD)
#error "QPI mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (IS25LP064_ENABLE_QE==1)
#if (IS25LP064_IOMODE!=SFLS_QUAD)
#error "QE mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (IS25LP064_IOMODE==SFLS_QUAD)
#if (IS25LP064_ENABLE_QE==0)
#error "IO mode is QUAD but failed to activate. Set the serial flash memory to QE mode."
#endif
#endif

#endif // __ISSI_IS25LP064__H__
