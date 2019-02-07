# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_Bootloader C ASM )

# 링크 라이브러리 (-l)
LINK_LIBRARIES( freertos )

# 링크 옵션 추가
SET ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_CURRENT_SOURCE_DIR}/exbl.lds")

# 소스 코드
SET ( BOOTLOADER_SRCS
	uart.c
	print.c
	boot.c
	exbl.c
	mentry.S
)

# 실행파일 생성
#ADD_DEPENDENCIES ( ${EN675_BOOT} freertos )
ADD_EXECUTABLE ( ${EN675_BOOT} ${BOOTLOADER_SRCS} )
target_link_libraries ( ${EN675_BOOT} freertos )