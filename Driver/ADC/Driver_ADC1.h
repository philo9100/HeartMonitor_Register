#ifndef __DRIVER_ADC1_H
#define __DRIVER_ADC1_H

#include "stm32f10x.h"

void Driver_ADC_Init(void);

void Driver_ADC_Start(void);

void Driver_ADC_Stop(void);

uint16_t Driver_ADC_ReadVal(void);

#endif

