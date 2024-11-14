#include "App_Comm.h"

/**
 * 接收上位机传递过来的指令。一共有两种指令
 * 1、"s1000\n" s表示下发的是采样率 1000表示采样率（每分钟采样1000次）
 * 2、"c60\n"   c表示下发的采样时长 60表示采样60s
 *
 * 接收到这个命令后开启ADC和定时器开始采样，用 \n 表示命令结束
 *
 */

/**
 * @brief 启动通讯模块
 * @param
 */
void App_Comm_Start(void)
{
    /* 1、初始化串口 */
    Driver_USART1_Init();

    /* 2、开启串口 */
    Driver_USART1_Start();
}

/* 标志位：表示所有参数全部接受完毕 */
uint8_t g_RxCplt = 0;
/* 用来存储采样率 */
uint8_t g_RcvRate[10] = {0};
/* 用来存储采样时长 */
uint8_t g_RcvDuration[10] = {0};

void USART1_RxIdleIRQCallBack(uint8_t *buff, uint8_t count)
{
    if (buff[0] == 's') /* 下发的是采样率 */
    {
        strncpy(g_RcvRate, buff + 1, count - 1);
    }
    else if (buff[0] == 'c') /* 下发的是采样时长 */
    {
        strncpy(g_RcvDuration, buff + 1, count - 1);
    }
    g_RxCplt = 1;
}

/**
 * @brief 处理用户下发的命令
 * @param sampling_rate 写出采样率
 * @param duration 写出采样时长
 */
void App_Comm_CommandProcess(uint16_t *sampling_rate, uint16_t *duration)
{
    /* 1、等待用户下发指令 */
    while (g_RxCplt == 0)
        ;

    /* 2、处理指令 */
    // debug_printfln("采样率=%s", g_RcvRate);
    // debug_printfln("采样时长=%s", g_RcvDuration);
    /* 2.1 把字符串采样率 "1000" 转换成数字 1000 */
    *sampling_rate = atoi(g_RcvRate);
    /* 2.2 把字符串采样时间 "60" 转换成数字 60 */
    *duration = atoi(g_RcvDuration);
}


/*
    给上位机发送的数据 "3049"
    12   => 0012\n
    123  => 0123\n
    1234 => 1234\n
*/
uint8_t commbuff[6] = {0};

/**
 * @brief 将采集到的数据发送给上位机
 * @param data 
 */
void App_Comm_SendDataToUpper(uint16_t heartData)
{
    sprintf((char*)commbuff, "%04d\n", heartData);
    Driver_USART1_SendString(commbuff, strlen(commbuff));
}




