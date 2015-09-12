/*
 * i2c.c
 *
 *  Created on: Sep 9, 2015
 *      Author: domahony
 */

#include <stdint.h>
#include <pru_cfg.h>
#include "pru_i2c.h"
#include "i2c.h"

volatile far pruI2C CT_I2C __attribute__((cregister("I2C2", far), peripheral));

#pragma DATA_SECTION(I2C_CLK, ".THE_I2C_CLK")
volatile far unsigned char I2C_CLK[4];

void i2c_init() {

	CT_I2C.CON_bit.I2C_EN = 1;
	I2C_CLK[0] = 0x2;
	while ((I2C_CLK[2] & 0x3) != 0) { }

	CT_I2C.SYSC_bit.SRST = 0x2;
	while (CT_I2C.SYSS_bit.RDONE == 0) {};
	CT_I2C.CON_bit.I2C_EN = 0;

	/*
	 * Some pinmux notes....
	 */
	//I2C2_SCL pinmux 0x73 pin19 (0x17c)
	// -> 111 0011
	// -> bit 6: 1: slow
	// -> bit 5: 1: output
	// -> bit 34: 10: pullup
	// -> bit 012: 011: mode 3

	//I2C2_SDA pinmux 0x73 pin20 (0x178)
	// -> 111 0011
	// -> bit 6: 1: slow
	// -> bit 5: 1: output
	// -> bit 34: 10: pullup
	// -> bit 012: 011: mode 3

	// pinmux 0x37
	// -> 011 0111
	// -> bit 6: 0: fast
	// -> bit 5: 1: output
	// -> bit 34: 10: pullup
	// -> bit 012: 111: mode 7

	// pinmux 0x2c
	// -> 010 0100
	// -> bit 6: 0: fast
	// -> bit 5: 1: output
	// -> bit 34: 00: pulldown
	// -> bit 012: 100: mode 4


	//48mhz / 2 = 24mhz
	CT_I2C.PSC_bit.PSC = 1;

	//24mhz / (2 * 400khz) = 30
	CT_I2C.SCLL_bit.SCLL = 23; //x + 7
	CT_I2C.SCLH_bit.SCLH = 25; // x + 8
	//CT_I2C.OA_bit.OA = 0; // don't need to set this to anything for master mode
	CT_I2C.CON_bit.MST = 1;
	CT_I2C.CON_bit.I2C_EN = 1;
}

void i2c_begin(uint8_t address) {
	CT_I2C.SA_bit.SA = address;
}

void i2c_end()
{
}

void i2c_write8(uint8_t reg, uint8_t val)
{
	//write 1 byte register address
	CT_I2C.CNT = 2;
	CT_I2C.DATA = reg;
	CT_I2C.DATA = val;

	while(CT_I2C.IRQSTATUS_RAW_bit.BB != 0) {}

	/*
	CMD_ENABLE CMD_MASTER CMD_TX CMD_STP CMD_START
	*/
	// need #defines for all these bits...  can I set individual bits? what iniitiates the activity?
	CT_I2C.CON = (1 << 15) | (1 << 10) | (1 << 9) | (1 << 1) | (1 << 0);
	while (CT_I2C.CON_bit.STP != 0) {};
}

uint16_t
i2c_read16(uint8_t reg)
{
	//write 1 byte register address
	CT_I2C.CNT = 1;
	CT_I2C.DATA = reg;

	while(CT_I2C.IRQSTATUS_RAW_bit.BB != 0) {}
	/*
	CMD_ENABLE CMD_MASTER CMD_TX CMD_START
	*/
	// need #defines for all these bits...  can I set individual bits? what iniitiates the activity?
	CT_I2C.CON = (1 << 15) | (1 << 10) | (1 << 9) | (1 << 0);
	//wait by count
	while (CT_I2C.CNT != 0) {};

	CT_I2C.CNT = 2;
	CT_I2C.CON = (1 << 15) | (1 << 10) | (0 << 9) | (1 << 1) | (1 << 0);
	while (CT_I2C.CON_bit.STP != 0) {};

	uint16_t ret = (CT_I2C.DATA_bit.DATA << 8);
	ret |= CT_I2C.DATA_bit.DATA;

	return ret;
}
uint16_t
i2c_read(uint8_t reg, uint8_t* buf, uint16_t sz)
{
		//write 1 byte register address
	CT_I2C.CNT = 1;
	CT_I2C.DATA = reg;

	while(CT_I2C.IRQSTATUS_RAW_bit.BB != 0) {}
	/*
	CMD_ENABLE CMD_MASTER CMD_TX CMD_START
	*/
	// need #defines for all these bits...  can I set individual bits? what iniitiates the activity?
	CT_I2C.CON = (1 << 15) | (1 << 10) | (1 << 9) | (1 << 0);
	//wait by count
	while (CT_I2C.CNT != 0) {};

	CT_I2C.CNT = sz;
	CT_I2C.CON = (1 << 15) | (1 << 10) | (0 << 9) | (1 << 1) | (1 << 0);
	while (CT_I2C.CON_bit.STP != 0) {};

	int i;
	for (i = 0; i < sz; i++) {
		buf[i] = CT_I2C.DATA_bit.DATA;
	}

	return sz;
}

uint8_t i2c_read8(uint8_t reg)
{
	//write 1 byte register address
	CT_I2C.CNT = 1;
	CT_I2C.DATA = reg;

	while(CT_I2C.IRQSTATUS_RAW_bit.BB != 0) {}
	/*
	CMD_ENABLE CMD_MASTER CMD_TX CMD_START
	*/
	CT_I2C.CON = (1 << 15) | (1 << 10) | (1 << 9) | (1 << 0);
	//wait by count
	while (CT_I2C.CNT != 0) {};

	CT_I2C.CNT = 1;
	CT_I2C.CON = (1 << 15) | (1 << 10) | (0 << 9) | (1 << 1) | (1 << 0);
	while (CT_I2C.CON_bit.STP != 0) {};

	//read 1 byte value
	return CT_I2C.DATA_bit.DATA;
}
