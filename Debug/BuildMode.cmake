# ���� ����(Configuration)
SET ( CMAKE_BUILD_TYPE Debug )

# "Debug" ���� ���� ������ �ɼ�, ��ũ �ɼ�
SET ( CMAKE_ASM_FLAGS_DEBUG "-DDEBUG -DC_FLAGS -O0 -MMD -MP -Wall -D__NO_INLINE__ -mcmodel=medany -std=gnu99 -Wno-unused -Wno-attributes -fno-delete-null-pointer-checks -g" )
SET ( CMAKE_C_FLAGS_DEBUG   "-DDEBUG -DC_FLAGS -O0 -MMD -MP -Wall -D__NO_INLINE__ -mcmodel=medany -std=gnu99 -Wno-unused -Wno-attributes -fno-delete-null-pointer-checks -g" )
SET ( CMAKE_EXE_LINKER_FLAGS_DEBUG "-DDEBUG -DLINKER_FLAGS -Wl,--build-id=none -nostartfiles -static" )
