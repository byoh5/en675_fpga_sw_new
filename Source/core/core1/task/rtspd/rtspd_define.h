#ifndef _RTSP_DEFINE_H_
#define _RTSP_DEFINE_H_

// Server Agent
#define RTSPD_SERVER_AGENT					DEVICE_SYSTEM_REALM

// Method
#define ENX_RTSP_METHOD_strOPTIONS			"OPTIONS"
#define ENX_RTSP_METHOD_strDESCRIBE			"DESCRIBE"
#define ENX_RTSP_METHOD_strSETUP			"SETUP"
#define ENX_RTSP_METHOD_strPLAY				"PLAY"
#define ENX_RTSP_METHOD_strPAUSE			"PAUSE"
#define ENX_RTSP_METHOD_strTEARDOWN			"TEARDOWN"
#define ENX_RTSP_METHOD_strGET_PARAMETER	"GET_PARAMETER"
#define ENX_RTSP_METHOD_steSET_PARAMETER	"SET_PARAMETER"

typedef enum {
	ENX_RTSP_METHOD_OPTIONS,				// 10.1 OPTIONS
	ENX_RTSP_METHOD_DESCRIBE,				// 10.2 DESCRIBE
	ENX_RTSP_METHOD_SETUP,					// 10.4 SETUP
	ENX_RTSP_METHOD_PLAY,					// 10.5 PLAY
	ENX_RTSP_METHOD_PAUSE,					// 10.6 PAUSE
	ENX_RTSP_METHOD_TEARDOWN,				// 10.7 TEARDOWN
	ENX_RTSP_METHOD_GET_PARAMETER,			// 10.8 GET_PARAMETER
	ENX_RTSP_METHOD_SET_PARAMETER			// 10.9 SET_PARAMETER
} rtsp_method_type;

// State
typedef enum {
	ENX_RTSP_STATE_NONE,					// 0 idle, connect
	ENX_RTSP_STATE_OPTIONS,					// 1 options
	ENX_RTSP_STATE_DESCRIBE,				// 2 deseribe
	ENX_RTSP_STATE_SETUP,					// 3 setup
	ENX_RTSP_STATE_PLAY,					// 4 play
	ENX_RTSP_STATE_PAUSE,					// 5 pause
	ENX_RTSP_STATE_TEARDOWN,				// 6 teardown
	ENX_RTSP_STATE_GET_PARAMETER,			// 7 get_parameter
	ENX_RTSP_STATE_SET_PARAMETER,			// 8 set_parameter
	ENX_RTSP_STATE_ERROR					// 9 error
} rtsp_client_state;

// Stream type
typedef enum {
	ENX_RTSP_STREAM_FILE_H264_TYPE,
	ENX_RTSP_STREAM_FILE_H265_TYPE,
	ENX_RTSP_STREAM_LIVE,
	ENX_RTSP_STREAM_LIVE_H264_1,
	ENX_RTSP_STREAM_LIVE_H264_2,
	ENX_RTSP_STREAM_LIVE_H265_1,
	ENX_RTSP_STREAM_LIVE_H265_2,
	ENX_RTSP_STREAM_LIVE_JPEG,
	ENX_RTSP_STREAM_LIVE_JPEG_SW,
	ENX_RTSP_STREAM_NONE,
	ENX_RTSP_STREAM_ERROR,
} rtsp_stream_type;

// RTSP message
#define ENX_RTSP_HEADER_strCSEQ				"CSeq"
#define ENX_RTSP_HEADER_strSESSION			"Session"
#define ENX_RTSP_HEADER_strACCEPT			"Accept"
#define ENX_RTSP_HEADER_strTRANSPORT		"Transport"
#define ENX_RTSP_HEADER_strAUTHORIZATION	"Authorization"

#define ENX_RTSP_RESPONSE_HEAD		"RTSP/1.0 %s\r\nCSeq: %u\r\n"
#define ENX_RTSP_RESPONSE_HEAD_S	"RTSP/1.0 %s\r\nCSeq: %u\r\nSession: %u\r\n"

