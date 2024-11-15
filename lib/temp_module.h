#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "lpc17xx_adc.h"
#include <lpc17xx_systick.h>

// Define the period of the SysTick timer interrupt in ms
#define SYSTICK_PERIOD 10 //ms

// Define the frequency of the ADC
#define FREQ_ADC 200000

/**
 * @brief Inicializa el módulo del lector de temperatura
 */
void init_temp_module();

/**
 * @brief Configura el SysTick timer para que interrumpa cada SYSTICK_PERIOD ms
 */
void configure_systick();

/**
 * @brief Configura el ADC a 200 kHz
 */
void configure_adc();

/**
 * @brief Comienza una nueva conversión del ADC
 */
void start_adc();
