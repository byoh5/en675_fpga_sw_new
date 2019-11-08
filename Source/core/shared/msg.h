#ifndef __MSG_H__
#define __MSG_H__

#include <time.h>

//...............................................
// Shell message(?Byte)
#define MSG_SHELL_NUM		3
#define MSG_SHELL_MAXLEN	63
typedef struct{									// Shell : cpu0 -> cpu1
	UINT lock;									//		"
	UINT head;									//		"
	UINT tail;									//		"
	UINT index;									//		"
	UINT tot_num;								//		"
	char arg[MSG_SHELL_NUM][MSG_SHELL_MAXLEN+1];//		"
} shell_msg_t;									//		"

typedef struct {
	// Time info
	time_t TIME;
	INT TIMEZONE;
	time_t UPTIME;

	UINT CMP_DATEID;
	UINT VIDEO_REC;
	UINT VIDEO_FPS;
} share_msg_t;

typedef struct {
	UINT ETH_RX_CHECK;
	UINT ETH_TX_CHECK;

	UINT MAKEVID_COUNT;
	UINT WIFI_INTER_LOG;
} debug_msg_t;

typedef struct {
	UINT addr;									// address
	UINT size;									// size
	UINT type	: 8;							// 1,2(h264 1) 3,4(h264 2) 5,6(h265 1) 7,8(h265 2) 9(jpeg) 10(jpeg-sw) 11(audio) 12(txts)
	UINT ts		:24;							// time stamp
} stream_info;	// 12byte

#if defined(__NETWORK__)
#define MSG_STREAM_NUM		150
typedef struct {
	UINT tot_num;								// = MSG_STREAM_NUM

	UINT tail;									// input
	UINT head;									// output(streaming)

	// ---- 28byte
	stream_info info[MSG_STREAM_NUM];			// 12 * MSG_STREAM_NUM = 1800byte
} msgq_stream_t;
#endif

#if (LOAD_FS_SDCARD==1)
#define MSG_RECORD_NUM		(NUM_STREAM_TOTAL)
typedef struct {
	UINT tot_num;								// = MSG_STREAM_NUM

	UINT tail;									// input
	UINT head;									// output(streaming)

	// ---- 28byte
	stream_info info[MSG_RECORD_NUM];			// 12 * MSG_STREAM_NUM = 1800byte
} msgq_record_t;
#endif

extern shell_msg_t gptMsgShell;
extern share_msg_t gptMsgShare;
extern debug_msg_t gptMsgDebug;

extern void enx_msgshell_init(volatile shell_msg_t *p);

#endif // __MSG_H__
