# About this project
# ------------------------------------------------------------------------------
PROJECT(EN675_DEV)

# 출력 디렉토리
#SET ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE} )
#SET ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib )
#SET ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib )

# Configure File ---------------------------------------------------------------
CONFIGURE_FILE ( dev_peri.h.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/dev_peri.h)

# Add subdirectory
# ------------------------------------------------------------------------------
