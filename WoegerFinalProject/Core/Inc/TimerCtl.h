/*
 * TimerCtl.h
 *
 *  Created on: Dec 1, 2024
 *      Author: nikkwoeg
 */

#ifndef INC_TIMERCTL_H_
#define INC_TIMERCTL_H_

#include "stm32f4xx_hal.h"
#include <stdio.h>

#define		TIM_BUZZER 0
#define		TIM_STOPWATCH 1
#define		TIM_STOPWATCH_PRE	0xC349    // makes 320 hz clock
#define		TIM_STOPWATCH_ARR	0xFFFF

#define 	TIM_BUZZER_PRE		0x200
#define		TIM_BUZZER_ARR		0x9


void TIM_init(uint8_t timer);
void TIM_stopwatch_printTime();
void TIM_start(uint8_t timer);
void TIM_stop(uint8_t timer);
void TIM_reset(uint8_t timer);
void TIM_buzzer_clearUIF();
void TIM_buzzer_toggle();

void TIM_buzzerProtocol();


#endif /* INC_TIMERCTL_H_ */
