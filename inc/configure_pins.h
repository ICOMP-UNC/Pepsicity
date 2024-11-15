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
 * Define the pins for the project
 */
#define START_BUTTON_PIN ((uint32_t)(1 << 10)) /**< Start button pin */
#define STOP_BUTTON_PIN  ((uint32_t)(1 << 11)) /**< Stop button pin */

/**
 * @brief Configure the pins for the project
 */
void configure_pins(void);

/**
 * @brief Configure external interrupts
 */
void configure_interrupts(void);

#endif
