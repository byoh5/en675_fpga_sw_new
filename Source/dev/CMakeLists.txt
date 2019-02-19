# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT(EN675_DEV)

# Configure File
CONFIGURE_FILE ( dev_peri.h.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/dev_peri.h)
CONFIGURE_FILE ( dev_device.h.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/dev_device.h)

# 소스 코드
SET ( DEV_HDRS
	dev.h
	dev_types.h
	dev_regs.h
	dev_peri.h
	cpu/asm.h
	cpu/bits.h
	cpu/encoding.h
	cpu/mtrap.h
	string/vsnprintf.h
	rtc/rtc.h
	rtc/pcf8563.h
	rtc/s35390a.h
)

SET ( DEV_SRCS
	aes.c
	checksum.c
	ddr.c
	dma.c
	eth.c
	gpio.c
	i2c.c
	i2s.c
	irq.c
	sdio.c
	sfls.c
	sha.c
	spi.c
	time.c
	timer.c
	uart.c
	wdt.c
	string/vsnprintf.c
	rtc/rtc.c
	rtc/rtc-lib.c
	rtc/pcf8563.c
	rtc/s35390a.c
)

# 라이브러리 생성
ADD_LIBRARY ( dev STATIC ${DEV_SRCS} ${DEV_HDRS} )