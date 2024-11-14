/**
 * @file main.c
 * @brief Main file for the project
 **/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "lpc17xx_adc.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_uart.h"

/**
 * Pin Definitions
 *
 * Definitions for the pins used in the application.
 */
#define ECHO_PIN    ((uint32_t)(1 << 4)) /**< P0.4 */
#define TRIGGER_PIN ((uint32_t)(1 << 5)) /**< P0.5 */
#define LED_PIN     ((uint32_t)(1 << 6)) /**< P0.6 */

/**
 * Booleans
 */
#define TRUE  1 /**< True */
#define FALSE 0 /**< False */

/**
 * OUTPUT and INPUT Definitions
 */
#define OUTPUT 1 /**< Output */
#define INPUT  0 /**< Input */

/**
 * Object Count Definitions
 *
 * DISTANCE: The distance from the ultrasonic sensor to the surface in meters.
 * SPEED_OF_SOUND: The speed of sound in air in meters per second.
 * SAFE_MARGIN: The safe margin in meters.
 */

/**
 * Timer and Match Definitions
 *
 * Definitions for the timer and match registers used in the application.
 */
#define START_TIME      5     /**< Set TRIGGER pin at 5us */
#define TRIGGER_TIME    10    /**< TRIGGER set time to start an ultrasonic sensor measurement */
#define RESET_TIME      50000 /**< Reset the timer (an ultrasonic sensor measurement each 50ms) */
#define PRESCALER_VALUE 1     /**< value for prescaler */

/**
 * Global Variables
 */
static uint32_t max_time =
    2270; /**< Maximum time for the echo signal (DISTANCE - SAFE_MARGIN) * 2 / SPEED_OF_SOUND * 1000000;*/
static uint32_t echo_up_time = 470;     /**< Time when the echo signal is up */
static uint32_t echo_down_time = 0;     /**< Time when the echo signal is down */
static uint32_t object_count = 0;       /**< Count of objects detected */
static uint16_t detection_flag = FALSE; /**< Flag to check if an object is being detected */

/**
 * @brief configures the pins.
 *
 * This function configures the pins.
 */
void config_pins(void);

/**
 * @brief Configures the timer.
 *
 * This function sets up the timer, match and capture registers for timing operations.
 */
void config_timer(void);

/**
 * @brief Checks if an object is detected.
 *
 * This function checks if an object is detected and increments the object count.
 */
void check_object(void);

/**
 * @brief Turns on the count LED.
 *
 * This function turns on the count LED when an object is detected.
 */
void turn_on_led(void);

/**
 * @brief Turns off the count LED.
 *
 * This function turns off the count LED when an object is not detected.
 */
void turn_off_led(void);

void config_pins(void)
{
    PINSEL_CFG_Type PinCfg;

    // Configure the ECHO pin
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 4;
    PinCfg.Funcnum = PINSEL_FUNC_3;
    PinCfg.Pinmode = PINSEL_PINMODE_PULLDOWN;
    PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
    PINSEL_ConfigPin(&PinCfg);

    // Configure the TRIGGER pin
    PinCfg.Pinnum = 5;
    PinCfg.Funcnum = PINSEL_FUNC_0;
    PinCfg.Pinmode = PINSEL_PINMODE_TRISTATE;
    PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
    PINSEL_ConfigPin(&PinCfg);

    // Configure the LED pin
    PinCfg.Pinnum = 6;
    PinCfg.Funcnum = PINSEL_FUNC_0;
    PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
    PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
    PINSEL_ConfigPin(&PinCfg);

    // Set the TRIGGER pin as output
    GPIO_SetDir(PINSEL_PORT_0, TRIGGER_PIN, OUTPUT);

    // Set the LED pin as output
    GPIO_SetDir(PINSEL_PORT_0, LED_PIN, OUTPUT);

    // Set the ECHO pin as input
    GPIO_SetDir(PINSEL_PORT_0, ECHO_PIN, INPUT);
}

