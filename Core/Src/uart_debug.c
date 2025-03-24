/*
 * uart_debug.c
 *
 *  Created on: Mar 20, 2025
 *      Author: Aswin P Krishnan
 */

#include "uart_debug.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart3;  // Ensure UART3 is initialized in main.c

void Debug_Print(const char *format, ...) {
    char debugMsg[128];
    va_list args;
    va_start(args, format);
    vsnprintf(debugMsg, sizeof(debugMsg), format, args);
    va_end(args);

    strcat(debugMsg, "\r\n");  // Add new line for readability
    HAL_UART_Transmit(&huart3, (uint8_t*)debugMsg, strlen(debugMsg), 100);
}
