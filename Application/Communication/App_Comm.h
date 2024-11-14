#ifndef __APP_COMM_H__
#define __APP_COMM_H__

#include "stm32f10x.h"
#include "string.h"
#include "stdlib.h"
#include "Driver_USART1.h"
#include "Common_Debug.h"

void App_Comm_Start(void);

void App_Comm_CommandProcess(uint16_t *sampling_rate, uint16_t *duration);

void App_Comm_SendDataToUpper(uint16_t data);

void USART1_RxIdleIRQCallBack(uint8_t *buff, uint8_t count);

#endif

