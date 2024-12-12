/*
 * ApplicationCode.h
 *
 *  Created on: Nov 27, 2024
 *      Author: nikkwoeg
 */

#ifndef INC_APPLICATIONCODE_H_
#define INC_APPLICATIONCODE_H_
#include "LCD_Driver.h"
#include "TimerCtl.h"
#include "stmpe811.h"
#include "MetalDetector.h"

#define	STARTSCREEN 0
#define DETECTSCREEN 1
#define ENDSCREEN 2



void ApplicationInit(void);

// timers
void App_Stopwatch_init();
void App_Stopwatch_test1();
void App_Buzzer_init();
void App_Buzzer_test1();
void App_Buzzer_beep();

// screen
void App_endScreen();
void App_startScreen();
void App_DetectScreen();
void App_ScreenHandler();
void drawTime(uint16_t x, uint16_t y, uint16_t bg, uint16_t fg);

#endif /* INC_APPLICATIONCODE_H_ */
