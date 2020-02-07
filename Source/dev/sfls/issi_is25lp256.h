#ifndef __ISSI_IS25LP256__H__
#define __ISSI_IS25LP256__H__

////////////////////////////////////////////////////////////////////////////////
// RTL(200130_1405) - single(o) dual(o) quad(o) quad-qpi(o)
////////////////////////////////////////////////////////////////////////////////

#define IS25LP256_SIZE					(32*1024*1024)

#define IS25LP256_IOMODE				SFLS_QUAD // SFLS_SINGLE // SFLS_DUAL // SFLS_QUAD
#define IS25LP256_ENABLE_QPI			0 // SFLS_QUAD only
#define IS25LP256_ENABLE_QE				1 // SFLS_QUAD only
#define IS25LP256_ENABLE_4B				1

#if (IS25LP256_IOMODE==SFLS_SINGLE)
#define IS25LP256_RDLTC					1
#define IS25LP256_READ_GAP				8
#define IS25LP256_IOR_CMD				SFLS_SINGLE
#define IS25LP256_IOR_ADR				SFLS_SINGLE
#define IS25LP256_IOR_DAT				SFLS_SINGLE
#define IS25LP256_IOW_CMD				SFLS_SINGLE
#define IS25LP256_IOW_ADR				SFLS_SINGLE
#define IS25LP256_IOW_DAT				SFLS_SINGLE
#define IS25LP256_IOWE_CMD				SFLS_SINGLE
#define IS25LP256_IORS_CMD				SFLS_SINGLE
#define IS25LP256_IORS_DAT				SFLS_SINGLE
#define IS25LP256_CMD_WRITE				0x02
#define IS25LP256_CMD_READ				0x0B
#elif (IS25LP256_IOMODE==SFLS_DUAL)
#define IS25LP256_RDLTC					1
#define IS25LP256_READ_GAP				11
#define IS25LP256_IOR_CMD				SFLS_SINGLE
#define IS25LP256_IOR_ADR				SFLS_SINGLE
#define IS25LP256_IOR_DAT				SFLS_DUAL
#define IS25LP256_IOW_CMD				SFLS_SINGLE
#define IS25LP256_IOW_ADR				SFLS_SINGLE
#define IS25LP256_IOW_DAT				SFLS_SINGLE
#define IS25LP256_IOWE_CMD				SFLS_SINGLE
#define IS25LP256_IORS_CMD				SFLS_SINGLE
#define IS25LP256_IORS_DAT				SFLS_SINGLE
#define IS25LP256_CMD_WRITE				0x02
#define IS25LP256_CMD_READ				0x3B
#elif (IS25LP256_IOMODE==SFLS_QUAD)
#if (IS25LP256_ENABLE_QPI==0)
#define IS25LP256_RDLTC					1
#define IS25LP256_READ_GAP				9
#define IS25LP256_IOR_CMD				SFLS_SINGLE
#define IS25LP256_IOR_ADR				SFLS_SINGLE
#define IS25LP256_IOR_DAT				SFLS_QUAD
#define IS25LP256_IOW_CMD				SFLS_SINGLE
#define IS25LP256_IOW_ADR				SFLS_SINGLE
#define IS25LP256_IOW_DAT				SFLS_QUAD
#define IS25LP256_IOWE_CMD				SFLS_SINGLE
#define IS25LP256_IORS_CMD				SFLS_SINGLE
#define IS25LP256_IORS_DAT				SFLS_SINGLE
#define IS25LP256_CMD_WRITE				0x32
#define IS25LP256_CMD_READ				0x6B
#else
#define IS25LP256_RDLTC					2
#define IS25LP256_READ_GAP				6
#define IS25LP256_IOR_CMD				SFLS_QUAD
#define IS25LP256_IOR_ADR				SFLS_QUAD
#define IS25LP256_IOR_DAT				SFLS_QUAD
#define IS25LP256_IOW_CMD				SFLS_QUAD
#define IS25LP256_IOW_ADR				SFLS_QUAD
#define IS25LP256_IOW_DAT				SFLS_QUAD
#define IS25LP256_IOWE_CMD				SFLS_QUAD
#define IS25LP256_IORS_CMD				SFLS_QUAD
#define IS25LP256_IORS_DAT				SFLS_QUAD
#define IS25LP256_CMD_WRITE				0x02
#define IS25LP256_CMD_READ				0xEB
#endif
#endif

#define IS25LP256_CMD_FAST_READ			IS25LP256_CMD_READ
#define IS25LP256_CMD_PAGE_PROGRAM		IS25LP256_CMD_WRITE
#define IS25LP256_CMD_WRITE_ENABLE		0x06
#define IS25LP256_CMD_READ_STATUS		0x05
#define IS25LP256_CMD_WRITE_STATUS		0x01
#define IS25LP256_CMD_READ_SFDP			0x5A
#define IS25LP256_CMD_ENTER_QPI			0x35
#define IS25LP256_CMD_EXIT_QPI			0xF5
#define IS25LP256_CMD_EN4B				0xB7
#define IS25LP256_CMD_EX4B				0x29
#define IS25LP256_CMD_SECTOR_ERASE		0xD7
#define IS25LP256_CMD_32KB_ERASE		0x52
#define IS25LP256_CMD_64KB_ERASE		0xD8
#define IS25LP256_CMD_CHIP_ERASE		0xC7

extern void SflsIs25lp256_Init1(void);
extern void SflsIs25lp256_Init2(void);
extern void SflsIs25lp256_WriteEnable(void);
extern void SflsIs25lp256_SecterErase(UINT addr);
extern void SflsIs25lp256_32kErase(UINT addr);
extern void SflsIs25lp256_64kErase(UINT addr);
extern void SflsIs25lp256_ChipErase(UINT addr);
extern void SflsIs25lp256_ReadSFDP(BYTE *sfdp_bin);
extern BYTE SflsIs25lp256_ReadStatus(void);
extern void SflsIs25lp256_WriteStatus(BYTE status);
extern void SflsIs25lp256_Enter4B(void);
extern void SflsIs25lp256_Exit4B(void);
extern UINT SflsIs25lp256_Is4B(void);
extern void SflsIs25lp256_EnterQPI(void);
extern void SflsIs25lp256_ExitQPI(void);
extern UINT SflsIs25lp256_IsQPI(void);
extern void SflsIs25lp256_EnterQE(void);
extern void SflsIs25lp256_ExitQE(void);
extern UINT SflsIs25lp256_IsQE(void);
extern void SflsIs25lp256_EnterProtection(void);
extern void SflsIs25lp256_ExitProtection(void);
extern UINT SflsIs25lp256_IsProtection(void);

#if (IS25LP256_ENABLE_QPI==1)
#if (IS25LP256_IOMODE!=SFLS_QUAD)
#error "QPI mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (IS25LP256_ENABLE_QE==1)
#if (IS25LP256_IOMODE!=SFLS_QUAD)
#error "QE mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (IS25LP256_IOMODE==SFLS_QUAD)
#if (IS25LP256_ENABLE_QE==0)
#error "IO mode is QUAD but failed to activate. Set the serial flash memory to QE mode."
#endif
#endif

#endif // __ISSI_IS25LP256__H__
