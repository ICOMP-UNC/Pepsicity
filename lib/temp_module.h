#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "lpc17xx_adc.h"
#include <lpc17xx_systick.h>

// Define the period of the SysTick timer interrupt in ms
#define SYSTICK_PERIOD 10 //ms

// Define the frequency of the ADC
#define FREQ_ADC 200000

void init_temp_module();
// Configure the ADC with a frequency of FREQ_ADC
void configure_adc();
// Initialize the ADC
void start_adc();
// Configure the SysTick timer with a period of SYSTICK_PERIOD ms
void configure_systick();

void SysTick_Handler(void);
void ADC_IRQHandler(void);
