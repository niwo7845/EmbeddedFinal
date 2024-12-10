/*
 * ApplicationCode.c
 *
 *  Created on: Nov 27, 2024
 *      Author: nikkwoeg
 */


#include "ApplicationCode.h"
static EXTI_HandleTypeDef EXTI15;
static uint8_t detected;
static uint8_t detected_in_session = 0;
static uint8_t CurrentScreen;
static uint8_t count;
static float lastTime;
static uint8_t exitIT;
static STMPE811_TouchData t;

void ApplicationInit(void) {
	LTCD__Init();
	LTCD_Layer_Init(0);
	LCD_SetFont(&Font16x24);
	LCD_Clear(0, LCD_COLOR_GREY);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	App_Buzzer_init();
	EXTI15.Line = EXTI_LINE_15;

	if (STMPE811_Init() != STMPE811_State_Ok) {
		for (;;);
	}
	MetalDetectorInit();
//	App_startScreen();
//	App_DetectScreen();
}

void App_ScreenHandler() {
	while (1){
		exitIT = 0;
		if (CurrentScreen == STARTSCREEN) {
			TIM_stop(TIM_BUZZER);
			TIM_stop(TIM_STOPWATCH);
			TIM_reset(TIM_STOPWATCH);
			App_startScreen();
		}
		else if (CurrentScreen == DETECTSCREEN) {
			App_DetectScreen();
		}
		else if (CurrentScreen == ENDSCREEN){
			App_endScreen();
		}
	}
}

void App_startScreen() {
	LCD_Clear(0, LCD_COLOR_GREY);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font12x12);
	LCD_DisplayStr(40, 50, "Press to start", 14);
	LCD_DisplayStr(10, 300, "Nikolai Woeger", 14);
	LCD_DrawBox_Unfilled(40, 130, 160, 80, LCD_COLOR_BLACK);
	LCD_DrawBox_Filled(41, 131, 159, 79, LCD_COLOR_WHITE);
	LCD_SetFont(&Font16x24);
	LCD_DisplayStr(85, 160, "START", 5);
	LCD_DisplayStr(15, 10, "Metal Detector", 14);
	while (!exitIT);
	HAL_Delay(100);
	I2C3_Write(STMPE811_ADDRESS, STMPE811_INT_STA, 0xFF);
}
void drawBackground() {
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
	TIM_reset(TIM_STOPWATCH);
	HAL_Delay(100);
	TIM_start(TIM_STOPWATCH);
	while (1) {
		if (TIM_stopwatch_getTime() >= 45 || exitIT) {
			break;
		}
		if (pollDetector() == detected) {
			detected ^= 1;
			drawBackground();
			if (detected) {
				TIM_start(TIM_BUZZER);
				detected_in_session = 1;
			}
			else {
				TIM_stop(TIM_BUZZER);
			}
		}
		drawTime();
		HAL_Delay(50);
	}
	TIM_stop(TIM_STOPWATCH);
	if (!exitIT)
		CurrentScreen++;
	HAL_Delay(100);
	I2C3_Write(STMPE811_ADDRESS, STMPE811_INT_STA, 0xFF);
}
void drawTime() {
	LCD_DrawBox_Filled(40, 220, 170, 40, ((CurrentScreen == ENDSCREEN) ? LCD_COLOR_GREY : (detected ? LCD_COLOR_RED : LCD_COLOR_GREEN)));
	LCD_SetTextColor((CurrentScreen == ENDSCREEN) ? LCD_COLOR_BLACK : (detected ? LCD_COLOR_WHITE : LCD_COLOR_BLACK));
	LCD_SetFont(&Font12x12);
	float value = TIM_stopwatch_getTime();
	char time_s [4];
	gcvt(value, 4, time_s);
	LCD_DisplayStr(60, 220, time_s, 5);
	LCD_DisplayStr(140, 220, "sec", 3);
}
void App_endScreen() {
	LCD_Clear(0, LCD_COLOR_GREY);
	drawTime();

	if (detected_in_session) {
		LCD_DrawBox_Unfilled(15, 70, 210, 80, LCD_COLOR_BLACK);
		LCD_DrawBox_Filled(16, 71, 209, 79, LCD_COLOR_RED);
		LCD_SetFont(&Font12x12);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DisplayStr(48, 90, "Metal has been", 14);
		LCD_DisplayStr(75, 115, "Detected", 8);
	}
	else {
		LCD_DrawBox_Unfilled(15, 70, 210, 80, LCD_COLOR_BLACK);
		LCD_DrawBox_Filled(16, 71, 209, 79, LCD_COLOR_GREEN);
		LCD_SetFont(&Font12x12);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_DisplayStr(55, 90, "No Metal was", 12);
		LCD_DisplayStr(75, 115, "Detected", 8);
	}
	LCD_SetFont(&Font16x24);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_DisplayStr(60, 10, "Finished", 12);
	LCD_SetFont(&Font12x12);
	LCD_DisplayStr(40, 190, "Session Length:", 15);
	LCD_DisplayStr(40, 290, "Tap to Restart", 14);
	while (!exitIT);
	HAL_Delay(100);
	I2C3_Write(STMPE811_ADDRESS, STMPE811_INT_STA, 0xFF);
}

void App_Stopwatch_init() {
	TIM_init(TIM_STOPWATCH);
}
void App_Stopwatch_test1() {
	TIM_start(TIM_STOPWATCH);
	HAL_Delay(2000);
	TIM_stop(TIM_STOPWATCH);
	float value = TIM_stopwatch_getTime();
	char time_s [6];
	gcvt(value, 6, time_s);
	printf("%s\n", time_s);
}
void App_Buzzer_init() {
	TIM_init(TIM_BUZZER);
}
void App_Buzzer_test1() {
	TIM_start(TIM_BUZZER);
	HAL_Delay(1000);
	TIM_stop(TIM_BUZZER);
	printf("%d\n", count/2);
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

void EXTI15_10_IRQHandler() {
	NVIC_DisableIRQ(EXTI15_10_IRQn);
	exitIT = 1;
	CurrentScreen++;
	CurrentScreen%=3;
	if (CurrentScreen == ENDSCREEN) {
		lastTime = TIM_stopwatch_getTime();
	}
	STMPE811_DetermineTouchPosition(&t);
	if (!isSTMPE811_Ready()) {
		for(;;);
	}
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	HAL_EXTI_ClearPending(&EXTI15, 1);

	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void TIM2_IRQHandler() {
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
	TIM_buzzer_clearUIF();
	HAL_NVIC_ClearPendingIRQ(TIM2_IRQn);
	count++;
	TIM_buzzer_toggle();
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

}
