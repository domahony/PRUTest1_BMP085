/*
 * bmp085.h
 *
 *  Created on: Sep 10, 2015
 *      Author: domahony
 */

#include "../PRUController/GY80.h"

#ifndef BMP085_H_
#define BMP085_H_

void bmp085_init();
int bmp085_get_reading(GY80*);


#endif /* BMP085_H_ */
