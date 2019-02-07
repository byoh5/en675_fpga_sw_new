# this one is important
SET(CMAKE_SYSTEM_NAME Generic)

# this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# prefix of the toolchain path
SET(TOOLCHAIN_PATH "E:/EN675/eclipse/tool/riscv64-unknown-elf-gcc-20180928-x86_64-w64-mingw32/bin/riscv64-unknown-elf-")

# specify the cross compiler
SET(CMAKE_C_COMPILER          ${TOOLCHAIN_PATH}gcc.exe)
SET(CMAKE_CXX_COMPILER        ${TOOLCHAIN_PATH}g++.exe)
SET(CMAKE_ASM-ATT_COMPILER    ${TOOLCHAIN_PATH}as.exe)