#include "i2c_driver.h"

#define I2C_CTRL_REG (*(volatile unsigned int *)I2C_CTRL)
#define I2C_STATUS_REG (*(volatile unsigned int *)I2C_STATUS)
#define I2C_DATA_REG (*(volatile unsigned int *)I2C_DATA)
#define I2C_PRESCALE_REG (*(volatile unsigned int *)I2C_PRESCALE)

// Wait for I2C to become not busy
static void i2c_wait_not_busy(void) {
  while ((I2C_STATUS_REG & I2C_STATUS_BUSY) != 0) {
    // Wait for bus to be free
  }
}

// Wait for I2C transfer to complete
static int i2c_wait_done(void) {
  while ((I2C_STATUS_REG & I2C_STATUS_DONE) == 0) {
    // Check for errors
    if (I2C_STATUS_REG & I2C_STATUS_ERROR) {
      return I2C_ERROR;
    }
    if (I2C_STATUS_REG & I2C_STATUS_ARB_LOST) {
      return I2C_ARB_LOST;
    }
  }
  return I2C_OK;
}

// Check if ACK was received
static int i2c_check_ack(void) {
  return (I2C_STATUS_REG & I2C_STATUS_ACK) != 0;
}

// Write a byte to I2C bus (internal function)
static int i2c_write_byte_internal(unsigned char data) {
  // Step 2: Write data to DATA register
  I2C_DATA_REG = data;

  // Step 3/5: Write I2C_CTRL_START to begin sending
  I2C_CTRL_REG = I2C_CTRL_START;

  // Step 4: Loop to check status done
  int result = i2c_wait_done();
  if (result != I2C_OK) {
    return result;
  }

  // Check if ACK was received
  if (!i2c_check_ack()) {
    return I2C_NACK;
  }

  return I2C_OK;
}

// Generate STOP condition
static void i2c_stop(void) {
  // Step 6: Send I2C_CTRL_STOP (without ENABLE) to stop
  I2C_CTRL_REG = I2C_CTRL_STOP;

  // Wait for STOP to complete
  i2c_wait_done();

  // Wait for bus to be free
  i2c_wait_not_busy();
}

// Read a byte from I2C bus (internal function)
static unsigned char i2c_read_byte_internal(int send_ack) {
  // Configure read mode and ACK
  unsigned int ctrl = I2C_CTRL_READ;
  if (send_ack) {
    ctrl |= I2C_CTRL_ACK_EN;
  }

  // Step 3/5: Write I2C_CTRL_START to begin reading
  ctrl |= I2C_CTRL_START;
  I2C_CTRL_REG = ctrl;

  // Step 4: Loop to check status done
  i2c_wait_done();

  // Read data from DATA register
  return (unsigned char)I2C_DATA_REG;
}

// Public API implementation

int i2c_init(int prescale) {
  I2C_PRESCALE_REG = prescale;

  // Wait for initialization
  __asm__ volatile("nop");
  __asm__ volatile("nop");

  return I2C_OK;
}

int i2c_write(unsigned char addr, const unsigned char *data, int len) {
  if (data == 0 || len <= 0) {
    return I2C_ERROR;
  }

  // Wait for bus to be free
  i2c_wait_not_busy();

  int result;

  // Send device address (7-bit address, write bit = 0)
  result = i2c_write_byte_internal((addr << 1) | 0x00);
  if (result != I2C_OK) {
    i2c_stop();
    return result;
  }

  // Send data bytes
  for (int i = 0; i < len; i++) {
    result = i2c_write_byte_internal(data[i]);
    if (result != I2C_OK) {
      i2c_stop();
      return result;
    }
  }

  // Generate STOP condition
  i2c_stop();

  return I2C_OK;
}

int i2c_read(unsigned char addr, unsigned char *data, int len) {
  if (data == 0 || len <= 0) {
    return I2C_ERROR;
  }

  // Wait for bus to be free
  i2c_wait_not_busy();

  int result;

  // Send device address (7-bit address, read bit = 1)
  result = i2c_write_byte_internal((addr << 1) | 0x01);
  if (result != I2C_OK) {
    i2c_stop();
    return result;
  }

  // Read data bytes
  for (int i = 0; i < len; i++) {
    // Send ACK for all bytes except the last one
    int send_ack = (i < len - 1) ? 1 : 0;
    data[i] = i2c_read_byte_internal(send_ack);
  }

  // Generate STOP condition
  i2c_stop();

  return I2C_OK;
}

int i2c_write_byte(unsigned char addr, unsigned char byte) {
  return i2c_write(addr, &byte, 1);
}

int i2c_read_byte(unsigned char addr, unsigned char *byte) {
  if (byte == 0) {
    return I2C_ERROR;
  }
  return i2c_read(addr, byte, 1);
}

int i2c_scan(unsigned char *found_addrs, int max_addrs) {
  if (found_addrs == 0 || max_addrs <= 0) {
    return 0;
  }

  int count = 0;

  // Scan addresses from 0x08 to 0x77 (valid I2C address range)
  for (unsigned char addr = 0x08; addr <= 0x77 && count < max_addrs; addr++) {
    // Wait for bus to be free
    i2c_wait_not_busy();

    // Try to write address byte
    int write_result = i2c_write_byte_internal((addr << 1) | 0x00);
    i2c_stop();

    // If ACK received, device is present at this address
    if (write_result == I2C_OK) {
      found_addrs[count++] = addr;
    }
  }

  return count;
}
