#ifndef __DRIVER_USART1_H
#define __DRIVER_USART1_H

#include "stm32f10x.h"
#include "stdio.h"


void Driver_USART1_Init(void);

void Driver_USART1_Start(void);

void Driver_USART1_Stop(void);

void Driver_USART1_SendString(uint8_t *str, uint16_t len);

void Driver_USART1_SendChar(uint8_t ch);

extern void USART1_RxIdleIRQCallBack(uint8_t *buff, uint8_t count);

#endif

