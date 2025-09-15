// src/startup.c
#include <stdint.h>

extern int main(void);
extern unsigned long _estack;

void Reset_Handler(void);
void Default_Handler(void);
void NMI_Handler(void)      __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)__attribute__((weak, alias("Default_Handler")));

__attribute__((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
  (void (*)(void))(&_estack), // Initial SP
  Reset_Handler,              // Reset
  NMI_Handler,                // NMI
  HardFault_Handler,          // HardFault
};

void Chip_SystemInit(void); // from lpc_chip_11cxx_lib/src/system_LPC11xx.c

void Reset_Handler(void) {
  extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;
  uint32_t *src = &_sidata;
  uint32_t *dst = &_sdata;

  while (dst < &_edata) { *dst++ = *src++; }   // ← &_edata と比較
  for (dst = &_sbss; dst < &_ebss; ) { *dst++ = 0; } // ← &_ebss と比較

  Chip_SystemInit();
  main();
  for(;;){}
}

void Default_Handler(void) { for(;;){} }
