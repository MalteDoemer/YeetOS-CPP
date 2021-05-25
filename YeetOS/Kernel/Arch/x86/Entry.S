.section .mboot, "awx"

.global start
.intel_syntax noprefix


MULTIBOOT2_HEADER_MAGIC = 0xe85250d6
MULTIBOOT2_ARCHITECTURE_I386 = 0       
KERNEL_BASE = 0xC0000000

.align 8
mboot_header:
    //magic number
    .long MULTIBOOT2_HEADER_MAGIC

    //architecure
    .long MULTIBOOT2_ARCHITECTURE_I386
          

    //header size
    .long mboot_header_end - mboot_header

    //checksum
    .long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT2_ARCHITECTURE_I386 + mboot_header_end - mboot_header)
    
    .long 0
    .long 8

mboot_header_end:

start:
    cli
    cld


    // tell the MMU where to find the page directory
    lea ecx, boot_page_dir
    mov cr3, ecx

    // set PSE bit in CR4 to enable 4MiB pages.
    mov ecx, cr4
    or ecx, 0x00000010
    mov cr4, ecx

    // set PG bit in CR0 to enable paging
    mov ecx, cr0
    or ecx, 0x80000000
    mov cr0, ecx

    // jump into higher half Yey
    jmp up


.align 4*1024

boot_page_dir:
    .long 0x00000083
    .fill ((KERNEL_BASE >> 22) - 1), 4, 0
    .long 0x00000083
    .long 0x00400083
    .fill (1024 - (KERNEL_BASE >> 22) - 2), 4, 0

boot_page_dir_end:


.text

up:

    mov dword ptr [boot_page_dir + KERNEL_BASE], 0
    invlpg [0]

init:

    // setup stack pointer and base pointer
    lea esp, kernel_stack_top
    mov ebp, esp

    xor ecx, ecx
    push ecx
    push ecx

    // correct mboot structure for higher half
    add ebx, KERNEL_BASE


    // arch_eraly_init(void* multiboot_info, uint32_t multiboot_magic)
    // used for early initialization e.g heap allocator, logger
    push eax
    push ebx
    call arch_early_init
    add esp, 8


loop:
    hlt
    jmp loop


.bss

kernel_stack:
.fill 1024 * 4
kernel_stack_top: