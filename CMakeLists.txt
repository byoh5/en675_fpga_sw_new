CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )                                       # 최소한의 Cmake 버전을 설정

# About this project
# ------------------------------------------------------------------------------
PROJECT(EN675_FPGA)
SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "1")
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Binary file name
SET ( EN675_BOOT bootloader.bin )
SET ( EN675_CORE core.bin )
SET ( EN675_OUTPUT EN675.bin )

SET ( CMAKE_VERBOSE_MAKEFILE false )                                            # 빌드 상세 과정을 모두 출력하는 Makefile을 생성함

SET ( ROOT_PATH "$(CMAKE_CURRENT_SOURCE_DIR)" )

INCLUDE ( ${PROJECT_BINARY_DIR}/BuildMode.cmake )
INCLUDE ( Cmake/cpack.cmake )
INCLUDE ( Cmake/includeheaddir.cmake )
INCLUDE ( Cmake/peripheral.cmake )
INCLUDE ( Cmake/device.cmake )

IF ( NOT CMAKE_BUILD_TYPE )
	SET ( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
ENDIF ( NOT CMAKE_BUILD_TYPE )

# 컴파일 옵션 추가
#ADD_COMPILE_OPTIONS ( "-g -Wall" )
SET ( CMAKE_EXE_LINKER_FLAGS "-static -Wl,--gc-sections" )

# 전처리기 매크로 추가 (-D)
ADD_DEFINITIONS( -DMY_DEBUG=1 )

# 라이브러리 디렉토리 지정 (-L)
LINK_DIRECTORIES ( ${CMAKE_BUILD_TYPE}/lib )

# "Install" ...? 안먹히는듯
#SET ( CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" )

# 출력 디렉토리
SET ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE} )
SET ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib )
SET ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib )

# Add subdirectory -------------------------------------------------------------
ADD_SUBDIRECTORY( Source/dev )
ADD_SUBDIRECTORY( Source/lib )
#ADD_SUBDIRECTORY( Source/boot )
ADD_SUBDIRECTORY( Source/core )

#ADD_CUSTOM_COMMAND ( test COMMAND make all -DTEST=y )

#ADD_CUSTOM_COMMAND (
#                TARGET ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${EN675_CORE}
#                POST_BUILD
#                COMMENT "Generating binary from executable"
#                COMMAND ren ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${EN675_CORE} ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${EN675_CORE}
#)

#ADD_CUSTOM_COMMAND (
#                OUTPUT ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${EN675_OUTPUT}
#                COMMENT "Generating binary from executable"
#                DEPENDS ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${EN675_CORE}
#                COMMAND ren ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${EN675_CORE} ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${EN675_OUTPUT}
#)

#ADD_CUSTOM_COMMAND (
#                OUTPUT ${CMAKE_BUILD_TYPE}/${EN675_OUTPUT}.dis
#                COMMENT "Generating binary hex dump"
#                DEPENDS ${CMAKE_BUILD_TYPE}/${EN675_CORE}
#                COMMAND ${CMAKE_OBJDUMP} -h 
#)

ADD_CUSTOM_TARGET ( objdump_head
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_OBJDUMP} -h ${EN675_CORE}
		DEPENDS ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${EN675_CORE}
        )

ADD_CUSTOM_TARGET ( objdump_dis
		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${CMAKE_OBJDUMP} -h -d -S ${EN675_CORE} > ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/core.dump
		DEPENDS ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${EN675_CORE}
        )

#ADD_CUSTOM_TARGET ( target_name
#		WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#		COMMAND copy /Y /B ${EN675_BOOT} + ${EN675_CORE} ${EN675_OUTPUT}
#		COMMAND move /Y ${EN675_CORE} ${EN675_OUTPUT}
#		DEPENDS ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${EN675_CORE}
#       )

# Eclipse 관련 설정 ------------------------------------------------------------

# 컴파일에 사용할 CPU core 설정
SET (CMAKE_ECLIPSE_MAKE_ARGUMENTS "-j4")