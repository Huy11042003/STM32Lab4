/*
 * fsm.h
 *
 *  Created on: Nov 13, 2023
 *      Author: ad
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "software_timer.h"

#define MAX_BUFFER_SIZE		30


void command_parser_fsm();
void uart_communiation_fsm();

extern ADC_HandleTypeDef hadc1;

extern UART_HandleTypeDef huart2;

extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;

#endif /* INC_FSM_H_ */
