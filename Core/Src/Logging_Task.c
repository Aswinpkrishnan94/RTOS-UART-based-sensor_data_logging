/*
 * Logging_Task.c
 *
 *  Created on: Mar 16, 2025
 *      Author: Aswin P Krishnan
 */

#include "Logging_Task.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

extern osMessageQueueId_t SensorQueueHandle;
extern UART_HandleTypeDef huart2;

/**
  * @brief To receive data from message queue and
  * sends it to an MQTT server using AT commands over UART2
  * @retval none
  */
void StartLoggingTask(void *argument)
{
	float temp_received, humid_received;	// temperature and humidity values received
	char mqttbuffer[100];

	while(1)
	{
		osMessageQueueGet(SensorQueueHandle, &temp_received, NULL, osWaitForever);	// wait indefinitely if no data is available
		osMessageQueueGet(SensorQueueHandle, &humid_received, NULL, osWaitForever);

		sprintf(mqttbuffer, "Received Temperature: %.2f C, Received Humidity: %.2f %%", temp_received, humid_received);

		char mqttcommand[120];
		sprintf(mqttcommand, "AT+MQTTPUB=\Sensor/data\", \"%s\",0,0\r\n", mqttbuffer);
		HAL_UART_Transmit(&huart2, (uint8_t*)mqttcommand, strlen(mqttcommand), 100); // transmitting over AT commands for MQTT

		osDelay(5000);	// provide a delay of 5s before next log cycle
	}
}
