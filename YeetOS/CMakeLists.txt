if(YEETOS_ARCH STREQUAL "x86")

    set(KERNEL_TARGET i686-pc-none-elf)

    set(KERNEL_LIBGCC_DIR "/usr/local/cross/lib/gcc/i686-elf/10.3.0")
    set(KERNEL_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Kernel/Arch/${YEETOS_ARCH}/Link.ld)

    set(KERNEL_ARCH_SOURCES
        Kernel/Arch/x86/Arch.cpp
        Kernel/Arch/x86/Entry.S
    )

    set(KERNEL_ARCH_COMPILE_OPTIONS
        -mno-sse
        -msoft-float
    )

else()
    message(FATAL_ERROR "unknown architecture: " ${YEETOS_ARCH})
endif()


set(KERNEL_SOURCES 
    Kernel/Main.cpp
    ${KERNEL_ARCH_SOURCES}
)

set(LIBC_SOURCES
    Libc/string.cpp
)

set(LIBYT_SOURCES
    LibYT/Verify.cpp
)

set(KERNEL_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Libc
)

set(KERNEL_COMPILE_DEFINITIONS
    KERNEL
)

set(KERNEL_COMPILE_OPTIONS
    --target=${KERNEL_TARGET}
    -Wall
    -fno-rtti
    -fno-exceptions
    -ffreestanding
    -O2
    ${KERNEL_ARCH_COMPILE_OPTIONS}
)

set(KERNEL_LINK_OPTIONS
    --target=${KERNEL_TARGET}
    -ffreestanding
    -nostdlib
    -static
    -Wl,-T${KERNEL_LINKER_SCRIPT}
)

set(KERNEL_LINK_DIRECTORIES
    ${KERNEL_LIBGCC_DIR}
)

set(KERNEL_LIBS
    gcc
    c_k
    yt_k
)

if(${CMAKE_BUILD_TYPE} MATCHES DEBUG)
    set(KERNEL_COMPILE_OPTIONS 
        -ggdb
        ${KERNEL_COMPILE_OPTIONS}
    )

    set(KERNEL_COMPILE_DEFINITIONS
        DEBUG
        ${KERNEL_COMPILE_DEFINITIONS}
    )
elseif(${CMAKE_BUILD_TYPE} MATCHES RELEASE)
    set(KERNEL_COMPILE_DEFINITIONS
        NDEBUG
        ${KERNEL_COMPILE_DEFINITIONS}
    )
endif()


add_library(c_k STATIC ${LIBC_SOURCES})
add_library(yt_k STATIC ${LIBYT_SOURCES})

add_executable(kernel ${KERNEL_SOURCES})

target_include_directories(kernel PRIVATE ${KERNEL_INCLUDE_DIRECTORIES})
target_compile_options(kernel PRIVATE ${KERNEL_COMPILE_OPTIONS})
target_compile_definitions(kernel PRIVATE ${KERNEL_COMPILE_DEFINITIONS})
target_link_options(kernel PRIVATE ${KERNEL_LINK_OPTIONS})
target_link_directories(kernel PRIVATE ${KERNEL_LINK_DIRECTORIES})
target_link_libraries(kernel PRIVATE ${KERNEL_LIBS})

target_include_directories(c_k PRIVATE ${KERNEL_INCLUDE_DIRECTORIES})
target_compile_options(c_k PRIVATE ${KERNEL_COMPILE_OPTIONS})
target_compile_definitions(c_k PRIVATE ${KERNEL_COMPILE_DEFINITIONS})

target_include_directories(yt_k PRIVATE ${KERNEL_INCLUDE_DIRECTORIES})
target_compile_options(yt_k PRIVATE ${KERNEL_COMPILE_OPTIONS})
target_compile_definitions(yt_k PRIVATE ${KERNEL_COMPILE_DEFINITIONS})
