# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_CORE_SHARED C ASM )

# 링크 라이브러리 (-l)
LINK_LIBRARIES( core0 )
LINK_LIBRARIES( core1 )
LINK_LIBRARIES( core2 )
LINK_LIBRARIES( core3 )
IF (USE_FIESYSTEM)
	LINK_LIBRARIES( fatfs )
	ADD_DEPENDENCIES ( fatfs dev )
	ADD_DEPENDENCIES ( core1 fatfs )
ENDIF (USE_FIESYSTEM)
IF (USE_NETWORK)
	LINK_LIBRARIES( lwip )
	ADD_DEPENDENCIES ( lwip dev )
	ADD_DEPENDENCIES ( core1 lwip )
ENDIF (USE_NETWORK)
#IF (USE_FREERTOS)
	LINK_LIBRARIES( freertos )
	ADD_DEPENDENCIES ( freertos dev )
	ADD_DEPENDENCIES ( core1 freertos )
#ENDIF (USE_FREERTOS)
IF (USE_WF_SD8782)
	LINK_LIBRARIES( wifisd8782 )
	ADD_DEPENDENCIES ( wifisd8782 dev )
	ADD_DEPENDENCIES ( core1 wifisd8782 )
ENDIF (USE_WF_SD8782)
LINK_LIBRARIES( dev )
LINK_LIBRARIES( enxisp )

# Target간 의존성 정의
ADD_DEPENDENCIES ( core0 dev )
ADD_DEPENDENCIES ( core1 dev )
ADD_DEPENDENCIES ( core2 dev )
ADD_DEPENDENCIES ( core3 dev )
ADD_DEPENDENCIES ( enxisp dev )
ADD_DEPENDENCIES ( core0 enxisp )

# 링크 옵션 추가
SET ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_CURRENT_SOURCE_DIR}/exbl.lds")

# 소스 코드
SET ( SHARED_HDRS
	boot.h
	exbl.h
	msg.h
	user.h
	test/test_nonos.h
	test/test_header.h
)

SET ( SHARED_SRCS
	boot.c
	exbl.c
	msg.c
	user.c
	mentry.S
	test/test_nonos.c
	test/test_dma.c
	test/test_eth.c
	test/test_sdio.c
)

# 바이너리 생성
ADD_EXECUTABLE ( ${EN675_CORE}.elf ${SHARED_SRCS} ${SHARED_HDRS} exbl.lds )
