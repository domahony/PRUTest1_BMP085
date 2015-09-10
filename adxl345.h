/*
 * adxl345.h
 *
 *  Created on: Sep 10, 2015
 *      Author: domahony
 */

#include "../PRUController/GY80.h"

#ifndef ADXL345_H_
#define ADXL345_H_

void adxl345_init();
int adxl345_get_reading(GY80*);


#endif /* ADXL345_H_ */
