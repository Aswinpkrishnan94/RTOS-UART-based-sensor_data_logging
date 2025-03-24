/*
 * uart_deug.h
 *
 *  Created on: Mar 20, 2025
 *      Author: Aswin P Krishnan
 */

#ifndef INC_UART_DEBUG_H_
#define INC_UART_DEBUG_H_

#include "stm32f4xx_hal.h"
#include <stdarg.h>

void Debug_Print(const char *format, ...);

#endif /* INC_UART_DEBUG_H_ */
