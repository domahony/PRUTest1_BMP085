/*
 * adxl345.c
 *
 *  Created on: Sep 10, 2015
 *      Author: domahony
 */
#include "i2c.h"
#include "adxl345.h"

#define ADXL345_ADDRESS  	(0x53)

#define REG_DEV_ID			(0x0)

#define REG_DATA_FORMAT 	(0x31)
#define SELF_TEST 			(1 << 7)
#define SPI 				(1 << 6)
#define INT_INVERT 			(1 << 5)
#define FULL_RES 			(1 << 3)
#define JUSTIFY 			(1 << 2)
#define RANGE_2G 			(0)
#define RANGE_4G 			(1)
#define RANGE_8G 			(2)
#define RANGE_16G 			(3)

#define REG_POWER_CTL		(0x2D)
#define	LINK				(1 << 7)
#define	AUTO_SLEEP			(1 << 4)
#define	MEASURE				(1 << 3)
#define	SLEEP				(1 << 2)
#define	WAKEUP_8HZ			(0)
#define	WAKEUP_4HZ			(1)
#define	WAKEUP_2HZ			(2)
#define	WAKEUP_1HZ			(3)

#define REG_INT_ENABLE		(0x2E)
#define	DATA_READY			(1 << 7)
#define	SINGLE_TAP			(1 << 6)
#define	DOUBLE_TAP			(1 << 5)
#define	ACTIVITY			(1 << 4)
#define	INACTIVITY			(1 << 3)
#define	FREE_FALL			(1 << 2)
#define	WATERMARK			(1 << 1)
#define	OVERRUN				(0)

#define REG_BW_RATE			(0x2C)
#define LOW_POWER			(1 << 4)
#define RATE_3200			(0xF)
#define RATE_1600			(0xE)
#define RATE_800			(0xD)
#define RATE_400			(0xC)
#define RATE_200			(0xB)
#define RATE_100			(0xA)
#define RATE_50				(0x9)
#define RATE_25				(0x8)
#define RATE_12P5			(0x7)
#define RATE_6P25			(0x6)
#define RATE_3P13			(0x5)
#define RATE_1P56			(0x4)
#define RATE_P78			(0x3)
#define RATE_P39			(0x2)
#define RATE_P2				(0x1)
#define RATE_P1				(0x0)

#define REG_DATA			(0x32)
#define REG_DATAX0			(0x32)
#define REG_DATAX1			(0x33)
#define REG_DATAY0			(0x34)
#define REG_DATAY1			(0x35)
#define REG_DATAZ0			(0x36)
#define REG_DATAZ1			(0x37)

static float SF;

void
adxl345_init()
{
	i2c_begin(ADXL345_ADDRESS);
	i2c_write8(REG_DATA_FORMAT, FULL_RES | RANGE_16G);
	//i2c_write8(REG_DATA_FORMAT, RANGE_2G);
	i2c_write8(REG_BW_RATE, RATE_400);
	i2c_write8(REG_POWER_CTL, MEASURE);
	i2c_write8(REG_INT_ENABLE, DATA_READY);

	uint8_t data_format = i2c_read8(REG_DATA_FORMAT);

	uint32_t range;
	uint32_t full_res = (data_format & FULL_RES) != 0;
	uint32_t nbits;

	switch (data_format & 0x3) {
	case RANGE_2G:
		range = 4000;
		nbits = 10;
		break;
	case RANGE_4G:
		range = 8000;
		nbits = full_res ? 11 : 10;
		break;
	case RANGE_8G:
		range = 16000;
		nbits = full_res ? 12 : 10;
		break;
	case RANGE_16G:
		range = 32000;
		nbits = full_res ? 13 : 10;
		break;
	}

	SF = (float)range / (1 << nbits);

	i2c_end();
}

int
adxl345_get_reading(GY80* gy80)
{
	uint8_t buf[6];
	i2c_begin(ADXL345_ADDRESS);
	i2c_read(REG_DATA, buf, 6);
	i2c_end();

	uint16_t x = buf[1] << 8 | buf[0];
	uint16_t y = buf[3] << 8 | buf[2];
	uint16_t z = buf[5] << 8 | buf[4];

	gy80->accelerometer.x = SF * (int16_t)x;
	gy80->accelerometer.y = SF * (int16_t)y;
	gy80->accelerometer.z = SF * (int16_t)z;

	return 1;
}
