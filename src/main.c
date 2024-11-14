/**
 * @file main.c
 * @brief Main file for the project
 **/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "lpc17xx_adc.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_uart.h"

#include "../lib/motor_control.h"

void configure_pins(void)
{
    PINSEL_CFG_Type cfg_pin;
    // DAC
    cfg_pin.Portnum = PINSEL_PORT_0;
    cfg_pin.Pinnum = PINSEL_PIN_26;
    cfg_pin.Funcnum = PINSEL_FUNC_2;
    cfg_pin.Pinmode = PINSEL_PINMODE_TRISTATE;
    PINSEL_ConfigPin(&cfg_pin);

}

int main()
{
	config_dac();
	set_motor_speed(0);

    while (1)
        {
            __asm("nop");
        }
}
