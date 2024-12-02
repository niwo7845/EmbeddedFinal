/*
 * ApplicationCode.c
 *
 *  Created on: Nov 27, 2024
 *      Author: nikkwoeg
 */


#include "ApplicationCode.h"
void ApplicationInit(void) {
	LTCD__Init();
	LTCD_Layer_Init(0);
	ili9341_DisplayOff();
	LCD_Clear(0, LCD_COLOR_WHITE);
	ili9341_DisplayOn();

}

void App_Stopwatch_init() {
	TIM_init(TIM_STOPWATCH);
}
void App_Stopwatch_test1() {
	TIM_stopwatch_printTime();
	TIM_start(TIM_STOPWATCH);
	HAL_Delay(2000);
	TIM_stopwatch_printTime();
}
void App_Buzzer_init() {
	TIM_init(TIM_BUZZER);
}
void App_Buzzer_test1() {
	TIM_start(TIM_BUZZER);
	HAL_Delay(100);
	TIM_stop(TIM_BUZZER);
	HAL_Delay(100);
	TIM_start(TIM_BUZZER);
	HAL_Delay(100);
	TIM_stop(TIM_BUZZER);
	HAL_Delay(100);
	TIM_start(TIM_BUZZER);
	HAL_Delay(100);
	TIM_stop(TIM_BUZZER);
}

void TIM2_IRQHandler() {
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
	TIM_buzzer_clearUIF();
	HAL_NVIC_ClearPendingIRQ(TIM2_IRQn);
	TIM_buzzer_toggle();
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

}
