/* LPC1114FN28 (32KB FLASH, 4KB RAM) */
ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 32K
  RAM   (rwx) : ORIGIN = 0x10000000, LENGTH = 4K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  .isr_vector :
  {
    KEEP(*(.isr_vector))
  } > FLASH

  .text :
  {
    *(.text*) *(.rodata*) *(.glue_7) *(.glue_7t)
    _etext = .;
  } > FLASH

  .data : AT (ADDR(.text) + SIZEOF(.text))
  {
    _sdata = .;
    *(.data*)
    _edata = .;
  } > RAM

  .bss (NOLOAD) :
  {
    _sbss = .;
    *(.bss*) *(COMMON)
    _ebss = .;
  } > RAM

  _sidata = LOADADDR(.data);
}
