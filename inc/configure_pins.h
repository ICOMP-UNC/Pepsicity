/**
 * @file configure_pins.h
 * @brief Configure the pins for the project
 *
 * This file contains the pin configuration for the project.
 * If you want to change the pin configuration, you can do it
 * here.
 *
 */

#ifndef CONFIGURE_PINS_H
#define CONFIGURE_PINS_H

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "../lib/CMSISv2p00_LPC17xx/drivers/include/lpc17xx_exti.h"
#include "../lib/CMSISv2p00_LPC17xx/drivers/include/lpc17xx_gpio.h"
#include "../lib/CMSISv2p00_LPC17xx/drivers/include/lpc17xx_pinsel.h"

/**
 * OUTPUT and INPUT Definitions
 */
#define OUTPUT 1 /**< Output */
#define INPUT  0 /**< Input */

/**
 * Define the pins for the project
 */
#define START_BUTTON_PIN ((uint32_t)(1 << 10)) /**< 2.10 */
#define STOP_BUTTON_PIN  ((uint32_t)(1 << 11)) /**< 2.11 */
#define ECHO_PIN         ((uint32_t)(1 << 4))  /**< P0.4 */
#define TRIGGER_PIN      ((uint32_t)(1 << 5))  /**< P0.5 */
#define LED_PIN          ((uint32_t)(1 << 6))  /**< P0.6 */

/**
 * @brief Configure the pins for the project
 */
void configure_pins(void);

/**
 * @brief Configure external interrupts
 */
void configure_interrupts(void);

#endif
