# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_LIB_CORE1 )

# 소스 코드
SET ( CORE1_HDRS
	enx_file.h
	task/ethloopback/ethloopback.h
	task/shell/shell.h
	task/shell/shell_cmd.h
	task/shell/shell_cmd_common.h
	task/shell/shell_cmd_fs.h
	task/shell/shell_cmd_sdio.h
	task/shell/shell_cmd_sfls.h
	task/shell/shell_cmd_mem.h
	task/shell/shell_cmd_net.h
	task/shell/shell_cmd_eth.h
	task/ledblink/ledblink.h
	task/sdcard/sdcard.h
	task/lwiperf/iperf.h
	task/ping/ping.h
	task/sntpc/sntpc.h
	task/http/httpd_structs.h
	task/videnc/fmt_avienc.h
	task/videnc/fmt_mp4enc.h
	task/videnc/muxer_videnc.h
	task/videnc/stream_buffering.h
	task/videnc/videnc_heap.h
	task/videnc/videnc_info.h
)

SET ( CORE1_SRCS
	main1.c
	enx_file.c
	task/ethloopback/ethloopback.c
	task/shell/shell.c
	task/shell/shell_cmd.c
	task/shell/shell_cmd_common.c
	task/shell/shell_cmd_fs.c
	task/shell/shell_cmd_sdio.c
	task/shell/shell_cmd_sfls.c
	task/shell/shell_cmd_mem.c
	task/shell/shell_cmd_net.c
	task/shell/shell_cmd_eth.c
	task/ledblink/ledblink.c
	task/sdcard/sdcard.c
	task/lwiperf/iperf.c
	task/lwiperf/lwiperf.c
	task/ping/ping.c
	task/sntpc/sntp.c
	task/sntpc/sntpc.c
	task/http/altcp_proxyconnect.c
	task/http/fs.c
	task/http/fsdata.c
	task/http/http_client.c
	task/http/httpd.c
#	task/videnc/fmt_avienc.c
#	task/videnc/fmt_mp4enc.c
#	task/videnc/muxer_videnc.c
#	task/videnc/stream_buffering.c
#	task/videnc/videnc_heap.c
#	task/videnc/videnc_info.c
)

# 라이브러리 생성
ADD_LIBRARY ( core1 STATIC ${CORE1_SRCS} ${CORE1_HDRS} )
