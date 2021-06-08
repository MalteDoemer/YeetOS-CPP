if(YEETOS_ARCH STREQUAL "x86")

    set(KERNEL_TARGET i686-pc-none-elf)

    set(KERNEL_LIBGCC_DIR "/usr/local/cross/lib/gcc/i686-elf/10.3.0")
    set(KERNEL_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/Kernel/Arch/${YEETOS_ARCH}/Link.ld)

    set(KERNEL_ARCH_SOURCES
        Kernel/Arch/x86/Arch.cpp
        Kernel/Arch/x86/Init.cpp
        Kernel/Arch/x86/DebugLog.cpp
        Kernel/Arch/x86/Entry.S
    )

    set(KERNEL_ARCH_COMPILE_OPTIONS
        -mno-sse
        -msoft-float
    )

    set(KERNEL_ARCH_COMPILE_DEFINITIONS
        __i386__
    )

else()
    message(FATAL_ERROR "unknown architecture: " ${YEETOS_ARCH})
endif()


set(KERNEL_SOURCES 
    Kernel/Main.cpp
    ${KERNEL_ARCH_SOURCES}
)

set(LIBCK_SOURCES
    Libc/string.cpp
    Libc/stdlib.cpp
    Libc/stdio.cpp
    Libc/assert.cpp
    Libc/pthread.cpp
    Libc/sched.cpp
    Libc/dlfcn.cpp
)

set(LIBC_SOURCES
    ${LIBCK_SOURCES}
)

set(LIBYT_SOURCES
    LibYT/Verify.cpp
    LibYT/New.cpp
)

set(LIBALLOC_SOURCES
    Liballoc/Alloc.cpp
)

set(CXXRT_SOURCES
    Libcxxrt/Guard.cpp
    Libcxxrt/TypeInfo.cpp
    Libcxxrt/AuxHelper.cpp
    Libcxxrt/DynamicCast.cpp
    Libcxxrt/Exception.cpp
)

set(KERNEL_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Libc
    ${CMAKE_CURRENT_SOURCE_DIR}/LibYT
)

set(KERNEL_COMPILE_DEFINITIONS
    YEETOS_KERNEL
    ${KERNEL_ARCH_COMPILE_DEFINITIONS}
)

set(KERNEL_COMPILE_OPTIONS
    --target=${KERNEL_TARGET}
    -Wall
    -ffreestanding
    -O2
    ${KERNEL_ARCH_COMPILE_OPTIONS}
)

#-fno-use-init-array
#-fno-rtti
#-fno-exceptions    

set(KERNEL_LINK_OPTIONS
    --target=${KERNEL_TARGET}
    -ffreestanding
    -nostdlib
    -static
    -Wl,--build-id=none
    -Wl,-T${KERNEL_LINKER_SCRIPT}
)

set(KERNEL_LINK_DIRECTORIES
    ${KERNEL_LIBGCC_DIR}
)

set(KERNEL_LIBS
    cxxrt_k
    gcc
    c_k
    yt_k
    alloc_k
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


add_library(c_k OBJECT ${LIBCK_SOURCES})
add_library(yt_k OBJECT ${LIBYT_SOURCES})
add_library(alloc_k OBJECT ${LIBALLOC_SOURCES})
add_library(cxxrt_k OBJECT ${CXXRT_SOURCES})

add_executable(kernel ${KERNEL_SOURCES})

target_include_directories(kernel PUBLIC ${KERNEL_INCLUDE_DIRECTORIES})
target_compile_options(kernel PUBLIC ${KERNEL_COMPILE_OPTIONS})
target_compile_definitions(kernel PUBLIC ${KERNEL_COMPILE_DEFINITIONS})
target_link_options(kernel PUBLIC ${KERNEL_LINK_OPTIONS})
target_link_directories(kernel PUBLIC ${KERNEL_LINK_DIRECTORIES})
target_link_libraries(kernel PUBLIC ${KERNEL_LIBS})

target_include_directories(c_k PUBLIC ${KERNEL_INCLUDE_DIRECTORIES})
target_compile_options(c_k PUBLIC ${KERNEL_COMPILE_OPTIONS})
target_compile_definitions(c_k PUBLIC ${KERNEL_COMPILE_DEFINITIONS})

target_include_directories(yt_k PUBLIC ${KERNEL_INCLUDE_DIRECTORIES})
target_compile_options(yt_k PUBLIC ${KERNEL_COMPILE_OPTIONS})
target_compile_definitions(yt_k PUBLIC ${KERNEL_COMPILE_DEFINITIONS})

target_include_directories(cxxrt_k PUBLIC ${KERNEL_INCLUDE_DIRECTORIES})
target_compile_options(cxxrt_k PUBLIC ${KERNEL_COMPILE_OPTIONS})
target_compile_definitions(cxxrt_k PUBLIC ${KERNEL_COMPILE_DEFINITIONS})

target_include_directories(alloc_k PUBLIC ${KERNEL_INCLUDE_DIRECTORIES})
target_compile_options(alloc_k PUBLIC ${KERNEL_COMPILE_OPTIONS})
target_compile_definitions(alloc_k PUBLIC ${KERNEL_COMPILE_DEFINITIONS})