#define ENX_RTSP_RESPONSE_100	"100 Continue"
#define ENX_RTSP_RESPONSE_200	"200 OK"
#define ENX_RTSP_RESPONSE_201	"201 Created"
#define ENX_RTSP_RESPONSE_250	"250 Low on Storage Space"
#define ENX_RTSP_RESPONSE_300	"300 Multiple Choices"
#define ENX_RTSP_RESPONSE_301	"301 Moved Permanently"
#define ENX_RTSP_RESPONSE_302	"302 Moved Temporarily"
#define ENX_RTSP_RESPONSE_303	"303 See Other"
#define ENX_RTSP_RESPONSE_304	"304 Not Modified"
#define ENX_RTSP_RESPONSE_305	"305 Use Proxy"
#define ENX_RTSP_RESPONSE_400	"400 Bad Request"
#define ENX_RTSP_RESPONSE_401	"401 Unauthorized"
#define ENX_RTSP_RESPONSE_402	"402 Payment Required"
#define ENX_RTSP_RESPONSE_403	"403 Forbidden"
#define ENX_RTSP_RESPONSE_404	"404 Not Found"
#define ENX_RTSP_RESPONSE_405	"405 Method Not Allowed"
#define ENX_RTSP_RESPONSE_406	"406 Not Acceptable"
#define ENX_RTSP_RESPONSE_407	"407 Proxy Authentication Required"
#define ENX_RTSP_RESPONSE_408	"408 Request Time-out"
#define ENX_RTSP_RESPONSE_410	"410 Gone"
#define ENX_RTSP_RESPONSE_411	"411 Length Required"
#define ENX_RTSP_RESPONSE_412	"412 Precondition Failed"
#define ENX_RTSP_RESPONSE_413	"413 Request Entity Too Large"
#define ENX_RTSP_RESPONSE_414	"414 Request-URI Too Large"
#define ENX_RTSP_RESPONSE_415	"415 Unsupported Media Type"
#define ENX_RTSP_RESPONSE_451	"451 Parameter Not Understood"
#define ENX_RTSP_RESPONSE_452	"452 Conference Not Found"
#define ENX_RTSP_RESPONSE_453	"453 Not Enough Bandwidth"
#define ENX_RTSP_RESPONSE_454	"454 Session Not Found"
#define ENX_RTSP_RESPONSE_455	"455 Method Not Valid in This State"
#define ENX_RTSP_RESPONSE_456	"456 Header Field Not Valid for Resource"
#define ENX_RTSP_RESPONSE_457	"457 Invalid Range"
#define ENX_RTSP_RESPONSE_458	"458 Parameter Is Read-Only"
#define ENX_RTSP_RESPONSE_459	"459 Aggregate operation not allowed"
#define ENX_RTSP_RESPONSE_460	"460 Only aggregate operation allowed"
#define ENX_RTSP_RESPONSE_461	"461 Unsupported transport"
#define ENX_RTSP_RESPONSE_462	"462 Destination unreachable"
#define ENX_RTSP_RESPONSE_500	"500 Internal Server Error"
#define ENX_RTSP_RESPONSE_501	"501 Not Implemented"
#define ENX_RTSP_RESPONSE_502	"502 Bad Gateway"
#define ENX_RTSP_RESPONSE_503	"503 Service Unavailable"
#define ENX_RTSP_RESPONSE_504	"504 Gateway Time-out"
#define ENX_RTSP_RESPONSE_505	"505 RTSP Version not supported"
#define ENX_RTSP_RESPONSE_551	"551 Option not supported"

