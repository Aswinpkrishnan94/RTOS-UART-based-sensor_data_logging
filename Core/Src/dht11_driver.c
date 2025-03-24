/*
 * dht11_driver.c
 *
 *  Created on: Mar 18, 2025
 *      Author: Aswin P Krishnan
 */
#include "dht11_driver.h"
#include "stm32f4xx_hal.h"

#define DHT11_PORT	GPIOB
#define DHT11_PIN	GPIO_PIN_5

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
	HAL_GPIO_Init(DHT11_PORT, &GPIO_Initstruct);

	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);	// pull low
	HAL_Delay(18);	// wait for 18 ms
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);		// pull high
	HAL_Delay(30);	// wait for 30 ms

	GPIO_Initstruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(DHT11_PORT, &GPIO_Initstruct);

}

/**
  * @brief To read data by byte
  *	If signal is HIGH for greater than 40 ms, then output is one, else output is zero
  * @retval data (byte received from the sensor)
  */
uint8_t ReadByte()
{
	uint8_t data;

	for(uint8_t i=0;i<8;i++)
	{
		while(!(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))); // wait for high signal
		HAL_Delay(40); // wait for 40 ms

		if(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
		{
			data |= (1 << (7 - i));	// store bit in the data
		}

		while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));	// wait for low signal
	}
	return data;
}

/**
  * @brief To read and store humidity and temperature
  *	4  bytes received are verified using a checksum
  * @retval none
  */
void Read_DHT11(float *temperature, float *humidity)
{
	uint8_t rhbyte1, rhbyte2, tempbyte1, tempbyte2, checksum;

	dht11_start();	// start the operation

	rhbyte1   = ReadByte();		// integer part of humidity
	rhbyte2   = ReadByte();		// decimal part of humidity
	tempbyte1 = ReadByte();		// integer part of temperature
	tempbyte2 = ReadByte();		// decimal part of temperature
	checksum  = ReadByte();		// checksum

	if((rhbyte1 + rhbyte2 + tempbyte1 + tempbyte2)==checksum)
	{
		*humidity 	 = (float)rhbyte1;		// store humidity
		*temperature = (float)tempbyte1;	// store temperature
	}
}
