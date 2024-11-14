#include "App_Sampling.h"

static uint32_t g_sampling_rate;
static uint32_t g_duration;

/**
 * @brief
 * @param sampling_rate 采样率
 * @param duration 采样时长
 */
void App_Sampling_Start(uint16_t sampling_rate, uint16_t duration)
{
    /* 存储采样率和时长 */
    g_sampling_rate = sampling_rate / (60 * 1000); /* 采样率为一分钟采样的次数，转换为1ms采样的次数 */
    g_duration = duration * 1000;                  /* 采样时长单位为s，转换为ms */

    /* 1.初始化ADC1 */
    Driver_ADC_Init();

    /* 2.启动ADC1 */
    Driver_ADC_Start();

    /* 3.初始化定时器 */
    Driver_TIM7_Init();

    /* 4.启动定时器 */
    Driver_TIM7_Init();
}


__IO uint8_t g_read_ADC_val = 0;
/**
 * @brief 应用层实现定时器中断的回调函数
 * @param msCount
 */
void TIM7_UpdateIRQCallBack(uint32_t msCount)
{
    if (msCount >= g_duration)
    {
        /* 关闭定时器 */
        Driver_TIM7_Stop();

        /* 停止ADC */
        Driver_ADC_Stop();
    }

    /* (1 / g_sampling_rate) 为采样一次所需要的ms值，msCount 每到这个ms值就读取一次ADC的值 */
    if (msCount % (1 / g_sampling_rate) == 0)
    {
        /* 读取一次ADC的值 */
        g_read_ADC_val = 1;
    }
}

/**
 * @brief
 * @param
 * @return
 */
uint16_t App_Sampling_ReadData(void)
{
    /*  */
    while (g_read_ADC_val == 0)
        ;
    g_read_ADC_val = 0;

    return Driver_ADC_ReadVal();
}
