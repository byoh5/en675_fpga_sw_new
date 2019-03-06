# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_LIB_CORE1 )

# 소스 코드
SET ( CORE1_HDRS
	enx_file.h
	task/shell/shell.h
	task/shell/shell_cmd.h
	task/shell/shell_cmd_common.h
	task/shell/shell_cmd_fs.h
	task/shell/shell_cmd_sdio.h
	task/ledblink/ledblink.h
	task/sdcard/sdcard.h
)

SET ( CORE1_SRCS
	main1.c
	enx_file.c
	task/shell/shell.c
	task/shell/shell_cmd.c
	task/shell/shell_cmd_common.c
	task/shell/shell_cmd_fs.c
	task/shell/shell_cmd_sdio.c
	task/ledblink/ledblink.c
	task/sdcard/sdcard.c
)

# 라이브러리 생성
ADD_LIBRARY ( core1 STATIC ${CORE1_SRCS} ${CORE1_HDRS} )
