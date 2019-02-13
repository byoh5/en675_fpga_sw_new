# About this project
# ------------------------------------------------------------------------------
PROJECT(EN675_CORE)

# 출력 디렉토리
SET ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE} )
SET ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib )
SET ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/lib )

# Add subdirectory
# ------------------------------------------------------------------------------
ADD_SUBDIRECTORY(core0)
ADD_SUBDIRECTORY(core1)
ADD_SUBDIRECTORY(core2)
ADD_SUBDIRECTORY(core3)
ADD_SUBDIRECTORY(shared)
