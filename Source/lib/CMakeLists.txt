# About this project
# ------------------------------------------------------------------------------
PROJECT(EN675_LIB)

# 출력 디렉토리
SET ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE} )
SET ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib )
SET ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib )

# Add subdirectory
# ------------------------------------------------------------------------------
ADD_SUBDIRECTORY(EnxISP)
ADD_SUBDIRECTORY(FreeRTOS)
ADD_SUBDIRECTORY(FatFs)