void config_timer(void)
{
    TIM_TIMERCFG_Type timer_cfg_struct;
    TIM_MATCHCFG_Type match_cfg_struct;

    // Configure Timer2 in microsecond mode with a prescaler
    timer_cfg_struct.PrescaleOption = TIM_PRESCALE_USVAL; // Prescaler in microseconds
    timer_cfg_struct.PrescaleValue = PRESCALER_VALUE;     // Prescaler for 100 MHz clock
    TIM_Init(LPC_TIM2, TIM_TIMER_MODE, &timer_cfg_struct);

    // Configure match channel to set the TRIGGER pin
    match_cfg_struct.MatchChannel = 0;
    match_cfg_struct.IntOnMatch = ENABLE;
    match_cfg_struct.StopOnMatch = DISABLE;
    match_cfg_struct.ResetOnMatch = DISABLE;
    match_cfg_struct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
    match_cfg_struct.MatchValue = (uint32_t)(START_TIME); // match value for the interrupt
    TIM_ConfigMatch(LPC_TIM2, &match_cfg_struct);

    // Configure match channel to clear the TRIGGER pin
    match_cfg_struct.MatchChannel = 1;
    match_cfg_struct.MatchValue = (uint32_t)(START_TIME + TRIGGER_TIME); // match value for the interrupt
    TIM_ConfigMatch(LPC_TIM2, &match_cfg_struct);

    // Configure match channel to reset the timer
    match_cfg_struct.MatchChannel = 2;
    match_cfg_struct.IntOnMatch = DISABLE;
    match_cfg_struct.ResetOnMatch = ENABLE;
    match_cfg_struct.MatchValue = (uint32_t)(RESET_TIME); // match value for the interrupt
    TIM_ConfigMatch(LPC_TIM2, &match_cfg_struct);

    // CAP2.0 Configuration for falling edge capture
    TIM_CAPTURECFG_Type capCfg;
    capCfg.CaptureChannel = 0;
    capCfg.FallingEdge = ENABLE;
    capCfg.IntOnCaption = ENABLE;
    capCfg.RisingEdge = DISABLE;

    // Initialize Timer2 in timer mode
    TIM_ConfigCapture(LPC_TIM2, &capCfg);

    // Enable Timer2 interrupt
    NVIC_EnableIRQ(TIMER2_IRQn);
}

void turn_on_led(void)
{
    GPIO_SetValue(PINSEL_PORT_0, LED_PIN);
}

void turn_off_led(void)
{
    GPIO_ClearValue(PINSEL_PORT_0, LED_PIN);
}

void check_object(void)
{
    // Calculate the time taken for the echo signal
    uint32_t echo_time = echo_down_time - echo_up_time;

    if (echo_time < max_time)
    {
        // If an object is detected, set the detection flag
        detection_flag = TRUE;
    }
    else
    {
        // If an object was being detected, increment the object count
        if (detection_flag)
        {
            object_count++;
            turn_on_led();
        }

        detection_flag = FALSE;
        turn_off_led();
    }
}

/**
 * @brief Timer2 interrupt handler to toggle LEDs.
 */
void TIMER2_IRQHandler(void)
{
    // Check and clear match interrupt for each channel

    if (TIM_GetIntStatus(LPC_TIM2, TIM_MR0_INT))
    {
        TIM_ClearIntPending(LPC_TIM2, TIM_MR0_INT);
        GPIO_SetValue(PINSEL_PORT_0, TRIGGER_PIN); // Set TRIGGER pin
    }
    if (TIM_GetIntStatus(LPC_TIM2, TIM_MR1_INT))
    {
        TIM_ClearIntPending(LPC_TIM2, TIM_MR1_INT);
        GPIO_ClearValue(PINSEL_PORT_0, TRIGGER_PIN); // Clear TRIGGER pin
    }
    if (TIM_GetIntStatus(LPC_TIM2, TIM_CR0_INT))
    {
        TIM_ClearIntPending(LPC_TIM2, TIM_CR0_INT);

        echo_down_time = TIM_GetCaptureValue(LPC_TIM2, 0); /**< Get the time when the echo signal is down */

        check_object(); // Check if an object is detected
    }
}

/**
 * @brief Main function for the project.
 *
 * This function initializes the system and starts the SysTick timer.
 */
int main(void)
{
    SystemInit(); // Initialize the system

    config_pins();

    config_timer();

    TIM_Cmd(LPC_TIM2, ENABLE);

    while (1)
    {
        __asm("nop");
    }

    return 0; // Should never reach this
}