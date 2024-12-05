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

static uint8_t detected = 0;

void ApplicationInit(void);
void App_Stopwatch_init();
void App_Stopwatch_test1();
void App_Buzzer_init();
void App_Buzzer_test1();
void App_Buzzer_beep();
void App_startScreen();
void App_DetectScreen();

#endif /* INC_APPLICATIONCODE_H_ */
