/*
 * l3g4200.c
 *
 *  Created on: Sep 13, 2015
 *      Author: domahony
 */

#include "i2c.h"
#include "l3g4200.h"

#define	L3G4200_ADDRESS			(0x69)

#define REG_CTRL_REG1			(0x20)
#define PD						(1 << 3)
#define HZ_100_CO_12P5			(0x0)
#define HZ_100_CO_25			(1 << 4)
#define HZ_200_CO_12P5			(4 << 4)
#define HZ_200_CO_25			(5 << 4)
/* more to fill-in */

#define REG_CTRL_REG4			(0x23)
#define BDU						(1 << 7)
#define BLE						(1 << 6)
#define FS_250					(0)
#define FS_500					(1 << 4)
#define FS_2000A				(2 << 4)
#define FS_2000B				(3 << 4)

#define REG_CTRL_REG5			(0x24)
#define BOOT					(1 << 7)
#define FIFO_EN					(1 << 6)
#define HPEN					(1 << 4)
#define INT_SEL1				(1 << 3)
#define INT_SEL0				(1 << 2)
#define OUT_SEL1				(1 << 1)
#define OUT_SEL0				(1 << 0)

#define REG_OUT					(0x28)
#define REG_OUTXL				(0x28)
#define REG_OUTXH				(0x29)
#define REG_OUTYL				(0x2A)
#define REG_OUTYH				(0x2B)
#define REG_OUTZL				(0x2C)
#define REG_OUTZH				(0x2D)


float SF;

void
l3g4200_init()
{
	i2c_begin(L3G4200_ADDRESS);

	uint8_t ctrl1 = i2c_read8(REG_CTRL_REG1);
	ctrl1 |= PD;
	i2c_write8(REG_CTRL_REG1, ctrl1);

	i2c_write8(REG_CTRL_REG4, FS_250 | BLE | BDU);
	uint8_t ctrl4 = i2c_read8(REG_CTRL_REG4);

	i2c_write8(REG_CTRL_REG5, HPEN);

	uint32_t range;

	switch (ctrl4 & (3 << 4)) {
	case FS_250:
		range = 500;
		break;
	case FS_500:
		range = 1000;
		break;
	case FS_2000A:
		range = 4000;
		break;
	case FS_2000B:
		range = 4000;
		break;
	}
	SF = (float)range / (1 << 16);

	i2c_end();
}

void
l3g4200_get_reading(GY80* gy80)
{
	uint8_t buf[6];
	i2c_begin(L3G4200_ADDRESS);
	i2c_read(REG_OUT | 0x80, buf, sizeof(buf)/sizeof(buf[0]));
	i2c_end();

	uint16_t x = buf[0] << 8 | buf[1];
	uint16_t y = buf[2] << 8 | buf[3];
	uint16_t z = buf[4] << 8 | buf[5];
	/*
	i2c_begin(L3G4200_ADDRESS);
	int16_t x = (int16_t)i2c_read16(REG_OUTXL);
	int16_t y = (int16_t)i2c_read16(REG_OUTYL);
	int16_t z = (int16_t)i2c_read16(REG_OUTZL);
	*/

	gy80->gyroscope.x = SF * (int16_t)x;
	gy80->gyroscope.y = SF * (int16_t)y;
	gy80->gyroscope.z = SF * (int16_t)z;
}

