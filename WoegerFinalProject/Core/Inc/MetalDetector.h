/*
 * MetalDetector.h
 *
 *  Created on: Dec 9, 2024
 *      Author: nikkwoeg
 */

#ifndef INC_METALDETECTOR_H_
#define INC_METALDETECTOR_H_
#include "stm32f4xx_hal.h"

#define DETECTORPORT	GPIOF
#define DETECTORPIN		GPIO_PIN_4



void MetalDetectorInit();

uint8_t pollDetector();

#endif /* INC_METALDETECTOR_H_ */
