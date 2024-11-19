/**
 * @file main.c
 * @brief Main file for the project
 **/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "../inc/communication.h"
#include "../inc/configure_pins.h"
#include "../inc/counting_module.h"
#include "../inc/motor_control.h"
#include "../inc/temp_module.h"

/**
 * @brief EINT0 interrupt handler
 */
void EINT0_IRQHandler(void)
{
    // Clear the interrupt
    EXTI_ClearEXTIFlag(EXTI_EINT0);

    // start the system
    init_counting_module(); // Initialize the counting module
    config_dac();           /**< Initialize DAC */
}

/**
 * @brief EINT1 interrupt handler
 */
void EINT1_IRQHandler(void)
{
    // Clear the interrupt
    EXTI_ClearEXTIFlag(EXTI_EINT1);

    // stop the system
    stop_counting_module(); // Stop the counting module
}

/**
 * @brief main function
 */
int main()
{
    SystemInit();           // Initialize the system
    configure_pins();       // Configure the pins
    init_communication();   // Initialize the communication
    init_temp_module();     // Initialize the temperature module
    configure_interrupts(); // Configure the interrupts

    while (1)
    {
    }

    return 0; // Should never reach this
}
