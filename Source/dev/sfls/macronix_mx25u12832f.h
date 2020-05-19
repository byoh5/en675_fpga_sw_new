#ifndef __MACRONIX_MX25U12832F__H__
#define __MACRONIX_MX25U12832F__H__

////////////////////////////////////////////////////////////////////////////////
// RTL(200118_1937) - single(o) dual(o) quad(o) quad-qpi(x)
////////////////////////////////////////////////////////////////////////////////

#define MX25U12832F_SIZE				(16*1024*1024)

#define MX25U12832F_IOMODE				SFLS_QUAD // SFLS_SINGLE // SFLS_DUAL // SFLS_QUAD
#define MX25U12832F_ENABLE_QPI			0 // SFLS_QUAD only
#define MX25U12832F_ENABLE_QE			1 // SFLS_QUAD only

#if (MX25U12832F_IOMODE==SFLS_SINGLE)
#define MX25U12832F_RDLTC				1
#define MX25U12832F_READ_GAP			8
#define MX25U12832F_IOR_CMD				SFLS_SINGLE
#define MX25U12832F_IOR_ADR				SFLS_SINGLE
#define MX25U12832F_IOR_DAT				SFLS_SINGLE
#define MX25U12832F_IOW_CMD				SFLS_SINGLE
#define MX25U12832F_IOW_ADR				SFLS_SINGLE
#define MX25U12832F_IOW_DAT				SFLS_SINGLE
#define MX25U12832F_IOWE_CMD			SFLS_SINGLE
#define MX25U12832F_IORS_CMD			SFLS_SINGLE
#define MX25U12832F_IORS_DAT			SFLS_SINGLE
#define MX25U12832F_CMD_WRITE			0x02
#define MX25U12832F_CMD_READ			0x0B
#elif (MX25U12832F_IOMODE==SFLS_DUAL)
#define MX25U12832F_RDLTC				1
#define MX25U12832F_READ_GAP			7
#define MX25U12832F_IOR_CMD				SFLS_SINGLE
#define MX25U12832F_IOR_ADR				SFLS_DUAL
#define MX25U12832F_IOR_DAT				SFLS_DUAL
#define MX25U12832F_IOW_CMD				SFLS_SINGLE
#define MX25U12832F_IOW_ADR				SFLS_SINGLE
#define MX25U12832F_IOW_DAT				SFLS_SINGLE
#define MX25U12832F_IOWE_CMD			SFLS_SINGLE
#define MX25U12832F_IORS_CMD			SFLS_SINGLE
#define MX25U12832F_IORS_DAT			SFLS_SINGLE
#define MX25U12832F_CMD_WRITE			0x02
#define MX25U12832F_CMD_READ			0xBB
#elif (MX25U12832F_IOMODE==SFLS_QUAD)
#if (MX25U12832F_ENABLE_QPI==0)
#define MX25U12832F_RDLTC				1
#define MX25U12832F_READ_GAP			7
#define MX25U12832F_IOR_CMD				SFLS_SINGLE
#define MX25U12832F_IOR_ADR				SFLS_QUAD
#define MX25U12832F_IOR_DAT				SFLS_QUAD
#define MX25U12832F_IOW_CMD				SFLS_SINGLE
#define MX25U12832F_IOW_ADR				SFLS_SINGLE
#define MX25U12832F_IOW_DAT				SFLS_QUAD
#define MX25U12832F_IOWE_CMD			SFLS_SINGLE
#define MX25U12832F_IORS_CMD			SFLS_SINGLE
#define MX25U12832F_IORS_DAT			SFLS_SINGLE
#define MX25U12832F_CMD_WRITE			0x32
#define MX25U12832F_CMD_READ			0xEB
#else
 #error "error: MX25U12832F_IOMODE==SFLS_QUAD, QPI==0"
#define MX25U12832F_RDLTC				1
#define MX25U12832F_READ_GAP			9
#define MX25U12832F_IOR_CMD				SFLS_QUAD
#define MX25U12832F_IOR_ADR				SFLS_QUAD
#define MX25U12832F_IOR_DAT				SFLS_QUAD
#define MX25U12832F_IOW_CMD				SFLS_QUAD
#define MX25U12832F_IOW_ADR				SFLS_QUAD
#define MX25U12832F_IOW_DAT				SFLS_QUAD
#define MX25U12832F_IOWE_CMD			SFLS_QUAD
#define MX25U12832F_IORS_CMD			SFLS_QUAD
#define MX25U12832F_IORS_DAT			SFLS_QUAD
#define MX25U12832F_CMD_WRITE			0x02
#define MX25U12832F_CMD_READ			0xEB
#endif
#endif

#define MX25U12832F_CMD_FAST_READ		MX25U12832F_CMD_READ
#define MX25U12832F_CMD_PAGE_PROGRAM	MX25U12832F_CMD_WRITE
#define MX25U12832F_CMD_WRITE_ENABLE	0x06
#define MX25U12832F_CMD_READ_STATUS		0x05
#define MX25U12832F_CMD_WRITE_STATUS	0x01
#define MX25U12832F_CMD_READ_SFDP		0x5A
#define MX25U12832F_CMD_ENTER_QPI		0x35
#define MX25U12832F_CMD_EXIT_QPI		0xF5
#define MX25U12832F_CMD_SECTOR_ERASE	0x20
#define MX25U12832F_CMD_32KB_ERASE		0x52
#define MX25U12832F_CMD_64KB_ERASE		0xD8
#define MX25U12832F_CMD_CHIP_ERASE		0x60

extern void SflsMx25u12832f_Init1(void);
extern void SflsMx25u12832f_Init2(void);
extern void SflsMx25u12832f_WriteEnable(void);
extern void SflsMx25u12832f_SecterErase(UINT addr);
extern void SflsMx25u12832f_32kErase(UINT addr);
extern void SflsMx25u12832f_64kErase(UINT addr);
extern void SflsMx25u12832f_ChipErase(UINT addr);
extern void SflsMx25u12832f_ReadSFDP(BYTE *sfdp_bin);
extern BYTE SflsMx25u12832f_ReadStatus(void);
extern void SflsMx25u12832f_WriteStatus(BYTE status);
extern void SflsMx25u12832f_EnterQPI(void);
extern void SflsMx25u12832f_ExitQPI(void);
extern UINT SflsMx25u12832f_IsQPI(void);
extern void SflsMx25u12832f_EnterQE(void);
extern void SflsMx25u12832f_ExitQE(void);
extern UINT SflsMx25u12832f_IsQE(void);
extern void SflsMx25u12832f_EnterProtection(void);
extern void SflsMx25u12832f_ExitProtection(void);
extern UINT SflsMx25u12832f_IsProtection(void);

#if (MX25U12832F_ENABLE_QPI==1)
#if (MX25U12832F_IOMODE!=SFLS_QUAD)
#error "QPI mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (MX25U12832F_ENABLE_QE==1)
#if (MX25U12832F_IOMODE!=SFLS_QUAD)
#error "QE mode failed to activate. Set the serial flash memory to QUAD mode."
#endif
#endif

#if (MX25U12832F_IOMODE==SFLS_QUAD)
#if (MX25U12832F_ENABLE_QE==0)
#error "IO mode is QUAD but failed to activate. Set the serial flash memory to QE mode."
#endif
#endif

#endif // __MACRONIX_MX25U12832F__H__
