# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_CORE_SHARED C ASM )

# 링크 라이브러리 (-l)
LINK_LIBRARIES( freertos )
LINK_LIBRARIES( core0 )
LINK_LIBRARIES( core1 )
LINK_LIBRARIES( core2 )
LINK_LIBRARIES( core3 )

# 링크 옵션 추가
SET ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_CURRENT_SOURCE_DIR}/exbl.lds")

# 소스 코드
SET ( SHARED_HDRS
	boot.h
	clib.h
	configstring.h
	exbl.h
	syscalls.h
	${CMAKE_SOURCE_DIR}/Source/dev/dev.h
	${CMAKE_SOURCE_DIR}/Source/dev/dev_types.h
	${CMAKE_SOURCE_DIR}/Source/dev/dev_regs.h
	${CMAKE_SOURCE_DIR}/Source/dev/dev_peri.h
	${CMAKE_SOURCE_DIR}/Source/dev/cpu/asm.h
	${CMAKE_SOURCE_DIR}/Source/dev/cpu/bits.h
	${CMAKE_SOURCE_DIR}/Source/dev/cpu/encoding.h
	${CMAKE_SOURCE_DIR}/Source/dev/cpu/mtrap.h
	${CMAKE_SOURCE_DIR}/Source/dev/string/vsnprintf.h
)

SET ( SHARED_SRCS
	clib.c
	syscalls.c
	boot.c
	exbl.c
	mentry.S
	${CMAKE_SOURCE_DIR}/Source/dev/aes.c
	${CMAKE_SOURCE_DIR}/Source/dev/checksum.c
	${CMAKE_SOURCE_DIR}/Source/dev/ddr.c
	${CMAKE_SOURCE_DIR}/Source/dev/dma.c
	${CMAKE_SOURCE_DIR}/Source/dev/eth.c
	${CMAKE_SOURCE_DIR}/Source/dev/gpio.c
	${CMAKE_SOURCE_DIR}/Source/dev/i2c.c
	${CMAKE_SOURCE_DIR}/Source/dev/i2s.c
	${CMAKE_SOURCE_DIR}/Source/dev/sdio.c
	${CMAKE_SOURCE_DIR}/Source/dev/sfls.c
	${CMAKE_SOURCE_DIR}/Source/dev/sha.c
	${CMAKE_SOURCE_DIR}/Source/dev/spi.c
	${CMAKE_SOURCE_DIR}/Source/dev/timer.c
	${CMAKE_SOURCE_DIR}/Source/dev/uart.c
	${CMAKE_SOURCE_DIR}/Source/dev/wdt.c
	${CMAKE_SOURCE_DIR}/Source/dev/string/vsnprintf.c
)

# 바이너리 생성
ADD_EXECUTABLE ( ${EN675_CORE} ${SHARED_SRCS} ${SHARED_HDRS} exbl.lds )