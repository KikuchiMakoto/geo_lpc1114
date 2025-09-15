#include "chip.h"

const uint32_t OscRateIn = 12000000;
const uint32_t ExtRateIn = 0;

static void delay(volatile uint32_t n){ while(n--) __asm__("nop"); }

int main(void){
  SystemCoreClockUpdate();

  // 必要クロック
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_IOCON);
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_GPIO);

  // P0_1 を GPIO に設定（FUNC0）。外部でプルアップ済みなら INACTでOK。
  Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_1, (IOCON_FUNC0 /*| IOCON_MODE_INACT*/));

  // 出力化
  Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 1);

  // アクティブLOWなので、初期はH=消灯（外部プルアップでも明示しておく）
  Chip_GPIO_SetPinState(LPC_GPIO, 0, 1, true);

  for(;;){
    // LOWで点灯
    Chip_GPIO_SetPinState(LPC_GPIO, 0, 1, false);
    delay(600000);

    // HIGHで消灯
    Chip_GPIO_SetPinState(LPC_GPIO, 0, 1, true);
    delay(600000);
  }
}
