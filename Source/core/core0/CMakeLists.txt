# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_LIB_CORE0 )

# 소스 코드
SET ( CORE0_HDRS
	enx_rtos.h
	enx_stream.h
	enx_record.h
	enx_file.h
	task/audtxrx/audtxrx.h
	task/ir/ir_recv.h
	task/change/ipchange.h
	task/change/videochange.h
	task/chargen/chargen.h
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
	task/shell/shell_cmd_media.h
	task/shell/shell_cmd_gpio.h
	task/shell/shell_cmd_uart.h
	task/shell/shell_cmd_spi.h
	task/shell/shell_cmd_i2c.h
	task/shell/shell_cmd_util.h
	task/ledblink/ledblink.h
	task/sdcard/sdcard.h
	task/lwiperf/iperf.h
	task/ping/ping.h
	task/rtspd/rtcp_process.h
	task/rtspd/rtp_g711.h
	task/rtspd/rtp_h264.h
	task/rtspd/rtp_h265.h
	task/rtspd/rtp_jpeg.h
	task/rtspd/rtp_jpeg_sw.h
	task/rtspd/rtp_queue.h
	task/rtspd/rtspd.h
	task/rtspd/rtspd_authorization.h
	task/rtspd/rtspd_define.h
	task/rtspd/rtspd_process.h
	task/sntpc/sntpc.h
	task/http/httpd_structs.h
	task/http/cgissi.h
	task/http/cgissi_en675.h
	task/http/fsaplist.h
	task/http/fsfilelist.h
	task/http/httpd_authorization.h
	task/videnc/fmt_avienc.h
	task/videnc/fmt_mp4enc.h
	task/videnc/muxer_videnc.h
	task/videnc/stream_buffering.h
	task/videnc/videnc_heap.h
	task/videnc/videnc_info.h
	task/wifi/wifi.h
	task/wifi/wifi_task.h
)

SET ( CORE0_SRCS
	main0.c
	enx_rtos.c
	enx_stream.c
	enx_record.c
	enx_file.c
	task/audtxrx/audtxrx.c
	task/ir/ir_recv.c
	task/change/ipchange.c
	task/change/videochange.c
	task/chargen/chargen.c
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
	task/shell/shell_cmd_media.c
	task/shell/shell_cmd_gpio.c
	task/shell/shell_cmd_uart.c
	task/shell/shell_cmd_spi.c
	task/shell/shell_cmd_i2c.c
	task/shell/shell_cmd_util.c
	task/ledblink/ledblink.c
	task/sdcard/sdcard.c
	task/lwiperf/iperf.c
	task/lwiperf/lwiperf.c
	task/ping/ping.c
	task/sntpc/sntp.c
	task/sntpc/sntpc.c
	task/rtspd/rtcp_process.c
	task/rtspd/rtp_g711.c
	task/rtspd/rtp_h264.c
	task/rtspd/rtp_h265.c
	task/rtspd/rtp_jpeg.c
	task/rtspd/rtp_jpeg_sw.c
	task/rtspd/rtp_queue.c
	task/rtspd/rtspd.c
	task/rtspd/rtspd_authorization.c
	task/rtspd/rtspd_process.c
	task/http/altcp_proxyconnect.c
	task/http/fs.c
	task/http/http_client.c
	task/http/httpd.c
	task/http/cgissi.c
	task/http/cgissi_en675.c
	task/http/fsaplist.c
	task/http/fsfilelist.c
	task/http/httpd_authorization.c
	task/videnc/fmt_avienc.c
	task/videnc/fmt_mp4enc.c
	task/videnc/muxer_videnc.c
	task/videnc/stream_buffering.c
	task/videnc/videnc_heap.c
	task/videnc/videnc_info.c
	task/wifi/wifi.c
	task/wifi/wifi_task.c
)

# 라이브러리 생성
ADD_LIBRARY ( core0 STATIC ${CORE0_SRCS} ${CORE0_HDRS} )
