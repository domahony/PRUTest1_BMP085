#include <stdint.h>
#include "bmp085.h"
#include "i2c.h"

#define BMP085_ADDRESS 0x77

static	uint16_t AC6;
static	uint16_t AC5;
static	int16_t MC;
static	int16_t MD;

void
bmp085_init()
{
	i2c_begin(BMP085_ADDRESS);
	AC6 = i2c_read16(0xb4);
	AC5 = i2c_read16(0xb2);
	MC = (int16_t)i2c_read16(0xbc);
	MD = (int16_t)i2c_read16(0xbe);
	i2c_end();
}

int
bmp085_get_reading(GY80* gy80) {
	
		i2c_begin(BMP085_ADDRESS);
		i2c_write8(0xF4, 0x2E);
		i2c_end();
		__delay_cycles(200000 * 4.5);

		long ut = (long)i2c_read16(0xF6);

		long x1 = (ut - ((int32_t)AC6)) * ((int32_t)AC5) >> 15;
		long x2 = (((int32_t)MC) << 11) / (x1 + (int32_t)MD);
		long b5 = x1 + x2;
		long T = (b5 + 8) >> 4;

		gy80->temperature = T/10.;

		return 1;
}
