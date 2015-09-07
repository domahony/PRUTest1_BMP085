#include <stdint.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include "pru_i2c.h"

volatile far pruIntc CT_INTC __attribute__((cregister("PRU_INTC", far), peripheral));
volatile pruCfg CT_CFG __attribute__((cregister("PRU_CFG", near), peripheral));
volatile far pruI2C CT_I2C __attribute__((cregister("I2C2", far), peripheral));

#pragma DATA_SECTION(I2C_CLK, ".THE_I2C_CLK")
volatile far unsigned char I2C_CLK[4];
#pragma DATA_SECTION(BUFFER, ".THE_MEMORY_LOCATION")
volatile far unsigned char BUFFER[4];

uint8_t read8(uint8_t reg);
uint16_t read16(uint8_t reg);
void write8(uint8_t reg, uint8_t val);

volatile register uint32_t __R31;
volatile register uint32_t __R30;

int main(void) {
	
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
	CT_CFG.GPCFG0 = 0;

	CT_INTC.HIEISR = (1 << 0);
	CT_INTC.GER = 1;

	__R31 = 0x0;
	__R30 = 0x1;

	I2C_CLK[0] = 0x2;
	unsigned char val;
	while (((val = (I2C_CLK[2]) & 0x3)) != 0) {
		switch (val) {
		case 1:
			__R30 = (1 << 1);
			break;
		case 2:
			__R30 = (1 << 2);
			break;
		case 3:
			__R30 = (1 << 3);
			break;
		default:
			__R30 = 0;
			break;
		}
	}

	__R30 = (1 << 2);

	/*
	 * This reset stuff is not currently working....
	CT_I2C.SYSC = 0x2;
	CT_I2C.SYSC = 0x0;

	while (CT_I2C.SYSS_bit.RDONE == 0) {};
	*/

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

	/*
	 * some notes re: gy-80...
	 */
	// Chip 1 - L883 2243 compass
	// Chip 2 - AGD8 2310 3R1EX  gyroscope
	// Chip 3 - 345B #419 3793 PHIL

	CT_I2C.CON_bit.I2C_EN = 0;

	//48mhz / 2 = 24mhz
	CT_I2C.PSC_bit.PSC = 1;

	//24mhz / (2 * 400khz) = 30
	CT_I2C.SCLL_bit.SCLL = 23; //x + 7
	CT_I2C.SCLH_bit.SCLH = 25; // x + 8
	//CT_I2C.OA_bit.OA = 0; // don't need to set this to anything for master mode
	CT_I2C.CON_bit.MST = 1;
	CT_I2C.SA = 0x77;


	CT_I2C.CON_bit.I2C_EN = 1;
	uint16_t AC6 = read16(0xb4);
	uint16_t AC5 = read16(0xb2);
	int16_t MC = (int16_t)read16(0xbc);
	int16_t MD = (int16_t)read16(0xbe);

	long prevT = 0;
	while (1) {
		write8(0xF4, 0x2E);
		__delay_cycles(200000 * 4.5);

		long ut = (long)read16(0xF6);

		long x1 = (ut - ((int32_t)AC6)) * ((int32_t)AC5) >> 15;
		long x2 = (((int32_t)MC) << 11) / (x1 + (int32_t)MD);
		long b5 = x1 + x2;
		long T = (b5 + 8) >> 4;

		if (prevT != T) {

			if (prevT < T) {
				__R30 = 1 << 3;
			} else {
				__R30 = 1 << 0;
			}

			prevT = T;
			*((long*)BUFFER) = T;
			__R31 = ((24 - 16) | (1 << 5)); //EVENT 3
		}
		__delay_cycles(200000 * 100);
	}

	__halt();  //need to code some interrupt response to break out of the loop
	return 0;
}

void write8(uint8_t reg, uint8_t val)
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

uint16_t read16(uint8_t reg)
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

uint8_t read8(uint8_t reg)
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
