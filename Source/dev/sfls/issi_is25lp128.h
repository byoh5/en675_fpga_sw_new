#ifndef __ISSI_IS25LP128__H__
#define __ISSI_IS25LP128__H__

////////////////////////////////////////////////////////////////////////////////
// RTL(200130_1405) - single(o) dual(o) quad(o) quad-qpi(o)
////////////////////////////////////////////////////////////////////////////////

#define IS25LP128_SIZE					(16*1024*1024)

#define IS25LP128_IOMODE				SFLS_QUAD // SFLS_SINGLE // SFLS_DUAL // SFLS_QUAD
#define IS25LP128_ENABLE_QPI			0
#define IS25LP128_ENABLE_QE				1

#if (IS25LP128_IOMODE==SFLS_SINGLE)
#define IS25LP128_RDLTC					1
#define IS25LP128_READ_GAP				8
#define IS25LP128_IOR_CMD				SFLS_SINGLE
#define IS25LP128_IOR_ADR				SFLS_SINGLE
#define IS25LP128_IOR_DAT				SFLS_SINGLE
#define IS25LP128_IOW_CMD				SFLS_SINGLE
#define IS25LP128_IOW_ADR				SFLS_SINGLE
#define IS25LP128_IOW_DAT				SFLS_SINGLE
#define IS25LP128_IOWE_CMD				SFLS_SINGLE
#define IS25LP128_IORS_CMD				SFLS_SINGLE
#define IS25LP128_IORS_DAT				SFLS_SINGLE
#define IS25LP128_CMD_WRITE				0x02
#define IS25LP128_CMD_READ				0x0B
#elif (IS25LP128_IOMODE==SFLS_DUAL)
#define IS25LP128_RDLTC					1
#define IS25LP128_READ_GAP				7
#define IS25LP128_IOR_CMD				SFLS_SINGLE
#define IS25LP128_IOR_ADR				SFLS_DUAL
#define IS25LP128_IOR_DAT				SFLS_DUAL
#define IS25LP128_IOW_CMD				SFLS_SINGLE
#define IS25LP128_IOW_ADR				SFLS_SINGLE
#define IS25LP128_IOW_DAT				SFLS_SINGLE
#define IS25LP128_IOWE_CMD				SFLS_SINGLE
#define IS25LP128_IORS_CMD				SFLS_SINGLE
#define IS25LP128_IORS_DAT				SFLS_SINGLE
#define IS25LP128_CMD_WRITE				0x02
#define IS25LP128_CMD_READ				0xBB
#elif (IS25LP128_IOMODE==SFLS_QUAD)
#if (IS25LP128_ENABLE_QPI==0)
#define IS25LP128_RDLTC					1
#define IS25LP128_READ_GAP				7
#define IS25LP128_IOR_CMD				SFLS_SINGLE
#define IS25LP128_IOR_ADR				SFLS_QUAD
#define IS25LP128_IOR_DAT				SFLS_QUAD
#define IS25LP128_IOW_CMD				SFLS_SINGLE
#define IS25LP128_IOW_ADR				SFLS_SINGLE
#define IS25LP128_IOW_DAT				SFLS_QUAD
#define IS25LP128_IOWE_CMD				SFLS_SINGLE
#define IS25LP128_IORS_CMD				SFLS_SINGLE
#define IS25LP128_IORS_DAT				SFLS_SINGLE
#define IS25LP128_CMD_WRITE				0x32
#define IS25LP128_CMD_READ				0xEB
#else
#define IS25LP128_RDLTC					2
#define IS25LP128_READ_GAP				6
#define IS25LP128_IOR_CMD				SFLS_QUAD
#define IS25LP128_IOR_ADR				SFLS_QUAD
#define IS25LP128_IOR_DAT				SFLS_QUAD
#define IS25LP128_IOW_CMD				SFLS_QUAD
#define IS25LP128_IOW_ADR				SFLS_QUAD
#define IS25LP128_IOW_DAT				SFLS_QUAD
#define IS25LP128_IOWE_CMD				SFLS_QUAD
#define IS25LP128_IORS_CMD				SFLS_QUAD
#define IS25LP128_IORS_DAT				SFLS_QUAD
#define IS25LP128_CMD_WRITE				0x02
#define IS25LP128_CMD_READ				0xEB
#endif
#endif

#define IS25LP128_CMD_FAST_READ			IS25LP128_CMD_READ
#define IS25LP128_CMD_PAGE_PROGRAM		IS25LP128_CMD_WRITE
#define IS25LP128_CMD_WRITE_ENABLE		0x06
#define IS25LP128_CMD_READ_STATUS		0x05
#define IS25LP128_CMD_WRITE_STATUS		0x01
#define IS25LP128_CMD_READ_SFDP			0x5A
#define IS25LP128_CMD_ENTER_QPI			0x35
#define IS25LP128_CMD_EXIT_QPI			0xF5
#define IS25LP128_CMD_SECTOR_ERASE		0x20
#define IS25LP128_CMD_32KB_ERASE		0x52
#define IS25LP128_CMD_64KB_ERASE		0xD8
#define IS25LP128_CMD_CHIP_ERASE		0xC7

extern void SflsIs25lp128_Init1(void);
extern void SflsIs25lp128_Init2(void);
extern void SflsIs25lp128_WriteEnable(void);
extern void SflsIs25lp128_SecterErase(UINT addr);
extern void SflsIs25lp128_32kErase(UINT addr);
extern void SflsIs25lp128_64kErase(UINT addr);
extern void SflsIs25lp128_ChipErase(UINT addr);
extern void SflsIs25lp128_ReadSFDP(BYTE *sfdp_bin);
extern BYTE SflsIs25lp128_ReadStatus(void);
extern void SflsIs25lp128_WriteStatus(BYTE status);
extern void SflsIs25lp128_EnterQPI(void);
extern void SflsIs25lp128_ExitQPI(void);
extern UINT SflsIs25lp128_IsQPI(void);
extern void SflsIs25lp128_EnterQE(void);
extern void SflsIs25lp128_ExitQE(void);
extern UINT SflsIs25lp128_IsQE(void);
extern void SflsIs25lp128_EnterProtection(void);
extern void SflsIs25lp128_ExitProtection(void);
extern UINT SflsIs25lp128_IsProtection(void);

#if (IS25LP128_ENABLE_QPI==1)
#if (IS25LP128_IOMODE!=SFLS_QUAD)
#error "QPI mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (IS25LP128_ENABLE_QE==1)
#if (IS25LP128_IOMODE!=SFLS_QUAD)
#error "QE mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (IS25LP128_IOMODE==SFLS_QUAD)
#if (IS25LP128_ENABLE_QE==0)
#error "IO mode is QUAD but failed to activate. Set the serial flash memory to QE mode."
#endif
#endif

#endif // __ISSI_IS25LP128__H__
