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

extern osMessageQueueId_t SensorQueueHandle, uartQueueHandle;
extern UART_HandleTypeDef huart3;

/**
  * @brief To read temperature and humidity from DHT11 sensor and
  * sends data to FreeRTOS Queue
  * @retval none
  */
void StartSensorTask(void *argument)
{
	//float temperature, humidity;
	//char debugMsg[50];

	float temperature = 25.8;  // Hardcoded test temperature
	float humidity = 55.5;     // Hardcoded test humidity
	char debugMsg[100];

	HAL_UART_Transmit(&huart3, (uint8_t *)"Sensor Task Started\r\n", 22, HAL_MAX_DELAY);

	while(1)
	{
		if(Read_DHT11(&temperature, &humidity)==0)	// read sensor data
		{
			osMessageQueuePut(SensorQueueHandle, &temperature, 0, osWaitForever);
			osMessageQueuePut(SensorQueueHandle, &humidity, 0, osWaitForever);

		// Print Debug Message to Check if Data is Sent to the Queue
		snprintf(debugMsg, sizeof(debugMsg), "Sensor Task: Temp = %.2f°C, Hum = %.2f%%\r\n", temperature, humidity);
		HAL_UART_Transmit(&huart3, (uint8_t *)debugMsg, strlen(debugMsg), HAL_MAX_DELAY);

		}
		else
		{
			 HAL_UART_Transmit(&huart3, (uint8_t *)"DHT11 Read Failed!\r\n", 21, HAL_MAX_DELAY);
		}
		osDelay(2000);		// read every 2s
	}
}
