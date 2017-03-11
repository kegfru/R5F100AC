/* Default linker script, for normal executables */
OUTPUT_ARCH(rl78)
ENTRY(_start)

/* Do we need any of these for elf?
   __DYNAMIC = 0;    */

/* This is for an RL78/G13, 64k flash, 4k ram */
MEMORY {
/* vector table area */
	VEC (r)   : ORIGIN = 0x00000, LENGTH = 0x00002
	IVEC (r)  : ORIGIN = 0x00004, LENGTH = 0x0007c
/* option byte area */
	OPT (r)   : ORIGIN = 0x000c0, LENGTH = 0x00004

/* program area */
/* This is for an R5F100SL, 512k flash, 32k ram, 8k data flash */
/*	ROM (r)   : ORIGIN = 0x000d8, LENGTH = 0x7ff28 */ /* skip 10 bytes from 000CEH. Why? */
/* This is for an R5F100LE, 64k flash, 4k ram, 4k data flash */
/*	ROM (r)   : ORIGIN = 0x000d8, LENGTH = 0x0ff28 */
/* This is for an R5F100AC, 32k flash, 2k ram, 4k data flash */
	ROM (r)   : ORIGIN = 0x000d8, LENGTH = 0x07f28

/* This is for an R5F100SL, 512k flash, 32k ram, 8k data flash */
/*	RAM (w)   : ORIGIN = 0xf8000, LENGTH = 0x07ee0 */ /* skip 256 bytes from F7F00H. Why? */
/* This is for an R5F100LE, 64k flash, 4k ram, 4k data flash */
/*	RAM (w)   : ORIGIN = 0xfef00, LENGTH = 0x00f20 */
/* This is for an R5F100AC, 32k flash, 2k ram, 4k data flash */
	RAM (w)   : ORIGIN = 0xff700, LENGTH = 0x00720

	STACK (w) : ORIGIN = 0xffe20, LENGTH = 0x00002
}

SECTIONS
{
  .vec :
  {
    KEEP(*(.vec))
  } > VEC
  .ivec :
  {
    KEEP(*(.ivec))
  } > IVEC
  .opt :
  {
    KEEP(*(.opt))
  } > OPT

  /* CubeSuite always starts at 0xd8.  */
  .csstart : {
    *(.csstart)
  } > ROM

  /* For code that must be in the first 64k, or could fill unused
     space below .rodata.  */
  .lowtext : {
    *(.plt)
    *(.lowtext)
  } > ROM

  .data : {
    . = ALIGN(2);
    PROVIDE (__datastart = .);

    KEEP (*(.jcr))
    *(.data.rel.ro.local) *(.data.rel.ro*)
    *(.dynamic)

    *(.data D .data.* .gnu.linkonce.d.*)
    KEEP (*(.gnu.linkonce.d.*personality*))
    SORT(CONSTRUCTORS)
    *(.data1)
    *(.got.plt) *(.got)

    /* We want the small data sections together, so single-instruction offsets
       can access them all, and initialized data all before uninitialized, so
       we can shorten the on-disk segment size.  */
    . = ALIGN(2);
    *(.sdata .sdata.* .gnu.linkonce.s.* D_2 D_1)

    . = ALIGN(2);
    _edata = .;
    PROVIDE (edata = .);
    PROVIDE (__dataend = .);
  } > RAM AT> ROM

  /* Note that crt0 assumes this is a multiple of two; all the
     start/stop symbols are also assumed word-aligned.  */
  PROVIDE(__romdatastart = LOADADDR(.data));
  PROVIDE (__romdatacopysize = SIZEOF(.data));

  .bss : {
    . = ALIGN(2);
    PROVIDE (__bssstart = .);
    *(.dynbss)
    *(.sbss .sbss.*)
    *(.bss B B_2 B_1 .bss.* .gnu.linkonce.b.*)
    . = ALIGN(2);
    *(COMMON)
    . = ALIGN(2);
    PROVIDE (__bssend = .);
    _end = .;
    PROVIDE (end = .);
  } > RAM
  PROVIDE (__bsssize = SIZEOF(.bss));

  .stack (ORIGIN (STACK)) :
  {
    PROVIDE (__stack = .);
    *(.stack)
  }

  .rodata (MAX(__romdatastart + __romdatacopysize, 0x2000)) : {
    . = ALIGN(2);
    *(.plt)
    *(.rodata C C_2 C_1 .rodata.* .gnu.linkonce.r.*)
    *(.rodata1)
    *(.eh_frame_hdr)
    KEEP (*(.eh_frame))
    KEEP (*(.gcc_except_table)) *(.gcc_except_table.*)
    PROVIDE (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE (__preinit_array_end = .);
    PROVIDE (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
    PROVIDE (__init_array_end = .);
    PROVIDE (__fini_array_start = .);
    KEEP (*(.fini_array))
    KEEP (*(SORT(.fini_array.*)))
    PROVIDE (__fini_array_end = .);
    LONG(0); /* Sentinel.  */

    /* gcc uses crtbegin.o to find the start of the constructors, so
       we make sure it is first.  Because this is a wildcard, it
       doesn't matter if the user does not actually link against
       crtbegin.o; the linker won't look for a file to match a
       wildcard.  The wildcard also means that it doesn't matter which
       directory crtbegin.o is in.  */
    KEEP (*crtbegin*.o(.ctors))

    /* We don't want to include the .ctor section from from the
       crtend.o file until after the sorted ctors.  The .ctor section
       from the crtend file contains the end of ctors marker and it
       must be last */
    KEEP (*(EXCLUDE_FILE (*crtend*.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))

    KEEP (*crtbegin*.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend*.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } > ROM

  .text           :
  {
    PROVIDE (_start = .);
    *(.text P .stub .text.* .gnu.linkonce.t.*)
    KEEP (*(.text.*personality*))
    /* .gnu.warning sections are handled specially by elf32.em.  */
    *(.gnu.warning)
    *(.interp .hash .dynsym .dynstr .gnu.version*)
    PROVIDE (__etext = .);
    PROVIDE (_etext = .);
    PROVIDE (etext = .);
    . = ALIGN(2);
    KEEP (*(.init))
    KEEP (*(.fini))
  } > ROM

  /* The rest are all not normally part of the runtime image.  */

  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  /DISCARD/ : { *(.note.GNU-stack) }
}
