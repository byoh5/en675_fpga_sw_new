# this one is important
SET(CMAKE_SYSTEM_NAME Generic)

# this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# prefix of the toolchain path
SET(TOOLCHAIN_PATH "D:/DEV_2020/en675/riscv64-unknown-elf-gcc-8.3.0-2019.08.0-x86_64-w64-mingw32/riscv64-unknown-elf-gcc-8.3.0-2019.08.0-x86_64-w64-mingw32/bin/riscv64-unknown-elf-")

IF (WIN32)
    SET(EXT ".exe")
ELSE ()
    SET(EXT "")
ENDIF ()

# specify the cross compiler
SET(CMAKE_C_COMPILER          ${TOOLCHAIN_PATH}gcc${EXT})
SET(CMAKE_CXX_COMPILER        ${TOOLCHAIN_PATH}g++${EXT})
SET(CMAKE_ASM-ATT_COMPILER    ${TOOLCHAIN_PATH}as${EXT})