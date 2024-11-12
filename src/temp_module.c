#include "../lib/temp_module.h"

uint16_t adc_value = 0;

void init_temp_module()
{
    configure_adc();
    configure_systick();
}

void configure_adc()
{
    ADC_Init(LPC_ADC, FREQ_ADC);
    ADC_BurstCmd(LPC_ADC, DISABLE);
    NVIC_SetPriority(ADC_IRQn, 1);
    NVIC_EnableIRQ(ADC_IRQn);
    ADC_ChannelCmd(LPC_ADC, 0, ENABLE);
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
    NVIC_SetPriority(SysTick_IRQn, 0);

}

void SysTick_Handler(void)
{
    start_adc();
    SYSTICK_ClearCounterFlag();
}

void ADC_IRQHandler(void)
{
    adc_value = ADC_ChannelGetData(LPC_ADC, 0);
    ADC_GlobalGetData(LPC_ADC);// Clear the interrupt
}
