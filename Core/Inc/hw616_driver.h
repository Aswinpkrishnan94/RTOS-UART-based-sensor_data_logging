/*
 * hw616_driver.h
 *
 *  Created on: Mar 26, 2025
 *      Author: Aswin P Krishnan
 */

#ifndef INC_HW616_DRIVER_H_
#define INC_HW616_DRIVER_H_

#include "stm32f4xx_hal.h"

uint8_t Read_hw616(float *accel_x, float *accel_y, float *accel_z);

#endif /* INC_HW616_DRIVER_H_ */
