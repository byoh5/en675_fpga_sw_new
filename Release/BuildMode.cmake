# ���� ����(Configuration)
SET ( CMAKE_BUILD_TYPE Release )

# "Release" ���� ���� ������ �ɼ�, ��ũ �ɼ�
SET ( CMAKE_ASM_FLAGS_RELEASE "-DRELEASE -DC_FLAGS -Os -MMD -MP -Wall -mcmodel=medany -std=gnu99 -Wno-unused -Wno-attributes -fno-delete-null-pointer-checks -g" )
SET ( CMAKE_C_FLAGS_RELEASE   "-DRELEASE -DC_FLAGS -Os -MMD -MP -Wall -mcmodel=medany -std=gnu99 -Wno-unused -Wno-attributes -fno-delete-null-pointer-checks -g" )
SET ( CMAKE_EXE_LINKER_FLAGS_RELEASE "-DRELEASE -DLINKER_FLAGS -Wl,--build-id=none -nostartfiles -static" )
