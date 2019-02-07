# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_LIB_CORE0 )

# 소스 코드
SET ( CORE0_HDRS
)

SET ( CORE0_SRCS
	main0.c
)

# 라이브러리 생성
ADD_LIBRARY ( core0 STATIC ${CORE0_SRCS} ${CORE0_HDRS} )
