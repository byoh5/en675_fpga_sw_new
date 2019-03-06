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
LINK_LIBRARIES( dev )
LINK_LIBRARIES( fatfs )

# 링크 옵션 추가
SET ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_CURRENT_SOURCE_DIR}/exbl.lds")

# 소스 코드
SET ( SHARED_HDRS
	boot.h
	exbl.h
	msg.h
)

SET ( SHARED_SRCS
	boot.c
	exbl.c
	msg.c
	mentry.S
)

# 바이너리 생성
ADD_EXECUTABLE ( ${EN675_CORE} ${SHARED_SRCS} ${SHARED_HDRS} exbl.lds )