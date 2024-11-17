/**
 * @file main.c
 * @brief Main file for the project
 **/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "../inc/configure_pins.h"
#include "../inc/motor_control.h"

/**
 * @brief EINT0 interrupt handler
 */
void EINT0_IRQHandler(void)
{
    // Clear the interrupt
    EXTI_ClearEXTIFlag(EXTI_EINT0);

    // start the system
    config_dac(); /**< Initialize DAC */
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
 * @brief main function
 */
int main()
{
    SystemInit(); // Initialize the system

    configure_pins(); // Configure the pins

    configure_interrupts(); // Configure the interrupts

    while (1)
    {
    }

    return 0; // Should never reach this
}
