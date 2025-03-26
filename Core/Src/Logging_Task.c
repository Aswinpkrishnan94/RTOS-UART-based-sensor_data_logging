/*
 * Logging_Task.c
 *
 *  Created on: Mar 16, 2025
 *      Author: Aswin P Krishnan
 */

#include "Logging_Task.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "usart.h"

extern osMessageQueueId_t SensorQueueHandle, uartQueueHandle;
extern UART_HandleTypeDef huart3;
extern osMutexId_t uartMutexHandle;

// Define the struct
typedef struct {
    float x;
    float y;
    float z;
}AccelData_t;

/**
  * @brief To receive data from message queue and
  * sends it to an MQTT server using AT commands over UART
  * @retval none
  */
void StartLoggingTask(void *argument)
{
	AccelData_t acceldata;		// received accelerated data
	//char mqttbuffer[100];
    char uart_buffer[100];

	HAL_UART_Transmit(&huart3, (uint8_t *)"Logging Task Started\n", 23, HAL_MAX_DELAY);

	while(1)
	{
		// Retrieve Temperature and Humidity from Sensor
		if (osMessageQueueGet(SensorQueueHandle, &acceldata, NULL, osWaitForever) == osOK)
		{
		/*
		// Format MQTT Payload
		sprintf(mqttbuffer, "Received Temperature: %.2f C, Received Humidity: %.2f %%", temp_received, humid_received);

		char mqttcommand[120];

		//Format MQTT AT Command
		sprintf(mqttcommand, "AT+MQTTPUB=\Sensor/data\", \"%s\",0,0\r\n", mqttbuffer);

		//Send Command via UART
		HAL_UART_Transmit(&huart2, (uint8_t*)mqttcommand, strlen(mqttcommand), 100); // transmitting over AT commands for MQTT
		}
		*/
		// Format Debug Output for PuTTY
	    snprintf(uart_buffer, sizeof(uart_buffer), "Logging Task: X = %.2f g, Y = %.2f g, Z = %.2f g\r\n",
                 acceldata.x, acceldata.y, acceldata.z);

	    // Print Output to PuTTY via UART3
	    HAL_UART_Transmit(&huart3, (uint8_t *)uart_buffer, strlen(uart_buffer), HAL_MAX_DELAY);
		}
		else
		{
			HAL_UART_Transmit(&huart3, (uint8_t *)"Sensor Data Queue Error!\r\n", 26, HAL_MAX_DELAY);
		}

	 osDelay(2000);  // Delay of 2s before next log cycle
	}
}
