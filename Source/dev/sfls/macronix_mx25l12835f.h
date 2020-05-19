#ifndef __MACRONIX_MX25L12835F__H__
#define __MACRONIX_MX25L12835F__H__

////////////////////////////////////////////////////////////////////////////////
// RTL(200130_1405) - single(o) dual(o) quad(o) quad-qpi(o)
////////////////////////////////////////////////////////////////////////////////

#define MX25L12835F_SIZE				(16*1024*1024)

#define MX25L12835F_IOMODE				SFLS_SINGLE // SFLS_SINGLE // SFLS_DUAL // SFLS_QUAD
#define MX25L12835F_ENABLE_QPI			0 // SFLS_QUAD only
#define MX25L12835F_ENABLE_QE			0 // SFLS_QUAD only

#if (MX25L12835F_IOMODE==SFLS_SINGLE)
#define MX25L12835F_RDLTC				1
#define MX25L12835F_READ_GAP			8
#define MX25L12835F_IOR_CMD				SFLS_SINGLE
#define MX25L12835F_IOR_ADR				SFLS_SINGLE
#define MX25L12835F_IOR_DAT				SFLS_SINGLE
#define MX25L12835F_IOW_CMD				SFLS_SINGLE
#define MX25L12835F_IOW_ADR				SFLS_SINGLE
#define MX25L12835F_IOW_DAT				SFLS_SINGLE
#define MX25L12835F_IOWE_CMD			SFLS_SINGLE
#define MX25L12835F_IORS_CMD			SFLS_SINGLE
#define MX25L12835F_IORS_DAT			SFLS_SINGLE
#define MX25L12835F_CMD_WRITE			0x02
#define MX25L12835F_CMD_READ			0x0B
#elif (MX25L12835F_IOMODE==SFLS_DUAL)
#define MX25L12835F_RDLTC				1
#define MX25L12835F_READ_GAP			11
#define MX25L12835F_IOR_CMD				SFLS_SINGLE
#define MX25L12835F_IOR_ADR				SFLS_SINGLE
#define MX25L12835F_IOR_DAT				SFLS_DUAL
#define MX25L12835F_IOW_CMD				SFLS_SINGLE
#define MX25L12835F_IOW_ADR				SFLS_SINGLE
#define MX25L12835F_IOW_DAT				SFLS_SINGLE
#define MX25L12835F_IOWE_CMD			SFLS_SINGLE
#define MX25L12835F_IORS_CMD			SFLS_SINGLE
#define MX25L12835F_IORS_DAT			SFLS_SINGLE
#define MX25L12835F_CMD_WRITE			0x02
#define MX25L12835F_CMD_READ			0x3B
#elif (MX25L12835F_IOMODE==SFLS_QUAD)
#if (MX25L12835F_ENABLE_QPI==0)
#define MX25L12835F_RDLTC				1
#define MX25L12835F_READ_GAP			7
#define MX25L12835F_IOR_CMD				SFLS_SINGLE
#define MX25L12835F_IOR_ADR				SFLS_QUAD
#define MX25L12835F_IOR_DAT				SFLS_QUAD
#define MX25L12835F_IOW_CMD				SFLS_SINGLE
#define MX25L12835F_IOW_ADR				SFLS_SINGLE
#define MX25L12835F_IOW_DAT				SFLS_QUAD
#define MX25L12835F_IOWE_CMD			SFLS_SINGLE
#define MX25L12835F_IORS_CMD			SFLS_SINGLE
#define MX25L12835F_IORS_DAT			SFLS_SINGLE
#define MX25L12835F_CMD_WRITE			0x32
#define MX25L12835F_CMD_READ			0xEB
#else
#define MX25L12835F_RDLTC				2
#define MX25L12835F_READ_GAP			6
#define MX25L12835F_IOR_CMD				SFLS_QUAD
#define MX25L12835F_IOR_ADR				SFLS_QUAD
#define MX25L12835F_IOR_DAT				SFLS_QUAD
#define MX25L12835F_IOW_CMD				SFLS_QUAD
#define MX25L12835F_IOW_ADR				SFLS_QUAD
#define MX25L12835F_IOW_DAT				SFLS_QUAD
#define MX25L12835F_IOWE_CMD			SFLS_QUAD
#define MX25L12835F_IORS_CMD			SFLS_QUAD
#define MX25L12835F_IORS_DAT			SFLS_QUAD
#define MX25L12835F_CMD_WRITE			0x02
#define MX25L12835F_CMD_READ			0xEB
#endif
#endif

#define MX25L12835F_CMD_FAST_READ		MX25L12835F_CMD_READ
#define MX25L12835F_CMD_PAGE_PROGRAM	MX25L12835F_CMD_WRITE
#define MX25L12835F_CMD_WRITE_ENABLE	0x06
#define MX25L12835F_CMD_READ_STATUS		0x05
#define MX25L12835F_CMD_WRITE_STATUS	0x01
#define MX25L12835F_CMD_READ_SFDP		0x5A
#define MX25L12835F_CMD_ENTER_QPI		0x35
#define MX25L12835F_CMD_EXIT_QPI		0xF5
#define MX25L12835F_CMD_SECTOR_ERASE	0x20
#define MX25L12835F_CMD_32KB_ERASE		0x52
#define MX25L12835F_CMD_64KB_ERASE		0xD8
#define MX25L12835F_CMD_CHIP_ERASE		0x60

extern void SflsMx25l12835f_Init1(void);
extern void SflsMx25l12835f_Init2(void);
extern void SflsMx25l12835f_WriteEnable(void);
extern void SflsMx25l12835f_SecterErase(UINT addr);
extern void SflsMx25l12835f_32kErase(UINT addr);
extern void SflsMx25l12835f_64kErase(UINT addr);
extern void SflsMx25l12835f_ChipErase(UINT addr);
extern void SflsMx25l12835f_ReadSFDP(BYTE *sfdp_bin);
extern BYTE SflsMx25l12835f_ReadStatus(void);
extern void SflsMx25l12835f_WriteStatus(BYTE status);
extern void SflsMx25l12835f_EnterQPI(void);
extern void SflsMx25l12835f_ExitQPI(void);
extern UINT SflsMx25l12835f_IsQPI(void);
extern void SflsMx25l12835f_EnterQE(void);
extern void SflsMx25l12835f_ExitQE(void);
extern UINT SflsMx25l12835f_IsQE(void);
extern void SflsMx25l12835f_EnterProtection(void);
extern void SflsMx25l12835f_ExitProtection(void);
extern UINT SflsMx25l12835f_IsProtection(void);

#if (MX25L12835F_ENABLE_QPI==1)
#if (MX25L12835F_IOMODE!=SFLS_QUAD)
#error "QPI mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (MX25L12835F_ENABLE_QE==1)
#if (MX25L12835F_IOMODE!=SFLS_QUAD)
#error "QE mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (MX25L12835F_IOMODE==SFLS_QUAD)
#if (MX25L12835F_ENABLE_QE==0)
#error "IO mode is QUAD but failed to activate. Set the serial flash memory to QE mode."
#endif
#endif

#endif // __MACRONIX_MX25L12835F__H__
