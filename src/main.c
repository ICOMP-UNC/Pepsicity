/**
 * @file main.c
 * @brief Main file for the project
 **/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "../lib/comunication.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_timer.h"

#define TRANSMITTER_PIN ((uint32_t)(1<<10))    // P0.10 connected to transmitter pin
#define RECEIVER_PIN   ((uint32_t)(1<<11))    // P0.11 connected to receiver pin

#define INPUT 0
#define OUTPUT 1

static char mode = 'A';
static limit_flag = 0;

/* UART parameters */
#define MODO_A "MODO A"
#define MODO_B "MODO B"
#define MODO_C "MODO C"
#define speed "="

/* DMA parameters */
GPDMA_LLI_Type LLI_TX;

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

    GPIO_SetDir(PINSEL_PORT_0, TRANSMITTER_PIN, OUTPUT);

    GPIO_SetDir(PINSEL_PORT_0, RECEIVER_PIN, INPUT);
}

void configure_uart(void)
{
    //
}

void configure_gpdma(void)
{
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
    init_comunication(); // Initialize the communication

    while (1)
    {
        __asm("nop");
    }

    return 0; // Should never reach this
}

void start(void)
{
    // Start conveyor belt
}

void stop(void)
{
    // Stop conveyor belt
}

void check_mode(void)
{
    if(mode == 'A')
    {
        send_counter();
    }
    else if(mode == 'B')
    {
        send_counter();
    }
    else if(mode == 'C')
    {
        send_counter();
    }
}

void send_counter(void)
{
    
}
