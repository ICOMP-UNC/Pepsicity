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
#include "lpc17xx_uart.h"
#include "lpc17xx_gpdma.h"

#define TRANSMITTER_PIN ((uint32_t)(1<<10))    // P0.10 connected to transmitter pin
#define RECEIVER_PIN   ((uint32_t)(1<<11))    // P0.11 connected to receiver pin

#define INPUT 0
#define OUTPUT 1

static char mode = 'A';

/* UART parameters */
#define MODO_A "MODO A"
#define MODO_B "MODO B"
#define MODO_C "MODO C"
#define speed "="

static uint32_t object_count = 0;       /**< Count of objects detected */
static uint32_t limit_flag = 0;         /**< Limit flag */
static uint32_t get_count_flag = 0;          /**< Get count flag */
static uint16_t temperature = 0;        /**< Temperature value */
static uint8_t speed_level = 0;         /**< Speed level */


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
        if(get_count_flag == 1)
        {
            send_counter();
        }
    }
    else if(mode == 'C')
    {
        if(object_count == limit_flag) 
        {
            send_counter();
            object_count = 0;
            stop();
        }
    }
}

void send_counter(void)
{
    // Send Counter
    send_data_dma_uart(object_count, sizeof(object_count));
}

void send_temperature()
{
    // Send Temperature
    send_data_dma_uart(temperature, sizeof(temperature));
}

void set_velocity(void)
{
    // Set velocity
}
