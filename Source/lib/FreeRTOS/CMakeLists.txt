# 최소한의 Cmake 버전을 설정
CMAKE_MINIMUM_REQUIRED ( VERSION 3.13.2 )

# 프로젝트 이름 설정
PROJECT ( EN675_LIB_FreeRTOS C ASM )

# 소스 코드
SET ( FREERTOS_HDRS
	enx_freertos.h
	FreeRTOSConfig.h
	Source/include/croutine.h
	Source/include/deprecated_definitions.h
	Source/include/event_groups.h
	Source/include/FreeRTOS.h
	Source/include/list.h
	Source/include/message_buffer.h
	Source/include/mpu_prototypes.h
	Source/include/mpu_wrappers.h
	Source/include/portable.h
	Source/include/projdefs.h
	Source/include/queue.h
	Source/include/semphr.h
	Source/include/stack_macros.h
	Source/include/StackMacros.h
	Source/include/stream_buffer.h
	Source/include/task.h
	Source/include/timers.h
	Source/portable/GCC/RISC-V/portmacro.h
)

SET ( FREERTOS_SRCS
	Source/croutine.c
	Source/event_groups.c
	Source/list.c
	Source/queue.c
	Source/stream_buffer.c
	Source/tasks.c
	Source/timers.c
	Source/portable/GCC/RISC-V/port.c
	Source/portable/GCC/RISC-V/portasm.S
	Source/portable/MemMang/heap_5.c
)

# 라이브러리 생성
ADD_LIBRARY ( freertos STATIC ${FREERTOS_SRCS} ${FREERTOS_HDRS} )
