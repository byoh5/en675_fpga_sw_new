# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_LIB_CORE3 )

# 소스 코드
SET ( CORE3_HDRS
)

SET ( CORE3_SRCS
	main3.c
)

# 라이브러리 생성
ADD_LIBRARY ( core3 STATIC ${CORE3_SRCS} ${CORE3_HDRS} )
