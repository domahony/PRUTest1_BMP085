/*
 * adxl345.c
 *
 *  Created on: Sep 10, 2015
 *      Author: domahony
 */
#include "i2c.h"
#include "adxl345.h"

static uint8_t devid;

void
adxl345_init()
{
	i2c_begin(0x53);
	devid = i2c_read8(0);
	i2c_end();
}

int
adxl345_get_reading(GY80* gy80)
{
	gy80->accelerometer.devid = devid;
	return 1;
}
