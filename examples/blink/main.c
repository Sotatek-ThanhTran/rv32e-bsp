#include "hw_registers.h"

int main() {
  volatile int *gpio_dir = (int *)GPIO_DIR;
  volatile int *gpio_out = (int *)GPIO_OUT;

  *gpio_dir = 0xFF;
  *gpio_out = 0x00;

  while (1) {
    for (int i = 0; i < 1000000; i++) {
    }
    *gpio_out = (*gpio_out) ^ 0x1;
  }
}
