#ifndef __APP_SAMPLING_H
#define __APP_SAMPLING_H

#include "stm32f10x.h"

#include "Driver_ADC1.h"
#include "Driver_TIM7.h"


void App_Sampling_Start(uint16_t sampling_rate, uint16_t duration);

uint16_t App_Sampling_ReadData(void);

#endif

