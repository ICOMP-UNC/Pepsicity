/**
 * @file counting_module.h
 * @brief Counting module for the conveyor belt system
 *
 * This file contains the counting module for the conveyor belt system.
 */

#ifndef COUNTING_MODULE_H
#define COUNTING_MODULE_H

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "../lib/CMSISv2p00_LPC17xx/drivers/include/lpc17xx_timer.h"
#include "configure_pins.h"

/**
 * Booleans
 */
#define TRUE  1 /**< True */
#define FALSE 0 /**< False */

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
#define MATCH_CHANNEL_0 0     /**< Match channel 0 */
#define MATCH_CHANNEL_1 1     /**< Match channel 1 */
#define MATCH_CHANNEL_2 2     /**< Match channel 2 */
#define CAPTURE_CHANNEL 0     /**< Capture channel 0 */

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

/**
 * @brief Starts the counting subsystem.
 *
 * This function starts the counting subsystem.
 */
void start_counting_subsystem(void);

#endif