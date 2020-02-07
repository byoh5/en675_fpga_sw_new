#ifndef __WINBOND_W25Q256JVQ__H__
#define __WINBOND_W25Q256JVQ__H__

////////////////////////////////////////////////////////////////////////////////
// RTL(200130_1405) - single(o) dual(o) quad(o)
////////////////////////////////////////////////////////////////////////////////

#define W25Q256JVQ_SIZE						(32*1024*1024)

#define W25Q256JVQ_IOMODE					SFLS_QUAD // SFLS_SINGLE // SFLS_DUAL // SFLS_QUAD
#define W25Q256JVQ_ENABLE_QE				1
#define W25Q256JVQ_ENABLE_4B				1

#if (W25Q256JVQ_IOMODE==SFLS_SINGLE)
#define W25Q256JVQ_RDLTC					1
#define W25Q256JVQ_READ_GAP					8
#define W25Q256JVQ_IOR_CMD					SFLS_SINGLE
#define W25Q256JVQ_IOR_ADR					SFLS_SINGLE
#define W25Q256JVQ_IOR_DAT					SFLS_SINGLE
#define W25Q256JVQ_IOW_CMD					SFLS_SINGLE
#define W25Q256JVQ_IOW_ADR					SFLS_SINGLE
#define W25Q256JVQ_IOW_DAT					SFLS_SINGLE
#define W25Q256JVQ_IOWE_CMD					SFLS_SINGLE
#define W25Q256JVQ_IORS_CMD					SFLS_SINGLE
#define W25Q256JVQ_IORS_DAT					SFLS_SINGLE
#define W25Q256JVQ_CMD_READ					0x0B
#define W25Q256JVQ_CMD_WRITE				0x02
#elif (W25Q256JVQ_IOMODE==SFLS_DUAL)
#define W25Q256JVQ_RDLTC					1
#define W25Q256JVQ_READ_GAP					11
#define W25Q256JVQ_IOR_CMD					SFLS_SINGLE
#define W25Q256JVQ_IOR_ADR					SFLS_SINGLE
#define W25Q256JVQ_IOR_DAT					SFLS_DUAL
#define W25Q256JVQ_IOW_CMD					SFLS_SINGLE
#define W25Q256JVQ_IOW_ADR					SFLS_SINGLE
#define W25Q256JVQ_IOW_DAT					SFLS_SINGLE
#define W25Q256JVQ_IOWE_CMD					SFLS_SINGLE
#define W25Q256JVQ_IORS_CMD					SFLS_SINGLE
#define W25Q256JVQ_IORS_DAT					SFLS_SINGLE
#define W25Q256JVQ_CMD_READ					0x3B
#define W25Q256JVQ_CMD_WRITE				0x02
#elif (W25Q256JVQ_IOMODE==SFLS_QUAD)
#define W25Q256JVQ_RDLTC					1
#define W25Q256JVQ_READ_GAP					9
#define W25Q256JVQ_IOR_CMD					SFLS_SINGLE
#define W25Q256JVQ_IOR_ADR					SFLS_SINGLE
#define W25Q256JVQ_IOR_DAT					SFLS_QUAD
#define W25Q256JVQ_IOW_CMD					SFLS_SINGLE
#define W25Q256JVQ_IOW_ADR					SFLS_SINGLE
#define W25Q256JVQ_IOW_DAT					SFLS_QUAD
#define W25Q256JVQ_IOWE_CMD					SFLS_SINGLE
#define W25Q256JVQ_IORS_CMD					SFLS_SINGLE
#define W25Q256JVQ_IORS_DAT					SFLS_SINGLE
#define W25Q256JVQ_CMD_READ					0x6B
#define W25Q256JVQ_CMD_WRITE				0x32
#endif

#define W25Q256JVQ_CMD_FAST_READ			W25Q256JVQ_CMD_READ
#define W25Q256JVQ_CMD_PAGE_PROGRAM			W25Q256JVQ_CMD_WRITE
#define W25Q256JVQ_CMD_WRITE_ENABLE			0x06
#define W25Q256JVQ_CMD_READ_STATUS1			0x05
#define W25Q256JVQ_CMD_READ_STATUS2			0x35
#define W25Q256JVQ_CMD_READ_STATUS3			0x15
#define W25Q256JVQ_CMD_WRITE_STATUS1		0x01
#define W25Q256JVQ_CMD_WRITE_STATUS2		0x31
#define W25Q256JVQ_CMD_WRITE_STATUS3		0x11
#define W25Q256JVQ_CMD_READ_SFDP			0x5A
#define W25Q256JVQ_CMD_EN4B					0xB7
#define W25Q256JVQ_CMD_EX4B					0xE9
#define W25Q256JVQ_CMD_SECTOR_ERASE			0x20
#define W25Q256JVQ_CMD_32KB_ERASE			0x52
#define W25Q256JVQ_CMD_64KB_ERASE			0xD8
#define W25Q256JVQ_CMD_CHIP_ERASE			0x60

extern void SflsW25q256jvq_Init1(void);
extern void SflsW25q256jvq_Init2(void);
extern void SflsW25q256jvq_WriteEnable(void);
extern void SflsW25q256jvq_SecterErase(UINT addr);
extern void SflsW25q256jvq_32kErase(UINT addr);
extern void SflsW25q256jvq_64kErase(UINT addr);
extern void SflsW25q256jvq_ChipErase(UINT addr);
extern void SflsW25q256jvq_ReadSFDP(BYTE *sfdp_bin);
extern BYTE SflsW25q256jvq_ReadStatus1(void);
extern BYTE SflsW25q256jvq_ReadStatus2(void);
extern BYTE SflsW25q256jvq_ReadStatus3(void);
extern void SflsW25q256jvq_WriteStatus1(BYTE status);
extern void SflsW25q256jvq_WriteStatus2(BYTE status);
extern void SflsW25q256jvq_WriteStatus3(BYTE status);
extern void SflsW25q256jvq_Enter4B(void);
extern void SflsW25q256jvq_Exit4B(void);
extern UINT SflsW25q256jvq_Is4B(void);
extern void SflsW25q256jvq_EnterQE(void);
extern void SflsW25q256jvq_ExitQE(void);
extern UINT SflsW25q256jvq_IsQE(void);
extern void SflsW25q256jvq_EnterProtection(void);
extern void SflsW25q256jvq_ExitProtection(void);
extern UINT SflsW25q256jvq_IsProtection(void);

#if (W25Q256JVQ_ENABLE_QE==1)
#if (W25Q256JVQ_IOMODE!=SFLS_QUAD)
#error "QE mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (W25Q256JVQ_IOMODE==SFLS_QUAD)
#if (W25Q256JVQ_ENABLE_QE==0)
#error "IO mode is QUAD but failed to activate. Set the serial flash memory to QE mode."
#endif
#endif

#endif // __WINBOND_W25Q256JVQ__H__
