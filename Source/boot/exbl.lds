OUTPUT_ARCH( "riscv" )

MEMORY
{
	FLASH   : ORIGIN = 0xC0000000,                              LENGTH = 0x10000
	SRAM    : ORIGIN = 0xA0000000,                              LENGTH = 0x100000
	DDR     : ORIGIN = 0x80000000,                              LENGTH = 0x4000000
}


ENTRY( reset_vector )

SECTIONS
{

  /*--------------------------------------------------------------------*/
  /* Code and read-only segment                                         */
  /*--------------------------------------------------------------------*/

  /* Begining of code and text segment */
  /*. = 0xa0000000;*/
  _ftext = .;
  PROVIDE( eprol = . );

  .text :
  {
    *(.text.init)
  } > DDR

  /* text: Program code section */
  .text : 
  {
    *(.text)
    *(.text.*)
    *(.gnu.linkonce.t.*)
  } > DDR

  /* rodata: Read-only data */
  .rodata : 
  {
    *(.rdata)
    *(.rodata)
    *(.rodata.*)
    *(.gnu.linkonce.r.*)
  } > DDR

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
  .data : 
  {
    *(.ispmdata)
    *(.data)
    *(.data.*)
    *(.srodata*)
    *(.gnu.linkonce.d.*)
    *(.comment)
  } > DDR

  /* End of initialized data segment */
  . = ALIGN(4);
  PROVIDE( edata = . );
  _edata = .;

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
  __bss_s = .;
    *(.bss)
   
    *(.bss.*)
    *(.sbss*)
    *(.gnu.linkonce.b.*)
    *(COMMON)

  __bss_e = .; 
  } > DDR

/*} > SRAM */
  .heap : {
   . = ALIGN(64);
   _heap = .;
   _heap_start = .;
   . += (4*1024*1024);
   _heap_end = .;
  } > DDR
/*} > SRAM */

  .sbi :
  {
    *(.sbi)
  }

  .payload :
  {
    *(.payload)
  }

  _end = .;
}
