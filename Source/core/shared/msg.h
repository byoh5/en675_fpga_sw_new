#ifndef __MSG_H__
#define __MSG_H__

#include <time.h>

typedef struct {
	// Time info
	time_t TIME;
	INT TIMEZONE;
	time_t UPTIME;
} share_msg_t;

extern share_msg_t gptMsgShare;

#endif // __MSG_H__
