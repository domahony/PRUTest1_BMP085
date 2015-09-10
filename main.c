#include <stdint.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include "../PRUController/GY80.h"
#include "i2c.h"

volatile far pruIntc CT_INTC __attribute__((cregister("PRU_INTC", far), peripheral));
volatile pruCfg CT_CFG __attribute__((cregister("PRU_CFG", near), peripheral));

#pragma LOCATION(gy80, 0x888)
GY80 gy80;

volatile register uint32_t __R31;
volatile register uint32_t __R30;

int main(void) {
	
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
	CT_CFG.GPCFG0 = 0;

	CT_INTC.HIEISR = (1 << 0);
	CT_INTC.GER = 1;

	__R31 = 0x0;
	__R30 = 0x1;

	__R30 = (1 << 2);

	/*
	 * some notes re: gy-80...
	 */
	// Chip 1 - L883 2243 compass
	// Chip 2 - AGD8 2310 3R1EX  gyroscope
	// Chip 3 - 345B #419 3793 PHIL

	i2c_init();
	i2c_begin(0x77);
	uint16_t AC6 = i2c_read16(0xb4);
	uint16_t AC5 = i2c_read16(0xb2);
	int16_t MC = (int16_t)i2c_read16(0xbc);
	int16_t MD = (int16_t)i2c_read16(0xbe);
	i2c_end();

	long prevT = 0;
	while (1) {
		i2c_begin(0x77);
		i2c_write8(0xF4, 0x2E);
		i2c_end();
		__delay_cycles(200000 * 4.5);

		long ut = (long)i2c_read16(0xF6);

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
			gy80.temperature = T/10.;
			__R31 = ((24 - 16) | (1 << 5)); //EVENT 3
		}
		__delay_cycles(200000 * 100);
	}
	__R31 = ((24 - 16) | (1 << 5)); //EVENT 3

	__halt();  //need to code some interrupt response to break out of the loop
	return 0;
}
