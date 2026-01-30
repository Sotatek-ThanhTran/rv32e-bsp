#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "hw_registers.h"

// I2C driver return codes
#define I2C_OK 0
#define I2C_ERROR -1
#define I2C_ARB_LOST -2
#define I2C_NACK -3
#define I2C_BUSY -4

// I2C driver API

/**
 * @brief Initialize I2C module
 * @param prescale Prescaler value (use I2C_PRESCALE_100KHZ or
 * I2C_PRESCALE_400KHZ)
 * @return I2C_OK on success
 */
int i2c_init(int prescale);

/**
 * @brief Write data to I2C device
 * @param addr 7-bit I2C device address
 * @param data Pointer to data buffer
 * @param len Number of bytes to write
 * @return I2C_OK on success, negative on error
 */
int i2c_write(unsigned char addr, const unsigned char *data, int len);

/**
 * @brief Read data from I2C device
 * @param addr 7-bit I2C device address
 * @param data Pointer to data buffer
 * @param len Number of bytes to read
 * @return I2C_OK on success, negative on error
 */
int i2c_read(unsigned char addr, unsigned char *data, int len);

/**
 * @brief Write a single byte to I2C device
 * @param addr 7-bit I2C device address
 * @param byte Single byte to write
 * @return I2C_OK on success, negative on error
 */
int i2c_write_byte(unsigned char addr, unsigned char byte);

/**
 * @brief Read a single byte from I2C device
 * @param addr 7-bit I2C device address
 * @param byte Pointer to store read byte
 * @return I2C_OK on success, negative on error
 */
int i2c_read_byte(unsigned char addr, unsigned char *byte);

/**
 * @brief Scan I2C bus for devices
 * @param found_addrs Array to store found addresses (must be at least 128
 * bytes)
 * @param max_addrs Maximum number of addresses to store
 * @return Number of devices found
 */
int i2c_scan(unsigned char *found_addrs, int max_addrs);

#endif  // I2C_DRIVER_H
