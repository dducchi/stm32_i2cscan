/*
 * uar.c
 *
 *  Created on: Feb 3, 2023
 *      Author: npedu002
 */

#include "uart.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;

int _write(int file, char *p, int len) {
	HAL_UART_Transmit(&huart1, p, len, 1);
	return len;
}
