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

/**
 * Booleans 
 */
#define TRUE 1
#define FALSE 0

/**
 * Object Count Definitions
 */
#define DISTANCE       0.2  /**< Distance from ultrasonic sensor to surface in meters */
#define SPEED_OF_SOUND 343  /**< Speed of sound in air in meters per second */
#define SAFE_MARGIN    0.01 /**< Safe margin in meters */

/**
 * Timer and Match Definitions
 *
 * Definitions for the timer and match registers used in the application.
 */
#define START_TIME      5    /**< Set TRIGGER pin at 5us */
#define TRIGGER_TIME    10   /**< TRIGGER set time to start an ultrasonic sensor measurement */
#define RESET_TIME      1000 /**< Reset the timer */
#define PRESCALER_VALUE 1    /**< value for prescaler */

/**
 * Global Variables
 */
static uint32_t max_time =
    (uint32_t)(DISTANCE - SAFE_MARGIN) * 2 / SPEED_OF_SOUND * 1000000; /**< Maximum time for the echo signal */
static uint32_t echo_down_flag = FALSE;                                    /**< Flag to check if the echo signal is down */
static uint32_t echo_up_time = 0;                                      /**< Time when the echo signal is up */
static uint32_t echo_down_time = 0;                                    /**< Time when the echo signal is down */
static uint32_t object_count = 0;                                      /**< Count of objects detected */
static uint16_t detection_flag = FALSE; /**< Flag to check if an object is being detected */

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

    // CAP2.0 Configuration for rising and falling edge capture
    TIM_CAPTURECFG_Type capCfg;
    capCfg.CaptureChannel = 0;
    capCfg.FallingEdge = ENABLE;
    capCfg.IntOnCaption = ENABLE;
    capCfg.RisingEdge = ENABLE;

    // Initialize Timer2 in timer mode
    TIM_ConfigCapture(LPC_TIM2, &capCfg);

    // Enable Timer2 interrupt
    NVIC_EnableIRQ(TIMER2_IRQn);
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
        }

        detection_flag = FALSE;
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
        GPIO_SetValue(PINSEL_PORT_1, TRIGGER_PIN); // Set TRIGGER pin
    }
    if (TIM_GetIntStatus(LPC_TIM2, TIM_MR1_INT))
    {
        TIM_ClearIntPending(LPC_TIM2, TIM_MR1_INT);
        GPIO_ClearValue(PINSEL_PORT_1, TRIGGER_PIN); // Clear TRIGGER pin
    }
    if (TIM_GetIntStatus(LPC_TIM2, TIM_CR0_INT))
    {
        TIM_ClearIntPending(LPC_TIM2, TIM_CR0_INT);
        if (echo_down_flag == FALSE)
        {
            echo_up_time = TIM_GetCaptureValue(LPC_TIM2, 0);
            echo_down_flag = TRUE;
        }
        else
        {
            echo_down_time = TIM_GetCaptureValue(LPC_TIM2, 0);
            echo_down_flag = FALSE;

            check_object();
        }
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

    while (1)
    {
        __asm("nop");
    }

    return 0; // Should never reach this
}