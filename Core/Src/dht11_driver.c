/*
 * dht11_driver.c
 *
 *  Created on: Mar 18, 2025
 *      Author: Aswin P Krishnan
 */
#include "dht11_driver.h"
#include "stm32f4xx_hal.h"

#define DHT11_PORT	GPIOB
#define DHT11_PIN	GPIO_PIN_0

extern UART_HandleTypeDef huart3;	 // UART3 for PuTTY output

/**
  * @brief To start Reading from DHT11 sensor
  *	Data line pulled low for 18 ms to wake up the sensor
  *	Data line pulled high for 30 ms to prepare the sensor to read data
  *	Port set to input mode, to receive data
  * @retval None
  */
void dht11_start()
{
	GPIO_InitTypeDef GPIO_Initstruct={0};
	GPIO_Initstruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Initstruct.Pin = DHT11_PIN;
	GPIO_Initstruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DHT11_PORT, &GPIO_Initstruct);

	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);	// pull low
	HAL_Delay(18);	// wait for 18 ms
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);		// pull high
	HAL_Delay(30);	// wait for 30 ms

	GPIO_Initstruct.Mode = GPIO_MODE_INPUT;
	GPIO_Initstruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DHT11_PORT, &GPIO_Initstruct);

	HAL_UART_Transmit(&huart3, (uint8_t *)"DHT11: GPIO Switched to Input\r\n", 32, HAL_MAX_DELAY);

	// Check if DHT11 pulls LOW after start signal
	uint32_t timeout = 100000;
	while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
	{
		if (--timeout == 0)
		{
	        HAL_UART_Transmit(&huart3, (uint8_t *)"DHT11 Error: No Response from Sensor!\r\n", 38, HAL_MAX_DELAY);
	        return;
	    }
	}
	HAL_UART_Transmit(&huart3, (uint8_t *)"DHT11: Sensor Response Detected\r\n", 34, HAL_MAX_DELAY);
}

/**
  * @brief To read data by byte
  *	If signal is HIGH for greater than 40 ms, then output is one, else output is zero
  * @retval data (byte received from the sensor)
  */
uint8_t ReadByte()
{
	 uint8_t data = 0;
	 char debugMsg[50];

	 for (uint8_t i = 0; i < 8; i++)
	 {
	         uint32_t timeout = 100000;

	         // Wait for HIGH signal
	         while (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) {
	             if (--timeout == 0) {
	                 HAL_UART_Transmit(&huart3, (uint8_t *)"ReadByte: Stuck at LOW\r\n", 24, HAL_MAX_DELAY);
	                 return 0;  // Error: Never goes HIGH
	             }
	         }

	         uint32_t start_time = HAL_GetTick();

	         // Wait for LOW signal
	         while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) {
	             if (--timeout == 0) {
	                 HAL_UART_Transmit(&huart3, (uint8_t *)"ReadByte: Stuck at HIGH\r\n", 25, HAL_MAX_DELAY);
	                 return 0;  // Error: Never goes LOW
	             }
	         }

	         uint32_t duration = HAL_GetTick() - start_time;

	         snprintf(debugMsg, sizeof(debugMsg), "Bit %d -> %d (Duration: %lu) GPIO: %d\r\n",
	                  7 - i, (duration > 40) ? 1 : 0, duration, HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));
	         HAL_UART_Transmit(&huart3, (uint8_t *)debugMsg, strlen(debugMsg), HAL_MAX_DELAY);

	         if (duration > 40)
	         {
	             data |= (1 << (7 - i));
	         }
	 }
	return data;
}

/**
  * @brief To read and store humidity and temperature
  *	4  bytes received are verified using a checksum
  * @retval none
  */

/*
uint8_t Read_DHT11(float *temperature, float *humidity)
{
	uint8_t rhbyte1, rhbyte2, tempbyte1, tempbyte2, checksum;
	char debug_msg[50];

	HAL_UART_Transmit(&huart3, (uint8_t *)"DHT11: Sending Start Signal\r\n", 30, HAL_MAX_DELAY);
	dht11_start();	// start the operation

	HAL_UART_Transmit(&huart3, (uint8_t *)"DHT11: Reading Data Bytes...\r\n", 30, HAL_MAX_DELAY);

	rhbyte1   = ReadByte();		// integer part of humidity
	rhbyte2   = ReadByte();		// decimal part of humidity
	tempbyte1 = ReadByte();		// integer part of temperature
	tempbyte2 = ReadByte();		// decimal part of temperature
	checksum  = ReadByte();		// checksum

	snprintf(debug_msg, sizeof(debug_msg), "Raw Data -> RH: %d.%d, Temp: %d.%d, Chksum: %d\r\n",
	        rhbyte1, rhbyte2, tempbyte1, tempbyte2, checksum);
	HAL_UART_Transmit(&huart3, (uint8_t *)debug_msg, strlen(debug_msg), HAL_MAX_DELAY);

	if((rhbyte1 + rhbyte2 + tempbyte1 + tempbyte2)==checksum)
	{
		*humidity 	 = (float)rhbyte1;		// store humidity
		*temperature = (float)tempbyte1;	// store temperature

	// Print Sensor Data to PuTTY before Uploading to Cloud
		snprintf(debug_msg, sizeof(debug_msg), "DHT11: Temp = %.2f°C, Hum = %.2f%%\r\n", *temperature, *humidity);
		HAL_UART_Transmit(&huart3, (uint8_t *)debug_msg, strlen(debug_msg), HAL_MAX_DELAY);
		return 0;
	}
	else
	{
		HAL_UART_Transmit(&huart3, (uint8_t *)"DHT11 Read Error\r\n", 19, HAL_MAX_DELAY);
		return 1;	// error
	}
}*/

uint8_t Read_DHT11(float *temperature, float *humidity) {
    if (temperature == NULL || humidity == NULL) {
        HAL_UART_Transmit(&huart3, (uint8_t *)"DHT11 Error: NULL Pointer!\r\n", 29, HAL_MAX_DELAY);
        return 1;  // Return error
    }

    *humidity = 55.5;   // Hardcoded test values
    *temperature = 25.8;

    char debug_msg[100];
    snprintf(debug_msg, sizeof(debug_msg), "Simulated DHT11 Data: Temp = %.2f°C, Hum = %.2f%%\r\n",
             *temperature, *humidity);
    HAL_UART_Transmit(&huart3, (uint8_t *)debug_msg, strlen(debug_msg), HAL_MAX_DELAY);

    return 0;
}


