#ifndef __MOAL__UTIL__
#define __MOAL__UTIL__

#include "mlan.h"
#include "ewl_os.h"


#ifdef DEBUG_LEVEL1
extern t_u32 drvdbg;
extern t_u32 ifdbg;

/** Debug message control bit definition for ifdbg */
#define MIF_D	MBIT(0)

#ifdef	DEBUG_LEVEL2
#define	PRINTM_MINFO(msg...)  do {if (drvdbg & MINFO)  ewl_os_printf(msg);} while(0)
#define	PRINTM_MWARN(msg...)  do {if (drvdbg & MWARN)  ewl_os_printf(msg);} while(0)
#define	PRINTM_MENTRY(msg...) do {if (drvdbg & MENTRY) ewl_os_printf(msg);} while(0)
#else
#define	PRINTM_MINFO(msg...)  do {} while (0)
#define	PRINTM_MWARN(msg...)  do {} while (0)
#define	PRINTM_MENTRY(msg...) do {} while (0)
#endif /* DEBUG_LEVEL2 */

#define	PRINTM_MFW_D(msg...)  do {if (drvdbg & MFW_D)  ewl_os_printf(msg);} while(0)
#define	PRINTM_MCMD_D(msg...) do {if (drvdbg & MCMD_D) ewl_os_printf(msg);} while(0)
#define	PRINTM_MDAT_D(msg...) do {if (drvdbg & MDAT_D) ewl_os_printf(msg);} while(0)

#define	PRINTM_MIOCTL(msg...) do {if (drvdbg & MIOCTL) ewl_os_printf(msg);} while(0)
#define	PRINTM_MINTR(msg...)  do {if (drvdbg & MINTR)  ewl_os_printf(msg);} while(0)
#define	PRINTM_MEVENT(msg...) do {if (drvdbg & MEVENT) ewl_os_printf(msg);} while(0)
#define	PRINTM_MCMND(msg...)  do {if (drvdbg & MCMND)  ewl_os_printf(msg);} while(0)
#define	PRINTM_MDATA(msg...)  do {if (drvdbg & MDATA)  ewl_os_printf(msg);} while(0)
#define	PRINTM_MERROR(msg...) do {if (drvdbg & MERROR) ewl_os_printf(msg);} while(0)
#define	PRINTM_MFATAL(msg...) do {if (drvdbg & MFATAL) ewl_os_printf(msg);} while(0)
#define	PRINTM_MMSG(msg...)   do {if (drvdbg & MMSG)   ewl_os_printf(msg);} while(0)

#define	PRINTM(level,msg...) PRINTM_##level(msg)
#else
#define	PRINTM(level,msg...) do {} while (0)
#endif /* DEBUG_LEVEL1 */


/** Wait until a condition becomes true */
#define MASSERT(cond)                   \
do {                                    \
    if (!(cond)) {                      \
        PRINTM(MFATAL, "ASSERT: %s: %i\n", __FUNCTION__, __LINE__); \
        while(1) ewl_os_printf("Assert failed: Panic!"); \
    }                                   \
} while(0)

/** Log entry point for debugging */
#define	ENTER()			//printf("Enter: %s\n",  __FUNCTION__)
/** Log exit point for debugging */
#define	LEAVE()			//printf("Leave: %s\n", __FUNCTION__)

#ifdef DEBUG_LEVEL1

#define DBG_HEXDUMP_MCMD_D(x,y,z)    do {if (drvdbg & MCMD_D) ewl_os_hexdump(x,y,z);} while(0)
#define DBG_HEXDUMP_MDAT_D(x,y,z)    do {if (drvdbg & MDAT_D) ewl_os_hexdump(x,y,z);} while(0)
#define DBG_HEXDUMP_MIF_D(x,y,z)     do {if (ifdbg & MIF_D)   ewl_os_hexdump(x,y,z);} while(0)
#define DBG_HEXDUMP_MFW_D(x,y,z)     do {if (drvdbg & MFW_D)  ewl_os_hexdump(x,y,z);} while(0)
#define	DBG_HEXDUMP(level,x,y,z)    DBG_HEXDUMP_##level(x,y,z)

#else
/** Do nothing since debugging is not turned on */
#define DBG_HEXDUMP(level,x,y,z)    do {} while (0)
#endif

#ifdef DEBUG_LEVEL2
#define HEXDUMP(x,y,z)              do {if (drvdbg & MINFO) ewl_os_hexdump(x,y,z);} while(0)
#else
/** Do nothing since debugging is not turned on */
#define HEXDUMP(x,y,z)              do {} while (0)
#endif

#endif

