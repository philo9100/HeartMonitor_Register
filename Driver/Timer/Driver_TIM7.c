#include "Driver_TIM7.h"

void Driver_TIM7_Init()
{
    /* 1.开启时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;

    /* 2.设置定时器的计数器的预分频值，分频7200次，即计数频率为 10000次/秒 */
    TIM7->PSC = 7200 - 1;

    /* 3.设置重装载寄存器的值，计数器计数10次，即1ms产生一次溢出 */
    TIM7->ARR = 10 - 1;

    /* 4.设置UG位手动产生一个更新事件，触发一个更新中断，将预装载寄存器中的值写入影子寄存器 */
    TIM7->EGR |= TIM_EGR_UG;

    /* 5.清除中断标志位 */
    TIM7->SR &= ~TIM_SR_UIF;

    /* 6.开启更新中断 */
    TIM7->DIER |= TIM_DIER_UIE;

    /* 7.配置NVIC */
    NVIC_SetPriority(TIM7_IRQn, 2);
    NVIC_EnableIRQ(TIM7_IRQn);
}

void Driver_TIM7_Start()
{
    /* 使能定时器 */
    TIM7->CR1 |= TIM_CR1_CEN;
}

void Driver_TIM7_Stop()
{
    /* 失能计数器 */
    TIM7->CR1 &= ~TIM_CR1_CEN;
}


/**
 * @brief 定时器更新中断的回调接口函数，应用层实现这个接口函数
 * @param msCount 
 * @return 
 */
__weak void TIM7_UpdateIRQCallBack(uint32_t msCount){}


uint32_t msCount = 0;
/**
 * @brief 更新中断服务函数
 */
void TIM7_IRQHandler(void)
{
    /* 
    每一毫秒产生一次更新中断，msCount自增，将msCount传递给应用层实现的中断回调函数
    应用层根据不同的 msCount 值进行业务实现 
    */

    /* 清除中断标志位 */
    TIM7->SR &= ~TIM_SR_UIF;

    msCount++;
    /* 调用应用层实现的函数 */
    TIM7_UpdateIRQCallBack(msCount);

}

