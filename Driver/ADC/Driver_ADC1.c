#include "Driver_ADC1.h"

void Driver_ADC_Init(void)
{
    /* 开启时钟 */
    RCC->APB2ENR |= (RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPCEN);

    /* 配置PC2的GPIO工作模式：模拟输入 CNF=00 MODE=00 */
    GPIOC->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);

    /* 配置ADC模块 */
    /* 禁用扫描模式 */
    ADC1->CR1 &= ~(ADC_CR1_SCAN);

    /* 启用连续转换 */
    ADC1->CR2 |= ADC_CR2_CONT;

    /* 数据对齐方式：右对齐 */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;

    /* 禁用规则通道外部触发 */
    ADC1->CR2 &= ~ADC_CR2_EXTTRIG;

    /* 使用软件触发 */
    ADC1->CR2 |= ADC_CR2_EXTSEL;

    /* 规则组有几个通道：1 */
    ADC1->SQR1 &= ~ADC_SQR1_L;

    /* 配置规则通道组的通道：使用12通道 */
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= 12 << 0;

    /* 设置采样周期 010=13.5周期 */
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP12;
    ADC1->SMPR1 |= ADC_SMPR1_SMP12_1;
}


void Driver_ADC_Start(void)
{
    /* 1.上电 */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* 2.校准 */
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL) /* 等于0校准完成 */
        ;

    /* 3.使能，开始转换 */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* 4.规则通道启动转换*/
    ADC1->CR2 |= ADC_CR2_SWSTART;

    /* 5.等待首次转化完成 */
    while ((ADC1->SR & ADC_SR_EOC) == 0)
        ;
}

/**
 * @brief 
 * @param  
 */
void Driver_ADC_Stop(void)
{
    ADC1->CR2 &= ~ADC_CR2_ADON;
}

/**
 * @brief 
 * @return 
 */
uint16_t Driver_ADC_ReadVal(void)
{
    return ADC1->DR;
}

