/**
 * @file main.c
 * @brief Main file for the project
 **/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "../lib/communication.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_gpdma.h"


void configure_pins(void)
{
    PINSEL_CFG_Type cfg_pin;
    // UART2_TXD
    cfg_pin.Portnum = PINSEL_PORT_0;
    cfg_pin.Pinnum = PINSEL_PIN_10;
    cfg_pin.Funcnum = PINSEL_FUNC_1;
    cfg_pin.Pinmode = PINSEL_PINMODE_PULLUP;
    cfg_pin.OpenDrain = PINSEL_PINMODE_NORMAL;
    PINSEL_ConfigPin(&cfg_pin);
    // UART2_RXD
    cfg_pin.Pinnum = PINSEL_PIN_11;
    cfg_pin.Funcnum = PINSEL_FUNC_1;
    cfg_pin.Pinmode = PINSEL_PINMODE_TRISTATE;
    PINSEL_ConfigPin(&cfg_pin);
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
    init_communication(); // Initialize the communication

    while (1)
    {
        __asm("nop");
    }

    return 0; // Should never reach this
}
