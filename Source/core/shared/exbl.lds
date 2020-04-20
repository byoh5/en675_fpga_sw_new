OUTPUT_ARCH( "riscv" )

MEMORY
{
	FLASH   : ORIGIN = 0xC0000000,                              LENGTH = 0x10000
	SRAM    : ORIGIN = 0xA0000000,                              LENGTH = 0x40000
	DDR     : ORIGIN = 0x80000000,                              LENGTH = 0x20000000
}

stack_size  = 4 * 4 * 1024;
__ram_size  = ORIGIN(SRAM) + LENGTH(SRAM);
__stack_s   = __ram_size - 4;
__stack_e   = __ram_size - stack_size;

ENTRY( reset_vector )

SECTIONS
{

  /*--------------------------------------------------------------------*/
  /* Code and read-only segment                                         */
  /*--------------------------------------------------------------------*/

  /* Begining of code and text segment */
  _ftext = .;
  PROVIDE( eprol = . );

/*  .reset : AT (ORIGIN(FLASH)) */
  .reset : AT (ORIGIN(DDR))
  {
    . = ALIGN(64);
    __reset_s = .;
    *(.text.init)
    . = ALIGN(64);
    __reset_e = .;
  } > DDR

  __reset_lma_s = LOADADDR(.reset);
  __reset_lma_e = ( (LOADADDR(.reset) + SIZEOF(.reset) + (64-1)) & ~ (64-1) );

  /* text: Program code section */
  .text : AT ( (LOADADDR(.reset) + SIZEOF(.reset) + (256-1)) & ~ (256-1) )
  {
    . = ALIGN(256);
    __text_s = .;
    *(.text)
    *(.text.*)
    *(.gnu.linkonce.t.*)
    . = ALIGN(256);
    __text_e = .;
  } > DDR

  __text_lma_s = LOADADDR(.text);
  __text_lma_e = ((LOADADDR(.text) + SIZEOF(.text) + (256-1)) & ~ (256-1));

  /* rodata: Read-only data */
  /* : AT ( (LOADADDR(.text) + SIZEOF(.text) + (64-1)) & ~ (64-1) ) */
  .rodata :
  {
    . = ALIGN(64);
    __rodata_s = .;
    *(.rdata)
    *(.rodata)
    *(.rodata.*)
    *(.gnu.linkonce.r.*)
    . = ALIGN(64);
    __rodata_e = .;
  } > DDR

  __rodata_lma_s = LOADADDR(.rodata);
  __rodata_lma_e = ((LOADADDR(.rodata) + SIZEOF(.rodata) + (64-1)) & ~ (64-1));

  /* End of code and read-only segment */
  PROVIDE( etext = . );
  _etext = .;

  /*--------------------------------------------------------------------*/
  /* HTIF, isolated onto separate page                                  */
  /*--------------------------------------------------------------------*/
  . = ALIGN(0x1000);
  .htif :
  {
    PROVIDE( __htif_base = .);
    *(.htif)
  } > DDR
  . = ALIGN(0x1000);

  /*--------------------------------------------------------------------*/
  /* Initialized data segment                                           */
  /*--------------------------------------------------------------------*/

  /* Start of initialized data segment */
  . = ALIGN(16);
   _fdata = .;

  /* data: Writable data */
  .data : AT ((LOADADDR(.rodata) + SIZEOF(.rodata) + (64-1)) & ~(64-1))
  {
    . = ALIGN(64);
    __data_s = .;
    *(.data)
    *(.data.*)
    *(.sdata)
    *(.sdata.*)
    *(.srodata*)
    *(.gnu.linkonce.d.*)
    *(.comment)
    . = ALIGN(64);
    __data_e = .;
  } > DDR

  __data_lma_s = LOADADDR(.data);
  __data_lma_e = ((LOADADDR(.data) + SIZEOF(.data) + (64-1)) & ~(64-1));

  /* End of initialized data segment */
  . = ALIGN(4);
  PROVIDE( edata = . );
  _edata = .;

  /*--------------------------------------------------------------------*/
  /* Initialized sram text/data segment                                 */
  /*--------------------------------------------------------------------*/

  . = ALIGN(64);

  .ispm_text : AT ((LOADADDR(.data) + SIZEOF(.data) + (64-1)) & ~(64-1))
  {
    . = ALIGN(64);
    __ispm_text_s = .;
    *(.ispmtext)
    *(.ispm_text_dummy)
    . = ALIGN(64);
    __ispm_text_e = .;
  } > SRAM

  __ispm_text_lma_s = LOADADDR(.ispm_text);
  __ispm_text_lma_e = ((LOADADDR(.ispm_text) + SIZEOF(.ispm_text) + (64-1)) & ~(64-1));

  .ispm_data : AT ((LOADADDR(.ispm_text) + SIZEOF(.ispm_text) + (64-1)) & ~(64-1))
  {
    . = ALIGN(64);
    __ispm_data_s = .;
    *(.ispmdata)
    . = ALIGN(64);
    __ispm_data_e = .;
  } > SRAM

  __ispm_data_lma_s = LOADADDR(.ispm_data);
  __ispm_data_lma_e = ((LOADADDR(.ispm_data) + SIZEOF(.ispm_data) + 63) & ~(64-1));

  . = ALIGN(64);

  /*--------------------------------------------------------------------*/
  /* Uninitialized data segment                                         */
  /*--------------------------------------------------------------------*/

  /* Start of uninitialized data segment */
  . = .;
  _fbss = .;

  /* sbss: Uninitialized writeable small data section */
  . = .;

  /* bss: Uninitialized writeable data section */
  . = .;
  _bss_start = .;
  .bss : 
  {
  . = ALIGN(64);
    __bss_s = .;
    *(.bss)
    *(.bss.*)
    *(.sbss*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
   . = ALIGN(64);
    __bss_e = .; 
  } > DDR

  .heap : {
   . = ALIGN(64);
   _heap = .;
   _heap_os_start = .;
   . += (16*1024*1024);
   . = ALIGN(64);
   _heap_os_end = .;
   . = ALIGN(64);
   _heap_fw_start = .;
   . += (4*1024*1024);
   . = ALIGN(64);
   _heap_fw_end = .;
   . = ALIGN(64);
   _heap_end = .;
  } > DDR

  .test_buffer : {
   . = ALIGN(64);
   _buf_start = .;
   . += (32*1024*1024);
   . = ALIGN(64);
   _buf_end = .;
  } > DDR

  .sbi :
  {
    *(.sbi)
  }

  .payload :
  {
    *(.payload)
  }

  __lma_size = ((LOADADDR(.ispm_data) + SIZEOF(.ispm_data) + (64-1)) & ~(64-1)) - LOADADDR(.reset);
  __lma_start = 0;	/* reserved */

  _end = .;
}