// Options
#define ENX_RTSP_RESPONSE_OPTIONS	"Public: OPTIONS, DESCRIBE, PLAY, PAUSE, SETUP, TEARDOWN, SET_PARAMETER, GET_PARAMETER\r\n"
// Describe
#define ENX_RTSP_RESPONSE_DESCRIBE	"Content-Type: application/sdp\r\nContent-Length: %u\r\n"
// Date
#define ENX_RTSP_RESPONSE_DATE		"Date: %s, %s %02u %04u %02u:%02u:%02u GMT\r\n"
// Authenticate-Basic
#define ENX_RTSP_RESPONSE_BAUTH		"WWW-Authenticate: Basic realm=\""RTSPD_SERVER_AGENT"\"\r\n"
// Authenticate-Digest
//#define ENX_RTSP_RESPONSE_DAUTH		"WWW-Authenticate: Digest realm=\""RTSPD_SERVER_AGENT"\", qop=\"auth, auth-int\", algorithm=MD5, nonce=\"%s\", stale=\"%s\"\r\n"
#define ENX_RTSP_RESPONSE_DAUTH		"WWW-Authenticate: Digest realm=\""RTSPD_SERVER_AGENT"\", qop=\"auth, auth-int\", algorithm=MD5, nonce=\"%s\", opaque=\"%s\"\r\n"
//#define ENX_RTSP_RESPONSE_DAUTH		"WWW-Authenticate: Digest realm=\""RTSPD_SERVER_AGENT"\", qop=\"auth, auth-int\", algorithm=SHA-256, nonce=\"%s\", stale=\"%s\"\r\n"
//#define ENX_RTSP_RESPONSE_DAUTH		"WWW-Authenticate: Digest realm=\""RTSPD_SERVER_AGENT"\", qop=\"auth, auth-int\", algorithm=SHA-256, nonce=\"%s\", opaque=\"%s\"\r\n"
// RTP-Info
#define ENX_RTSP_RESPONSE_RTPINFO_H "RTP-Info:"
#define ENX_RTSP_RESPONSE_RTPINFO_B	" url=%s/%s;seq=%u;rtptime=%u,"
// Setup
#define ENX_RTSP_RESPONSE_SETUP_UDP	"Transport: RTP/AVP;unicast;client_port=%d-%d;server_port=%u-%u;ssrc=%8x;mode=\"PLAY\"\r\n"
#define ENX_RTSP_RESPONSE_SETUP_TCP	"Transport: RTP/AVP/TCP;unicast;interleaved=%u-%u;ssrc=%8x;mode=\"PLAY\"\r\n"

// SDP URL
#define ENX_RTSP_SDP_URL_VIDEO "video"
#define ENX_RTSP_SDP_URL_AUDIO "audio"
//#define ENX_RTSP_SDP_URL_METADATA "meta"
//#define ENX_RTSP_SDP_URL_BACKCHANNEL "back"

#define RTSPD_AUTHRIZATION_BASIC "Authorization: Basic "
#define RTSPD_AUTHRIZATION_DIGEST "Authorization: Digest "

typedef enum {
	ENX_RTSP_TRANSPORT_NONE,
	ENX_RTSP_TRANSPORT_UDP,
	ENX_RTSP_TRANSPORT_TCP,
	ENX_RTSP_TRANSPORT_HTTP,
} rtsp_transport_type;

enum {
	ENX_RTSP_STRTYPE_NONE = 0,
	ENX_RTSP_STRTYPE_VIDEO = 1,
	ENX_RTSP_STRTYPE_AUDIO = 2,
//	ENX_RTSP_STRTYPE_METADATA = 4,
//	ENX_RTSP_STRTYPE_BACKCHANNEL = 8,
};

enum {
	ENX_RTSP_STRTYPE_numVIDEO,
	ENX_RTSP_STRTYPE_numAUDIO,
//	ENX_RTSP_STRTYPE_numMETADATA,
//	ENX_RTSP_STRTYPE_numBACKCHANNEL,
	ENX_RTSP_STRTYPE_numNONE,
};

typedef enum {
	ENX_RTP_TXSTE_READY = 1,
	ENX_RTP_TXSTE_DOING = 2,
	ENX_RTP_TXSTE_END = 3,
} rtp_txstate_type;

// 4 byte
typedef struct {
	uint8 un8Magic;
	uint8 un8Channel;
	uint16 un16Length;
}__attribute__ ((packed)) rthInterleaved;

