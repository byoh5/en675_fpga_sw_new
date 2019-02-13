# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_LIB_CORE2 )

# 소스 코드
SET ( CORE2_HDRS
)

SET ( CORE2_SRCS
	main2.c
)

# 라이브러리 생성
ADD_LIBRARY ( core2 STATIC ${CORE2_SRCS} ${CORE2_HDRS} )
