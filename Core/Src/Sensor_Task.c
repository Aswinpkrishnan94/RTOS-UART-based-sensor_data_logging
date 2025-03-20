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

/**
  * @brief To read temperature and humidity from DHT11 sensor and
  * sends data to FreeRTOS Queue
  * @retval none
  */
void StartSensorTask(void *argument)
{
	float temperature, humidity;
	while(1)
	{
		Read_DHT11(&temperature, &humidity);	// read sensor data

		char dmesg[50];
		sprintf(dmesg, "Temperature: %.2f, Humidity: %.2f\n", temperature, humidity);
		HAL_UART_Transmit(&huart3, (uint8_t*)dmesg, strlen(dmesg), 100);	// debugging

		osMessageQueuePut(SensorQueueHandle, &temperature, 0, osWaitForever);
		osMessageQueuePut(SensorQueueHandle, &humidity, 0, osWaitForever);

		osDelay(2000);		// read every 2s
	}
}