// 12 byte
typedef struct {
	BF_4(
		uint8 un2RTPVersion:2,
		uint8 un1Padding:1,
		uint8 un1Extension:1,
		uint8 un4CSRCcount:4
	)
	BF_2(
		uint8 un1Markerbit:1,
		uint8 un7Payloadtype:7
	)
	uint16 un16Sequencenum;
	uint32 un32Timestamp;
	uint32 un32SSIdentifier;
}__attribute__ ((packed)) rthRTPHeader;

typedef struct {
	UINT base;
	UINT size;
	UINT offset;

	UINT width;	// JPEG
	UINT height;// JPEG
	UINT data_offset;// JPEG

	UINT nalfirst; // H.264
	UINT naltype; // H.264/H.265
	UINT nalidc; // H.264
	UINT nalnum; // H.264

	WORD nalunithdr; // H.265
} rtp_packet;

typedef struct {
//	WORD rtp_port;
//	WORD rtcp_port;

	rtp_txstate_type tx_ready;
	rtp_packet rtp_pk;

	UINT payload_type;

	UINT ssrc;
	UINT start_rtptime;
	WORD start_seq;

// RTCP 
	UINT sr_send_time;
	UINT packet_cnt;
	UINT total_length;

// RTP Buffer
	BYTE buffer[2][1500];
	UINT buf_len[2];
	UINT buf_idx;
} rtp_session;

typedef struct {
	UINT server_addr;
	WORD server_port;
	WORD client_port;
	UINT client_addr;
	WORD video_type;	// H.264(1,2), MJPEG
	WORD rtp_type;		// UDP, TCP
	UINT connect_time;	// connect time, unused=>0x0
	WORD rtp_port[ENX_RTSP_STRTYPE_numNONE];
	WORD rtcp_port[ENX_RTSP_STRTYPE_numNONE];
} rtsp_connect_info;

typedef struct {
	int index;
	int clientid;
	int valid;
	int sockfd;
	int login;
	rtsp_client_state state;
	int client_read_timeout_sec;
	int client_write_timeout_sec;

// Timeout
	UINT lastTime;
	UINT erroldTime;
	UINT errCount;

// Address info
	rtsp_connect_info conn_info;

// RTP queue
	rtp_queue *rtp_queue_video;
#ifdef __AUDIO__
	rtp_queue *rtp_queue_audio;
#endif

// RTSP method processing...
	INT nVEncoderIdx;
	rtsp_stream_type isLive; // live stream인지 sdcard file stream인지 판단
	int nSession; // rand값. session이 설정된 이후에는 이 항목이 들어가야 한다.
	UINT play_query;
	rtsp_transport_type eTransport; // 전송방식(TCP, UDP, HTTP)
	rtp_session *rtp_ss;

// Variables used as one-time
	int nCSeq; // 공통, client가 주는 값을 응답값에 넣는다. 필수항목
	rtsp_method_type method; // 공통, 현재의 RTSP method
	char strUrl[128]; // 공통, 현재의 요청 URL
	char strVersion[12]; // 공통, 현재의 RTSP Version(?)
	ENX_YN isAccept; // Describe명령, client가 주는 Appcpt header가 존재하며, 그 값이 application/sdp 일 경우 DEF_YES.
	ENX_YN isTransport; // Setup명령, client가 주는 Transport header가 존재하며,
	UINT setup_query; // Setup명령, 
	UINT nQuery; // Setup명령, 
} rtsp_client;

typedef struct {
	int sockfd;

	int client_accept_timeout_sec;
	int client_count;
	int client_last_id;
	rtsp_client rclist[RTSP_info_MAX];
} rtsp_server;

#define RTSP_chrNULL	'\0'
#define RTSP_chrSPACE	' '
#define RTSP_strCRLF	"\r\n"

#define RTP_numRTPVer	2
#define RTP_numPayload_PCMU	0
#define RTP_numPayload_PCMA	8
#define RTP_numPayload_L16S 11
#define RTP_numPayload_JPEG	26
#define RTP_numPayload_H264	96 // == H.265
#define RTP_numPayload_H265	96 // == H.264

#define RTSP_TIMEOUT 60
#define RTCP_SR_INTERVAL 10

#endif
