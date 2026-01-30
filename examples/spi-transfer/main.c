#include "gpio_driver.h"
#include "spi_driver.h"

#define CS_N_PIN 2

__attribute__((unused))
static void spi_example_transfer_1(void) {
  const int BUF_SIZE = 256;
  spi_init(SPI_DIV_8MHZ);

  // Configure GPIO[4] as output for CS_N (active low), initially deasserted
  gpio_set_direction(CS_N_PIN, GPIO_OUTPUT);
  gpio_set_pin(CS_N_PIN);  // CS_N high (deasserted)

  unsigned char rx_buf[BUF_SIZE];

  gpio_clear_pin(CS_N_PIN);  // Assert CS_N
  // Read 32 bytes from SPI slave
  for (int i = 0; i < BUF_SIZE; i++) {
    rx_buf[i] = spi_transfer_byte((unsigned char)i);
  }
  gpio_set_pin(CS_N_PIN);  // Deassert CS_N

  gpio_clear_pin(CS_N_PIN);
  // Send the received data back
  for (int i = 0; i < BUF_SIZE; i++) {
    spi_transfer_byte(rx_buf[i]);
  }
  gpio_set_pin(CS_N_PIN);
}

__attribute__((unused))
static void spi_example_transfer_2(void) {
  const int BUF_SIZE = 32;
  spi_init(SPI_DIV_8MHZ);

  // Configure GPIO[4] as output for CS_N (active low), initially deasserted
  gpio_set_direction(CS_N_PIN, GPIO_OUTPUT);
  gpio_set_pin(CS_N_PIN);  // CS_N high (deasserted)

  unsigned char rx_buf[BUF_SIZE];

  // Read 32 bytes from SPI slave
  for (int i = 0; i < BUF_SIZE; i++) {
    gpio_clear_pin(CS_N_PIN);  // Assert CS_N
    rx_buf[i] = spi_transfer_byte((unsigned char)i);
    gpio_set_pin(CS_N_PIN);    // Deassert CS_N
  }

  // Send the received data back
  for (int i = 0; i < BUF_SIZE; i++) {
    gpio_clear_pin(CS_N_PIN);
    spi_transfer_byte(rx_buf[i]);
    gpio_set_pin(CS_N_PIN);
  }
}

int main(void) {
  // spi_example_transfer_1();
  spi_example_transfer_2();

  __asm__ volatile("nop");
  __asm__ volatile("nop");

  while (1) {
    // Program completed
  }
}
