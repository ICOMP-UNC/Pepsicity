/**
 * @file temp_module.c
 * @brief Module for the temperature sensor
 *
 * This file contains the implementation of the temperature sensor module.
 */

#include "../inc/temp_module.h"

uint16_t adc_value; /**< Last ADC conversion value */

void init_temp_module()
{
    configure_adc();
    configure_systick();
}

void configure_adc()
{
    ADC_Init(LPC_ADC, FREQ_ADC);
    ADC_BurstCmd(LPC_ADC, DISABLE);
    NVIC_SetPriority(ADC_IRQn, ADC_PRIORITY);
    NVIC_EnableIRQ(ADC_IRQn);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);
}

void start_adc()
{
    ADC_StartCmd(LPC_ADC, ADC_START_NOW);
}

void configure_systick()
{
    SYSTICK_InternalInit(SYSTICK_PERIOD);
    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE);
    NVIC_SetPriority(SysTick_IRQn, SYSTICK_PRIORITY);
}

void SysTick_Handler(void)
{
    start_adc();
    SYSTICK_ClearCounterFlag();
}

void ADC_IRQHandler(void)
{
    adc_value = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0);
    ADC_GlobalGetData(LPC_ADC); // Clear the interrupt
}
