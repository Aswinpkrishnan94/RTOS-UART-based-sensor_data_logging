/*
 * hw616_driver.c
 *
 *  Created on: Mar 26, 2025
 *      Author: Aswin P Krishnan
 */

#define HW616_ADDR 0x1D << 1  // I2C Address for HW-616 (Left Shift for 7-bit Addressing)

#include "hw616_driver.h"
#include "i2c.h"

/**
  * @brief  Read acceleration data from HW-616 sensor
  * @param  accel_x: Pointer to store X-axis acceleration
  * @param  accel_y: Pointer to store Y-axis acceleration
  * @param  accel_z: Pointer to store Z-axis acceleration
  * @retval 0 (Success), 1 (Error)
  */

uint8_t Read_hw616(float *accel_x, float *accel_y, float *accel_z)
{
	uint8_t data[6];
	int16_t x_raw, y_raw, z_raw;

	// Read 6 bytes from HW-616 (starting from register 0x32)
	if (HAL_I2C_Mem_Read(&hi2c1, HW616_ADDR, 0x32, I2C_MEMADD_SIZE_8BIT, data, 6, HAL_MAX_DELAY) != HAL_OK)
	{
	    return 1;  // Read error
	}

	// Combine high and low bytes for X, Y, and Z axis
	x_raw = (data[1] << 8) | data[0];	// x axis raw data
	y_raw = (data[3] << 8) | data[2];	// y axis raw data
	z_raw = (data[5] << 8) | data[4];	// z axis raw data

	// Convert raw values to acceleration in g-force (assuming 256 LSB/g)
	*accel_x = x_raw / 256.0;
	*accel_y = y_raw / 256.0;
	*accel_z = z_raw / 256.0;

	return 0;	// success

}
