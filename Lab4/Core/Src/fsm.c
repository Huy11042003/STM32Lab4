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


enum CommunicationStatus {
	START,
	WAIT,
	EXECUTE,
	END
};

enum CommunicationStatus communication_state = START;

void clear_buffer(){
	memset(buffer, 0, MAX_BUFFER_SIZE);
	index_buffer = 0;
}

void command_parser() {
	if(temp == '!'){
		clear_buffer();
	}
	if(temp == '#'){
		memcpy(command_data, buffer, MAX_BUFFER_SIZE);
		char str[3];
		HAL_UART_Transmit(&huart2, (void*)str, sprintf(str, "\r\n"),1000);
		if(strcmp((char *)command_data, "RST#") == 0){
			communication_state = EXECUTE;
			uint32_t ADC_value = 0;
			HAL_ADC_Start(&hadc1);
			ADC_value =  HAL_ADC_GetValue(&hadc1);
			sprintf((char *)res_syntax, "!ADC=%d# \r",ADC_value);
		}else if(strcmp((char *)command_data, "OK#") == 0){
			communication_state = END;
			memset(buffer, 0, MAX_BUFFER_SIZE);
		}
	}
}

void uart_communiation_fsm() {
	switch (communication_state) {
		case START:
			break;
		case EXECUTE:
			HAL_UART_Transmit(&huart2, res_syntax, sizeof(res_syntax), 1000);
			communication_state = WAIT;
			setTimer0(300);
			break;
		case WAIT:
			if (timer0_flag == 1) {
				setTimer0(300);
				communication_state = EXECUTE;
			}
			break;
		case END:
			communication_state = START;
			break;
		default:
			break;
	}
}
