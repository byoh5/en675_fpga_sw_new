#ifndef _SNTPC_H_
#define _SNTPC_H_

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
typedef enum {
	eSNTP_INIT    = 0,
	eSNTP_PROCESS = 1,
	eSNTP_TIMESET = 2,
	eSNTP_DELAY   = 3,
	eSNTP_FAIL    = 4,
} eSNTP_STATE;

typedef struct {
	eSNTP_STATE state;
	UINT is_timeset;
	UINT retry_cnt;
	time_t last_gettime;
} sntp_state;

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern void sntp_settime(UINT t);
extern void sntpc_start(void);
extern void sntpc_stop(void);

//-------------------------------------------------------------------------------------------------
// Variable
extern sntp_state sntp_info;	// sntp state, info, etc

#endif // _SNTPC_H_
