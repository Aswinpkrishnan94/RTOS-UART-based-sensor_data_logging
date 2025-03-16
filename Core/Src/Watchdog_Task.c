/*
 * Watchdog_Task.c
 *
 *  Created on: Mar 16, 2025
 *      Author: Aswin P Krishnan
 */

#include "Watchdog_task.h"

void StartWatchdogTask(void *argument)
{
	for(;;)
	{
		osDelay(1);
	}
}
