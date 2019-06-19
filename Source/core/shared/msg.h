#ifndef __MSG_H__
#define __MSG_H__

#include <time.h>

typedef struct {
	// Time info
	time_t TIME;
	INT TIMEZONE;
	time_t UPTIME;

	UINT VIDEO_REC;
	UINT VIDEO_FPS;
} share_msg_t;

typedef struct {
	UINT ETH_RX_CHECK;
	UINT ETH_TX_CHECK;

	UINT MAKEVID_COUNT;
} debug_msg_t;

extern share_msg_t gptMsgShare;
extern debug_msg_t gptMsgDebug;

#endif // __MSG_H__
