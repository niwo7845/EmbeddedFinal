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
	LCD_SetFont(&Font16x24);
	LCD_Clear(0, LCD_COLOR_GREY);
	LCD_SetTextColor(LCD_COLOR_BLACK);
//	App_startScreen();
	App_DetectScreen();
}

void App_startScreen() {
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font12x12);
	LCD_DisplayStr(40, 50, "Press to start", 14);
	LCD_DisplayStr(10, 300, "Nikolai Woeger", 14);
	LCD_DrawBox_Unfilled(40, 130, 160, 80, LCD_COLOR_BLACK);
	LCD_DrawBox_Filled(41, 131, 159, 79, LCD_COLOR_WHITE);
	LCD_SetFont(&Font16x24);
	LCD_DisplayStr(85, 160, "START", 5);
	LCD_DisplayStr(15, 10, "Metal Detector", 14);
}
void App_DetectScreen() {
	if (detected == 1) {
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_Clear(0, LCD_COLOR_RED);
		LCD_SetFont(&Font12x12);
		LCD_DisplayStr(40, 60, "Metal Detected!", 15);
	}
	else {
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_Clear(0, LCD_COLOR_GREEN);
		LCD_SetFont(&Font12x12);
		LCD_DisplayStr(25, 60, "No Metal Detected", 17);
	}
	LCD_DrawBox_Unfilled(40, 130, 160, 80, LCD_COLOR_BLACK);
	LCD_DrawBox_Filled(41, 131, 159, 79, LCD_COLOR_WHITE);
	LCD_SetFont(&Font16x24);
	LCD_DisplayStr(40, 10, "Detecting...", 12);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_DisplayStr(90, 160, "STOP", 4);
}


void App_Stopwatch_init() {
	TIM_init(TIM_STOPWATCH);
}
void App_Stopwatch_test1() {
	TIM_stopwatch_getTime();
	TIM_start(TIM_STOPWATCH);
	HAL_Delay(2000);
	printf("%f\n", TIM_stopwatch_getTime());
}
void App_Buzzer_init() {
	TIM_init(TIM_BUZZER);
}
void App_Buzzer_test1() {

}
void App_Buzzer_beep() {
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
