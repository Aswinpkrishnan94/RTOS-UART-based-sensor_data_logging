/*
 * Watchdog_Task.c
 *
 *  Created on: Mar 16, 2025
 *      Author: Aswin P Krishnan
 */

#include "Watchdog_Task.h"
#include "stm32f4xx_hal.h"

extern IWDG_HandleTypeDef hiwdg;

/**
  * @brief To reset watchdog timer if tasks stop responding
  * The watchdog task periodically refreshes the watchdog timer
  * @retval none
  */
void StartWatchdogTask(void *argument)
{
	while(1)
	{
		HAL_IWDG_Refresh(&hiwdg);	// refresh watchdog timer
		osDelay(2000);				// refresh every 2s
	}
}
