/**
 * @file configure_pins.c
 * @brief Configure the pins for the project
 * 
 * This file contains the pin configuration for the project.
 */

#include "../inc/configure_pins.h"

void configure_pins(void) {
    PINSEL_CFG_Type PinCfg;

    // Configure the start button pin
    PinCfg.Portnum = PINSEL_PORT_0;
    PinCfg.Pinnum = PINSEL_PIN_10;
    PinCfg.Funcnum = PINSEL_FUNC_1;
    PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
    PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;

    PINSEL_ConfigPin(&PinCfg);

    // Configure the stop button pin
    PinCfg.Pinnum = PINSEL_PIN_11;

    PINSEL_ConfigPin(&PinCfg);
}

void configure_interrupts(void) {

    EXTI_InitTypeDef EXTI_InitStruct;

    // Configure the start button interrupt
    EXTI_InitStruct.EXTI_Line = EXTI_EINT0;
    EXTI_InitStruct.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
    EXTI_InitStruct.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;

    EXTI_Config(&EXTI_InitStruct);

    // Configure the stop button interrupt
    EXTI_InitStruct.EXTI_Line = EXTI_EINT1;

    EXTI_Config(&EXTI_InitStruct);

    // Enable the EINT0 and EINT1 interrupts
    NVIC_EnableIRQ(EINT0_IRQn);
    NVIC_EnableIRQ(EINT1_IRQn);
}