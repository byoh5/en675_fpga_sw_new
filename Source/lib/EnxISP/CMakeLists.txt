# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_LIB_EnxISP )

# 소스 코드
SET ( ENXISP_HDRS
	isp_lib_main.h
)

SET ( ENXISP_SRCS
	isp_lib_main.c
)

# 라이브러리 생성
ADD_LIBRARY ( enxisp STATIC ${ENXISP_SRCS} ${ENXISP_HDRS} )
