/*
 * MetalDetector.c
 *
 *  Created on: Dec 9, 2024
 *      Author: nikkwoeg
 */


#include "MetalDetector.h"


void MetalDetectorInit() {
	__HAL_RCC_GPIOF_CLK_ENABLE();
	GPIO_InitTypeDef init;
	init.Mode = GPIO_MODE_INPUT;
	init.Pin = DETECTORPIN;
	init.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(DETECTORPORT, &init);
}

GPIO_PinState pollDetector() {
	return HAL_GPIO_ReadPin(DETECTORPORT, DETECTORPIN);
}
