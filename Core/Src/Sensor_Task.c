/*
 * StartSensorTask.c
 *
 *  Created on: Mar 16, 2025
 *      Author: Aswin P Krishnan
 */

#include "Sensor_Task.h"
#include "cmsis_os.h"
#include "dht11_driver.h"
#include "stm32f4xx_hal.h"

extern osMessageQueueId_t SensorQueueHandle;
extern UART_HandleTypeDef huart3;

// Define a struct to store accelerometer data
typedef struct {
    float x;
    float y;
    float z;
} AccelData_t;

/**
  * @brief Reads acceleration data from HW-616 and
  * sends data to FreeRTOS Queue
  * @retval none
  */
void StartSensorTask(void *argument)
{
	float accel_x, accel_y, accel_z;

	AccelData_t acceldata;

	char debugMsg[100];

	HAL_UART_Transmit(&huart3, (uint8_t *)"Sensor Task Started\r\n", 22, HAL_MAX_DELAY);

	while(1)
	{
		if(Read_hw616(&accel_x,&accel_y, &accel_z)==0)	// read sensor data
		{
            acceldata.x = accel_x;
            acceldata.y = accel_y;
            acceldata.z = accel_z;

			osMessageQueuePut(SensorQueueHandle, &acceldata, 0, osWaitForever);

		// Print Debug Message to Check if Data is Sent to the Queue
		snprintf(debugMsg, sizeof(debugMsg), "Sensor Task: X = %.2fg , Y = %.2fg Z = %.2f%%\r\n", accel_x, accel_y, accel_z);
		HAL_UART_Transmit(&huart3, (uint8_t *)debugMsg, strlen(debugMsg), HAL_MAX_DELAY);
		}

		else
		{
			 HAL_UART_Transmit(&huart3, (uint8_t *)"HW616 Read Failed!\r\n", 21, HAL_MAX_DELAY);
		}

	osDelay(2000);		// read every 2s
	}
}
