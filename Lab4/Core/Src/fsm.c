/*
 * fsm.c
 *
 *  Created on: Nov 13, 2023
 *      Author: ad
 */

#include "fsm.h"

uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

uint8_t command_data[MAX_BUFFER_SIZE];
uint8_t res_syntax[MAX_BUFFER_SIZE];

enum CommandStatus {
	IDLING,
	RECEIVING
};

enum CommunicationStatus {
	WAIT_REQ,
	EXEC_REQ,
	WAIT_RES
};

enum CommandStatus parser_state = IDLING;
enum CommunicationStatus communication_state = WAIT_REQ;

void clear_buffer(){
	memset(buffer, 0, MAX_BUFFER_SIZE);
	index_buffer = 0;
}

void command_parser_fsm() {
	switch (parser_state) {
		case IDLING:
			if (temp == '!') {
				parser_state = RECEIVING;
			}
			clear_buffer();
			break;
		case RECEIVING:
			if (temp == '#') {
				parser_state = IDLING;
				memcpy(command_data, buffer, MAX_BUFFER_SIZE);
				char str[3];
				HAL_UART_Transmit(&huart2, (void*)str, sprintf(str, "\r\n"),1000);
			}
			break;
		default:
			break;
	}
}

void uart_communiation_fsm() {
	switch (communication_state) {
		case WAIT_REQ:
			if (strcmp((char *)command_data, "RST#") == 0) {
				communication_state = EXEC_REQ;
				uint32_t ADC_value = 0;
				HAL_ADC_Start(&hadc1);
				ADC_value =  HAL_ADC_GetValue(&hadc1);
				sprintf((char *)res_syntax, "!ADC=%d# \r", ADC_value);
			}
			break;
		case EXEC_REQ:
			HAL_UART_Transmit(&huart2, res_syntax, sizeof(res_syntax), 1000);
			communication_state = WAIT_RES;
			setTimer0(300);
			break;
		case WAIT_RES:
			if (strcmp((char *)command_data, "OK#") == 0) {
				communication_state = WAIT_REQ;
				memset(command_data, 0, MAX_BUFFER_SIZE);
			}
			if (timer0_flag == 1) {
				communication_state = EXEC_REQ;
			}
			break;
		default:
			break;
	}
}
