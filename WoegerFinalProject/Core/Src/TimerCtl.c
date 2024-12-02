/*
 * TimerCtl.c
 *
 *  Created on: Dec 1, 2024
 *      Author: nikkwoeg
 */


#include "TimerCtl.h"
static TIM_HandleTypeDef stopwatch;
static TIM_HandleTypeDef buzzer;


void TIM_init(uint8_t timer) {

	if (timer == TIM_BUZZER){
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOF_CLK_ENABLE();
		GPIO_InitTypeDef gpioinit;

		buzzer.Instance = TIM2;
		buzzer.Init.Prescaler = TIM_BUZZER_PRE;
		buzzer.Init.Period = TIM_BUZZER_ARR;
		buzzer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		buzzer.Init.CounterMode = TIM_COUNTERMODE_UP;
		HAL_TIM_Base_Init(&buzzer);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);

		gpioinit.Pin = GPIO_PIN_7;
		gpioinit.Mode = GPIO_MODE_OUTPUT_PP;
		gpioinit.Pull = GPIO_NOPULL;
		gpioinit.Speed = GPIO_SPEED_FAST;
		HAL_GPIO_Init(GPIOF, &gpioinit);
	}
	else if (timer == TIM_STOPWATCH){
		__HAL_RCC_TIM5_CLK_ENABLE();
		stopwatch.Instance = TIM5;
		stopwatch.Init.Prescaler = TIM_STOPWATCH_PRE; // makes 320 hz clock
		stopwatch.Init.Period = TIM_STOPWATCH_ARR;		// maximum value
		stopwatch.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2; // divides to 160hz clock
		stopwatch.Init.CounterMode = TIM_COUNTERMODE_UP;
		HAL_TIM_Base_Init(&stopwatch);
		return;
		// the above configurations make it so that the timer would reset every 410 seconds
		// which is plenty of time.
	}
}
void TIM_stopwatch_printTime() {
	TIM_stop(TIM_STOPWATCH);
	uint32_t count = __HAL_TIM_GET_COUNTER(&stopwatch);
	float printvalue = (float) count / 125;
	printf("Time of session: %f seconds\n", printvalue);
	TIM_reset(TIM_STOPWATCH);

}
void TIM_start(uint8_t timer) {
	switch (timer) {
	case TIM_BUZZER:
		HAL_TIM_Base_Start_IT(&buzzer);
		break;
	case TIM_STOPWATCH:
		HAL_TIM_Base_Start(&stopwatch);
	}
}
void TIM_stop(uint8_t timer) {
	switch (timer) {
	case TIM_BUZZER:
		HAL_TIM_Base_Stop_IT(&buzzer);
		break;
	case TIM_STOPWATCH:
		HAL_TIM_Base_Stop(&stopwatch);
	}
}
void TIM_reset(uint8_t timer) {
	if (timer == TIM_BUZZER) {
		__HAL_TIM_SET_COUNTER(&buzzer, 0);
	}
	else if (timer == TIM_STOPWATCH) {
		__HAL_TIM_SET_COUNTER(&stopwatch, 0);
	}
}

void TIM_buzzer_clearUIF() {
	__HAL_TIM_CLEAR_IT(&buzzer, TIM_IT_UPDATE);
}

void TIM_buzzer_toggle() {
	HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_7);
}

void TIM_buzzerProtocol();
