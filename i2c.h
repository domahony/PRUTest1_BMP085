/*
 * i2c.h
 *
 *  Created on: Sep 9, 2015
 *      Author: domahony
 */
#include <stdint.h>

#ifndef I2C_H_
#define I2C_H_

void i2c_init();
void i2c_begin(uint8_t address);
void i2c_write8(uint8_t reg, uint8_t value);
uint8_t i2c_read8(uint8_t reg);
uint16_t i2c_read16(uint8_t reg);
uint16_t i2c_read(uint8_t reg, uint8_t* buf, uint16_t sz);
void i2c_end();


#endif /* I2C_H_ */
