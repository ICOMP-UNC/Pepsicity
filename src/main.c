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

int main()
{
	config_dac();

    configure_pins(); // Configure the pins

    configure_interrupts(); // Configure the interrupts
    
    set_motor_speed(0);

    while (1)
        {
            __asm("nop");
        }
}
