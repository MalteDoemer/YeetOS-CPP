ENTRY(start)

SECTIONS {

   /* Kernel is 1MiB in physical ram */
   . = 0x100000;

   .mboot : {
      *(.mboot)
   }

   /* and 3GiB in virtual address space */
   . += 0xC0000000;


   /* 
   #####################
   #  Executable Code  #
   #####################
   */

   executable_code_start = .;

   .text ALIGN(4K) : AT(ADDR(.text) - 0xC0000000) {
      *(.text*)
   }

   executable_code_end = .;

   /* 
   #####################
   #   Readonly Data   #
   #####################
   */

   readonly_data_start = .;

   .rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000) {
      *(.rodata*)
   }

   /* clang uses .init_array and .fini_array for constructors and destructors  (unless -fno-use-init-array is used) */
   .init_array : AT(ADDR(.init_array) - 0xC0000000) {
      init_array = .;
      *(.init_array)
      LONG(0)
   }

   .fini_array : AT(ADDR(.fini_array) - 0xC0000000) {
      fini_array = .;
      *(.fini_array)
      LONG(0)
   } 

   
   /* exception handling sections */

   .eh_frame_hdr ALIGN(8) : AT(ADDR(.eh_frame_hdr) - 0xC0000000) {
      *(.eh_frame_hdr )
   }

   .eh_frame ALIGN(8) : AT(ADDR(.eh_frame) - 0xC0000000) {
      eh_frame = .;
      *(.eh_frame)
      QUAD(0);
   }

   .gcc_except_table ALIGN(8) : AT(ADDR(.gcc_except_table) - 0xC0000000) {
      *(.gcc_except_table)
   }

   readonly_data_end = .;

   /* 
   #####################
   #   Writable Data   #
   #####################
   */
   writable_data_start = .;

   .data ALIGN (4K) : AT(ADDR(.data) - 0xC0000000) {
      *(.data*)
   }

   writable_data_end = .;

   /* 
   #####################
   # Alloc Only Memory #
   #####################
   */

   alloc_only_start = .;

   .bss ALIGN (4K) (NOLOAD) : AT(ADDR(.bss) - 0xC0000000) {
      *(COMMON)
      *(.bss*)
   }

   .heap_memory ALIGN (4M) (NOLOAD) : AT(ADDR(.heap_memory) - 0xC0000000) {
      heap_memory_start = .;
      . += 4M;
      heap_memory_end = .;
   }

   alloc_only_end = .;


   /* 
   #####################
   #      Discard      #
   #####################
   */

   /DISCARD/ : {
   }
}