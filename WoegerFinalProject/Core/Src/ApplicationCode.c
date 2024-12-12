/*
 * ApplicationCode.c
 *
 *  Created on: Nov 27, 2024
 *      Author: nikkwoeg
 */


#include "ApplicationCode.h"

static uint8_t detected_history [3];
static float times_history [3];
static int index;

static EXTI_HandleTypeDef EXTI15;
static uint8_t detected;
static uint8_t detected_in_session;
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
	detected_in_session = 0;
	detected = 0;
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
void move_arrays() {
	for (int i = 2; i > 0; i--) {
		detected_history[i] = detected_history[i-1];
		times_history[i] = times_history[i-1];
	}
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
		drawTime(60, 280, 0, 0);
		HAL_Delay(25);
	}
	TIM_stop(TIM_STOPWATCH);
	TIM_stop(TIM_BUZZER);
	move_arrays();
	detected_history[0] = detected_in_session;
	times_history[0] = TIM_stopwatch_getTime();
	if (index != 3) {
		index++;
	}
	if (!exitIT)
		CurrentScreen++;
	HAL_Delay(100);
	I2C3_Write(STMPE811_ADDRESS, STMPE811_INT_STA, 0xFF);
}
void drawTime(uint16_t x, uint16_t y, uint16_t bg, uint16_t fg) {
	LCD_DrawBox_Filled(x, y, 100, 12, ((CurrentScreen == ENDSCREEN) ? bg : (detected ? LCD_COLOR_RED : LCD_COLOR_GREEN)));
	LCD_SetTextColor((CurrentScreen == ENDSCREEN) ? fg : (detected ? LCD_COLOR_WHITE : LCD_COLOR_BLACK));
	LCD_SetFont(&Font12x12);
	float value = TIM_stopwatch_getTime();
	char time_s [4];
	gcvt(value, 4, time_s);
	LCD_DisplayStr(x, y, time_s, 5);
	LCD_DisplayStr(x + 80, y, "sec", 3);
}
void App_endScreen() {
	LCD_Clear(0, LCD_COLOR_GREY);

	if (detected_in_session) {
		LCD_DrawBox_Unfilled(15, 50, 210, 80, LCD_COLOR_BLACK);
		LCD_DrawBox_Filled(16, 51, 209, 79, LCD_COLOR_RED);
		LCD_SetFont(&Font12x12);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DisplayStr(48, 55, "Metal has been", 14);
		LCD_DisplayStr(75, 70, "Detected", 8);
		LCD_DisplayStr(40, 95, "Session Length:", 15);
		drawTime(65, 110, LCD_COLOR_RED, LCD_COLOR_WHITE);
	}
	else {
		LCD_DrawBox_Unfilled(15, 50, 210, 80, LCD_COLOR_BLACK);
		LCD_DrawBox_Filled(16, 51, 209, 79, LCD_COLOR_GREEN);
		LCD_SetFont(&Font12x12);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_DisplayStr(55, 55, "No Metal was", 12);
		LCD_DisplayStr(75, 70, "Detected", 8);
		LCD_DisplayStr(40, 95, "Session Length:", 15);
		drawTime(65, 110, LCD_COLOR_GREEN, LCD_COLOR_BLACK);
	}
	// log code
	LCD_DrawBox_Unfilled(15, 130, 210, 160, LCD_COLOR_BLACK);
	LCD_DrawBox_Filled(16, 131, 209, 159, LCD_COLOR_WHITE);
	LCD_SetTextColor(LCD_COLOR_BLACK);


	LCD_SetFont(&Font16x24);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_DisplayStr(60, 10, "Finished", 12);
	LCD_DisplayStr(85, 140, "Logs", 4);
	LCD_SetFont(&Font12x12);
	LCD_DisplayStr(48, 170, "Time", 4);
	LCD_DisplayStr(145, 170, "Metal", 5);
	LCD_Draw_Horizontal_Line(20, 185, 200, LCD_COLOR_BLACK);
	LCD_Draw_Vertical_Line(120, 185, 95, LCD_COLOR_BLACK);
	for (int i = 0; i < index; i++) {
		char time_s [4];
		gcvt(times_history[i], 4, time_s);
		uint16_t y_pos = 195 + (i*30);
		LCD_DisplayStr(30, y_pos, time_s, 4);
		LCD_DrawBox_Filled(121, y_pos, 99, 20, (detected_history[i] ? LCD_COLOR_RED : LCD_COLOR_GREEN));
	}
	LCD_SetFont(&Font12x12);
	LCD_DisplayStr(40, 300, "Tap to Restart", 14);
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
