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
void drawTime();

#endif /* INC_APPLICATIONCODE_H_ */
