#include <stdint.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include "../PRUController/GY80.h"
#include "i2c.h"
#include "bmp085.h"
#include "adxl345.h"
#include "l3g4200.h"

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

	bmp085_init();
	adxl345_init();
	l3g4200_init();

	float prevT = 0.0;
	while (1) {

		bmp085_get_reading(&gy80);
		adxl345_get_reading(&gy80);
		l3g4200_get_reading(&gy80);

		if (prevT != gy80.temperature) {

			if (prevT < gy80.temperature) {
				__R30 = 1 << 3;
			} else {
				__R30 = 1 << 0;
			}

			prevT = gy80.temperature;
		}

		__R31 = ((24 - 16) | (1 << 5)); //EVENT 3
		__delay_cycles(200000 * 100);
	}
	__R31 = ((24 - 16) | (1 << 5)); //EVENT 3

	__halt();  //need to code some interrupt response to break out of the loop
	return 0;
}
