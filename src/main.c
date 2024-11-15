/**
 * @file main.c
 * @brief Main file for the project
 **/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "lpc17xx_adc.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_uart.h"

#include "../inc/configure_pins.h"

/**
 * @brief EINT0 interrupt handler
 */
void EINT0_IRQHandler(void)
{
    // Clear the interrupt
    EXTI_ClearEXTIFlag(EXTI_EINT0);

    // start the system
}

/**
 * @brief EINT1 interrupt handler
 */
void EINT1_IRQHandler(void)
{
    // Clear the interrupt
    EXTI_ClearEXTIFlag(EXTI_EINT1);

    // stop the system
}

/**
 * @brief Main function for the project.
 *
 * This function initializes the system and starts the SysTick timer.
 */
int main(void)
{
    SystemInit(); // Initialize the system

    configure_pins(); // Configure the pins

    configure_interrupts(); // Configure the interrupts

    while (1)
    {
        __asm("nop");
    }

    return 0; // Should never reach this
}