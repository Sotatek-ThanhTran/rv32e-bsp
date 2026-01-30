#include "gpio_driver.h"

#define GPIO_DIR_REG (*(volatile unsigned int *)GPIO_DIR)
#define GPIO_OUT_REG (*(volatile unsigned int *)GPIO_OUT)
#define GPIO_IN_REG (*(volatile unsigned int *)GPIO_IN)

int gpio_set_direction(unsigned int pin, int direction) {
  if (pin > GPIO_PIN_MAX) {
    return GPIO_ERROR;
  }
  unsigned int mask = 1u << pin;
  if (direction == GPIO_OUTPUT) {
    GPIO_DIR_REG |= mask;
  } else {
    GPIO_DIR_REG &= ~mask;
  }
  return GPIO_OK;
}

void gpio_set_direction_mask(unsigned int mask) { GPIO_DIR_REG = mask; }

int gpio_set_pin(unsigned int pin) {
  if (pin > GPIO_PIN_MAX) {
    return GPIO_ERROR;
  }
  GPIO_OUT_REG |= (1u << pin);
  return GPIO_OK;
}

int gpio_clear_pin(unsigned int pin) {
  if (pin > GPIO_PIN_MAX) {
    return GPIO_ERROR;
  }
  GPIO_OUT_REG &= ~(1u << pin);
  return GPIO_OK;
}

int gpio_toggle_pin(unsigned int pin) {
  if (pin > GPIO_PIN_MAX) {
    return GPIO_ERROR;
  }
  GPIO_OUT_REG ^= (1u << pin);
  return GPIO_OK;
}

int gpio_write_pin(unsigned int pin, int value) {
  if (pin > GPIO_PIN_MAX) {
    return GPIO_ERROR;
  }
  unsigned int mask = 1u << pin;
  if (value) {
    GPIO_OUT_REG |= mask;
  } else {
    GPIO_OUT_REG &= ~mask;
  }
  return GPIO_OK;
}

void gpio_write(unsigned int value) { GPIO_OUT_REG = value; }

int gpio_read_pin(unsigned int pin) {
  if (pin > GPIO_PIN_MAX) {
    return GPIO_ERROR;
  }
  return (GPIO_IN_REG >> pin) & 1u;
}

unsigned int gpio_read(void) { return GPIO_IN_REG; }
