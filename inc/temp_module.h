/**
 * @file temp_module.h
 * @brief Module for the temperature sensor
 *
 * This file contains the declarations for the temperature sensor module.
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "../lib/CMSISv2p00_LPC17xx/drivers/include/lpc17xx_adc.h"
#include "../lib/CMSISv2p00_LPC17xx/drivers/include/lpc17xx_systick.h"

#define SYSTICK_PERIOD     10     /**< Define the period of the SysTick timer interrupt in ms */
#define FREQ_ADC           200000 /**< Define the frequency of the ADC */
#define MAX_VOLTAGE        3.3    /**< Define the maximum voltage of the ADC */
#define MAX_ADC_VALUE      4095   /**< Define the maximum value of the ADC */
#define VOLTAGE_PER_DEGREE 0.01   /**< Define the voltage per degree */
#define ADC_PRIORITY       1      /**< Define the priority of the ADC interrupt */
#define SYSTICK_PRIORITY   0      /**< Define the priority of the SysTick interrupt */

#define GET_TEMP(x) (x * MAX_VOLTAGE / (MAX_ADC_VALUE * VOLTAGE_PER_DEGREE)) /**< Transformation of ADC value to °C */

extern uint16_t adc_value; /**< Last ADC conversion value */

/**
 * @brief Initializes the temperature sensor module
 */
void init_temp_module();

/**
 * @brief Configures the SysTick timer to interrupt every SYSTICK_PERIOD ms
 */
void configure_systick();

/**
 * @brief Configures the ADC to 200 kHz
 */
void configure_adc();

/**
 * @brief Starts a new ADC conversion
 */
void start_adc();

/**
 * @brief Handler for the SysTick timer interrupt
 */
void SysTick_Handler(void);

/**
 * @brief Handler for the ADC interrupt
 */
void ADC_IRQHandler(void);
