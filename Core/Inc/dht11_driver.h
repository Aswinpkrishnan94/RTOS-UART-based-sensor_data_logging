/*
 * dht11_driver.h
 *
 *  Created on: Mar 18, 2025
 *      Author: Aswin P Krishnan
 */

#ifndef INC_DHT11_DRIVER_H_
#define INC_DHT11_DRIVER_H_

#include "stm32f4xx_hal.h"

uint8_t Read_DHT11(float *temperature, float *humidity);

#endif /* INC_DHT11_DRIVER_H_ */
