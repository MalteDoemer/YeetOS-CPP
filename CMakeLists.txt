cmake_minimum_required(VERSION 3.10)
project(YeetOS ASM C CXX)

set(YEETOS_ARCH "x86")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_ASM_COMPILER clang)
set(CMAKE_LINKER clang++)
set(CMAKE_RANLIB llvm-ranlib)
set(CMAKE_STRIP llvm-strip)
set(CMAKE_AR llvm-ar)

set(YEETOS_KERNEL ${CMAKE_BINARY_DIR}/YeetOS/kernel)
set(YEETOS_GRUB_CFG ${CMAKE_SOURCE_DIR}/grub.cfg)


if("${CMAKE_BUILD_TYPE}" STREQUAL "")
set(CMAKE_BUILD_TYPE DEBUG)
endif()

add_compile_options(-fcolor-diagnostics)

add_custom_target(iso
    USES_TERMINAL
    COMMAND ${CMAKE_COMMAND} -E env "YEETOS_GRUB_CFG=${YEETOS_GRUB_CFG}" "YEETOS_KERNEL=${YEETOS_KERNEL}" "YEETOS_ARCH=${YEETOS_ARCH}" "OUT_DIR=${CMAKE_BINARY_DIR}" ${CMAKE_SOURCE_DIR}/scripts/iso.sh
)

add_custom_target(run-qemu
    USES_TERMINAL
    COMMAND ${CMAKE_COMMAND} -E env "YEETOS_ARCH=${YEETOS_ARCH}" "YEETOS_CONFIG=${CMAKE_BUILD_TYPE}" "OUT_DIR=${CMAKE_BINARY_DIR}" ${CMAKE_SOURCE_DIR}/scripts/run-qemu.sh
)

add_custom_target(debug-qemu
    USES_TERMINAL
    COMMAND ${CMAKE_COMMAND} -E env "YEETOS_ARCH=${YEETOS_ARCH}" "YEETOS_CONFIG=${CMAKE_BUILD_TYPE}" "OUT_DIR=${CMAKE_BINARY_DIR}" ${CMAKE_SOURCE_DIR}/scripts/debug-qemu.sh
)

add_subdirectory(YeetOS)