/**
 * @file counting_module.c
 * @brief Counting module for the conveyor belt system
 *
 * This file contains the counting module for the conveyor belt system.
 */

#include "../inc/counting_module.h"

void config_timer(void)
{
    TIM_TIMERCFG_Type timer_cfg_struct;
    TIM_MATCHCFG_Type match_cfg_struct;

    // Configure Timer2 in microsecond mode with a prescaler
    timer_cfg_struct.PrescaleOption = TIM_PRESCALE_USVAL; /**< Prescaler in microseconds */
    timer_cfg_struct.PrescaleValue = PRESCALER_VALUE;     /**< Prescaler for 100 MHz clock */
    TIM_Init(LPC_TIM2, TIM_TIMER_MODE, &timer_cfg_struct);

    // Configure match channel to set the TRIGGER pin
    match_cfg_struct.MatchChannel = MATCH_CHANNEL_0;            /**< Match channel 0 */
    match_cfg_struct.IntOnMatch = ENABLE;                       /**< Enable interrupt on match */
    match_cfg_struct.StopOnMatch = DISABLE;                     /**< Do not stop on match */
    match_cfg_struct.ResetOnMatch = DISABLE;                    /**< Do not reset on match */
    match_cfg_struct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING; /**< Do nothing on match */
    match_cfg_struct.MatchValue = (uint32_t)(START_TIME);       /**< match value for the interrupt */
    TIM_ConfigMatch(LPC_TIM2, &match_cfg_struct);

    // Configure match channel to clear the TRIGGER pin
    match_cfg_struct.MatchChannel = MATCH_CHANNEL_1;                     /**< Match channel 1 */
    match_cfg_struct.MatchValue = (uint32_t)(START_TIME + TRIGGER_TIME); /**< match value for the interrupt */
    TIM_ConfigMatch(LPC_TIM2, &match_cfg_struct);

    // Configure match channel to reset the timer
    match_cfg_struct.MatchChannel = MATCH_CHANNEL_2;      /**< Match channel 2 */
    match_cfg_struct.IntOnMatch = DISABLE;                /**< Disable interrupt on match */
    match_cfg_struct.ResetOnMatch = ENABLE;               /**< Reset on match */
    match_cfg_struct.MatchValue = (uint32_t)(RESET_TIME); /**<  match value for the interrupt */
    TIM_ConfigMatch(LPC_TIM2, &match_cfg_struct);

    // CAP2.0 Configuration for falling edge capture
    TIM_CAPTURECFG_Type capCfg;
    capCfg.CaptureChannel = CAPTURE_CHANNEL; /**< Capture channel 0 */
    capCfg.FallingEdge = ENABLE;             /**< Capture on falling edge */
    capCfg.IntOnCaption = ENABLE;            /**< Enable interrupt on capture */
    capCfg.RisingEdge = DISABLE;             /**< Do not capture on rising edge */

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

            // check operation mode
            // if (operation_mode == mode a) -> send object count through UART
            // if (operation_mode == mode c) -> send object count when it reaches a certain value

            turn_on_led();
        }

        detection_flag = FALSE;
        turn_off_led();
    }
}

void init_counting_module(void)
{
    config_timer();            // Configure the timer
    TIM_Cmd(LPC_TIM2, ENABLE); // Start Timer2
}

void stop_counting_module(void)
{
    TIM_Cmd(LPC_TIM2, DISABLE); // Stop Timer2
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

        echo_down_time = TIM_GetCaptureValue(LPC_TIM2, MATCH_CHANNEL_0); // Get the time when the echo signal is down

        check_object(); // Check if an object is detected
    }
}