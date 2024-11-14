#ifndef __COMMON_DEBUG_H
#define __COMMON_DEBUG_H

#include "stm32f10x.h"
#include "string.h"
#include "Driver_USART1.h"

#define DEBUG /* 调试信息的开关 */
#ifdef DEBUG
#define debug_init()    \
        Common_Debug_Init()
#define FILENAME        \
        (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define debug_printf(format, ...)   \
        printf("[%s: %d]--" format, FILENAME, __LINE__, ##__VA_ARGS__)
#define debug_printfln(format, ...) \
        printf("[%s: %d]--" format "\r\n", FILENAME, __LINE__, ##__VA_ARGS__)
#else
#define debug_init()
#define debug_printf(format, ...)
#define debug_printfln(format, ...)
#endif

#endif