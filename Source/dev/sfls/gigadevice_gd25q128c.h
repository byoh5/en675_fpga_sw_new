#ifndef __GIGADEVICE_GD25Q128C__H__
#define __GIGADEVICE_GD25Q128C__H__

////////////////////////////////////////////////////////////////////////////////
// RTL(200130_1405) - single(o) dual(o) quad(o) quad-qpi(o)
////////////////////////////////////////////////////////////////////////////////

#define GD25Q128C_SIZE					(16*1024*1024)

#define GD25Q128C_IOMODE				SFLS_QUAD // SFLS_SINGLE // SFLS_DUAL // SFLS_QUAD
#define GD25Q128C_ENABLE_QPI			1
#define GD25Q128C_ENABLE_QE				1

#if (GD25Q128C_IOMODE==SFLS_SINGLE)
#define GD25Q128C_RDLTC					1
#define GD25Q128C_READ_GAP				8
#define GD25Q128C_IOR_CMD				SFLS_SINGLE
#define GD25Q128C_IOR_ADR				SFLS_SINGLE
#define GD25Q128C_IOR_DAT				SFLS_SINGLE
#define GD25Q128C_IOW_CMD				SFLS_SINGLE
#define GD25Q128C_IOW_ADR				SFLS_SINGLE
#define GD25Q128C_IOW_DAT				SFLS_SINGLE
#define GD25Q128C_IOWE_CMD				SFLS_SINGLE
#define GD25Q128C_IORS_CMD				SFLS_SINGLE
#define GD25Q128C_IORS_DAT				SFLS_SINGLE
#define GD25Q128C_CMD_WRITE				0x02
#define GD25Q128C_CMD_READ				0x0B
#elif (GD25Q128C_IOMODE==SFLS_DUAL)
#define GD25Q128C_RDLTC					1
#define GD25Q128C_READ_GAP				7
#define GD25Q128C_IOR_CMD				SFLS_SINGLE
#define GD25Q128C_IOR_ADR				SFLS_DUAL
#define GD25Q128C_IOR_DAT				SFLS_DUAL
#define GD25Q128C_IOW_CMD				SFLS_SINGLE
#define GD25Q128C_IOW_ADR				SFLS_SINGLE
#define GD25Q128C_IOW_DAT				SFLS_SINGLE
#define GD25Q128C_IOWE_CMD				SFLS_SINGLE
#define GD25Q128C_IORS_CMD				SFLS_SINGLE
#define GD25Q128C_IORS_DAT				SFLS_SINGLE
#define GD25Q128C_CMD_WRITE				0x02
#define GD25Q128C_CMD_READ				0xBB
#elif (GD25Q128C_IOMODE==SFLS_QUAD)
#if (GD25Q128C_ENABLE_QPI==0)
#define GD25Q128C_RDLTC					1
#define GD25Q128C_READ_GAP				7
#define GD25Q128C_IOR_CMD				SFLS_SINGLE
#define GD25Q128C_IOR_ADR				SFLS_QUAD
#define GD25Q128C_IOR_DAT				SFLS_QUAD
#define GD25Q128C_IOW_CMD				SFLS_SINGLE
#define GD25Q128C_IOW_ADR				SFLS_SINGLE
#define GD25Q128C_IOW_DAT				SFLS_QUAD
#define GD25Q128C_IOWE_CMD				SFLS_SINGLE
#define GD25Q128C_IORS_CMD				SFLS_SINGLE
#define GD25Q128C_IORS_DAT				SFLS_SINGLE
#define GD25Q128C_CMD_WRITE				0x32
#define GD25Q128C_CMD_READ				0xEB
#else
#define GD25Q128C_RDLTC					2
#define GD25Q128C_READ_GAP				4
#define GD25Q128C_IOR_CMD				SFLS_QUAD
#define GD25Q128C_IOR_ADR				SFLS_QUAD
#define GD25Q128C_IOR_DAT				SFLS_QUAD
#define GD25Q128C_IOW_CMD				SFLS_QUAD
#define GD25Q128C_IOW_ADR				SFLS_QUAD
#define GD25Q128C_IOW_DAT				SFLS_QUAD
#define GD25Q128C_IOWE_CMD				SFLS_QUAD
#define GD25Q128C_IORS_CMD				SFLS_QUAD
#define GD25Q128C_IORS_DAT				SFLS_QUAD
#define GD25Q128C_CMD_WRITE				0x02
#define GD25Q128C_CMD_READ				0xEB
#endif
#endif

#define GD25Q128C_CMD_FAST_READ			GD25Q128C_CMD_READ
#define GD25Q128C_CMD_PAGE_PROGRAM		GD25Q128C_CMD_WRITE
#define GD25Q128C_CMD_WRITE_ENABLE		0x06
#define GD25Q128C_CMD_READ_STATUS1		0x05
#define GD25Q128C_CMD_READ_STATUS2		0x35
#define GD25Q128C_CMD_READ_STATUS3		0x15
#define GD25Q128C_CMD_WRITE_STATUS1		0x01
#define GD25Q128C_CMD_WRITE_STATUS2		0x31
#define GD25Q128C_CMD_WRITE_STATUS3		0x11
#define GD25Q128C_CMD_READ_SFDP			0x5A
#define GD25Q128C_CMD_ENTER_QPI			0x38
#define GD25Q128C_CMD_EXIT_QPI			0xFF
#define GD25Q128C_CMD_SECTOR_ERASE		0x20
#define GD25Q128C_CMD_32KB_ERASE		0x52
#define GD25Q128C_CMD_64KB_ERASE		0xD8
#define GD25Q128C_CMD_CHIP_ERASE		0x60

extern void SflsGd25q128c_Init1(void);
extern void SflsGd25q128c_Init2(void);
extern void SflsGd25q128c_WriteEnable(void);
extern void SflsGd25q128c_SecterErase(UINT addr);
extern void SflsGd25q128c_32kErase(UINT addr);
extern void SflsGd25q128c_64kErase(UINT addr);
extern void SflsGd25q128c_ChipErase(UINT addr);
extern void SflsGd25q128c_ReadSFDP(BYTE *sfdp_bin);
extern BYTE SflsGd25q128c_ReadStatus1(void);
extern BYTE SflsGd25q128c_ReadStatus2(void);
extern BYTE SflsGd25q128c_ReadStatus3(void);
extern void SflsGd25q128c_WriteStatus1(BYTE status);
extern void SflsGd25q128c_WriteStatus2(BYTE status);
extern void SflsGd25q128c_WriteStatus3(BYTE status);
extern void SflsGd25q128c_EnterQPI(void);
extern void SflsGd25q128c_ExitQPI(void);
extern UINT SflsGd25q128c_IsQPI(void);
extern void SflsGd25q128c_EnterQE(void);
extern void SflsGd25q128c_ExitQE(void);
extern UINT SflsGd25q128c_IsQE(void);
extern void SflsGd25q128c_EnterProtection(void);
extern void SflsGd25q128c_ExitProtection(void);
extern UINT SflsGd25q128c_IsProtection(void);

#if (GD25Q128C_ENABLE_QPI==1)
#if (GD25Q128C_IOMODE!=SFLS_QUAD)
#error "QPI mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (GD25Q128C_ENABLE_QE==1)
#if (GD25Q128C_IOMODE!=SFLS_QUAD)
#error "QE mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (GD25Q128C_IOMODE==SFLS_QUAD)
#if (GD25Q128C_ENABLE_QE==0)
#error "IO mode is QUAD mode but failed to activate. Set the serial flash memory to QE mode."
#endif
#endif

#endif // __GIGADEVICE_GD25Q128C__H__
