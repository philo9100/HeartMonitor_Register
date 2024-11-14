#include "Driver_USART1.h"

/**
 * @brief 初始化串口1
 * @param
 */
void Driver_USART1_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    /* 复用推挽 PA9=TX MODE=11 CNF=10 */
    GPIOA->CRH |= (GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1 & ~GPIO_CRH_CNF9_0);
    /* 浮空输入 PA10=RX MODE=00 CNF=01 */
    GPIOA->CRH |= (~GPIO_CRH_CNF10_1 & GPIO_CRH_CNF10_0 & ~GPIO_CRH_MODE10);

    /* 波特率 115200 */
    USART1->BRR = 0x271;

    /* 配置一个字的长度 8位 */
    USART1->CR1 &= ~USART_CR1_M;
    /* 配置不需要校验位 */
    USART1->CR1 &= ~USART_CR1_PCE;
    /* 配置停止位的长度 */
    USART1->CR2 &= ~USART_CR2_STOP;

    /* 使能发送 */
    USART1->CR1 |= USART_CR1_TE;

    /* 使能接收 */
    USART1->CR1 |= USART_CR1_RE;

    /* 使能接收中断和空闲中断 */
    USART1->CR1 |= (USART_CR1_RXNEIE | USART_CR1_IDLEIE);

    /* NVIC 配置 */
    NVIC_SetPriorityGrouping(3); /* 全部配置为抢占优先级 */
    NVIC_SetPriority(USART1_IRQn, 3);
    NVIC_EnableIRQ(USART1_IRQn);
}

/**
 * @brief 启动串口1
 * @param
 */
void Driver_USART1_Start(void)
{
    USART1->CR1 |= USART_CR1_UE;
}

/**
 * @brief 停止串口1
 * @param
 */
void Driver_USART1_Stop(void)
{
    USART1->CR1 &= ~USART_CR1_UE;
}

/**
 * @brief 发送一个字符
 * @param ch 要发送的字符
 */
void Driver_USART1_SendChar(uint8_t ch)
{
    /* 等待发送寄存器为空 */
    while ((USART1->SR & USART_SR_TXE) == 0)
        ;

    /* 发送一个字节 */
    USART1->DR = ch;
}

/**
 * @brief 发送一个字符串
 * @param str 要发送的字符串指针
 * @param len 要发送的字符串长度
 */
void Driver_USART1_SendString(uint8_t *str, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        Driver_USART1_SendChar(str[i]);
    }
}

/**
 * @brief 重写 printf 函数的 fputc
 * @param ch
 * @param file
 * @return
 */
int fputc(int ch, FILE *file)
{
    Driver_USART1_SendChar((uint8_t)ch);
    return ch;
}

/**
 * @brief 串口空闲中断的回调接口函数，应用层实现这个接口函数
 * @param buff
 * @param count
 * @return
 */
__weak void USART1_RxIdleIRQCallBack(uint8_t *buff, uint8_t count) {}

uint8_t buff[100] = {0};
uint8_t count = 0;
/**
 * @brief 串口1的中断服务函数
 */
void USART1_IRQHandler(void)
{
    /* 驱动层，应该把数据接收存储到对应的缓冲区，然后再去调用弱函数，把缓冲区传递给弱函数 */
    if (USART1->SR & USART_SR_RXNE)
    {
        buff[count] = (USART1->DR & 0xff);
        count++;
    }
    else if (USART1->SR & USART_SR_IDLE)
    {
        /* 清除空闲中断标志位*/
        USART1->SR;
        USART1->DR;

        /* 调用弱函数，把缓冲区传递给弱函数，这个函数在应用层实现 */
        USART1_RxIdleIRQCallBack(buff, count);
        count = 0;
    }
}
