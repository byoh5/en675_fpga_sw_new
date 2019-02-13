# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_LIB_FatFS )

SET ( FATFS_HDRS
	source/diskio.h
	source/ff.h
	source/ffconf.h
)

SET ( FATFS_SRCS
	source/diskio.c
	source/ff.c
	source/ffsystem.c
	source/ffunicode.c
)

# 라이브러리 생성
ADD_LIBRARY ( fatfs STATIC ${FATFS_SRCS} ${FATFS_HDRS} )
