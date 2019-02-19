﻿# 헤더 디렉토리 추가 (-I) ----------------------------------------------------------
#INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/boot)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/dev)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/dev/cpu)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/dev/rtc)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/dev/string)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lib)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lib/FreeRTOS)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lib/FreeRTOS/Source)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lib/FreeRTOS/Source/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lib/FreeRTOS/Source/portable/MemMang)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/lib/FreeRTOS/Source/portable/GCC/RISCV)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/core/core1/task/shell)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Source/core/shared)

