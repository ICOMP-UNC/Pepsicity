/**
 * @file motor_control.h
 * @brief Header file for motor control functions
 *
 * This file contains the declarations of the functions that handle the motor control
 */

#include "lpc17xx_dac.h"

#define BIAS_1M_DAC     0    /**< DAC bias at 1MHz */
#define MAX_VELOCITY    10   /**< Maximum velocity */
#define MIN_VELOCITY    1    /**< Minimum velocity */
#define MAX_SET_DAC_VAL 1023 /**< Maximum DAC value */
#define MIN_SET_DAC_VAL 950  /**< Minimum DAC value */
#define STOP_MOTOR_VAL  0    /**< Stop motor value */

#define SPEED_VAL(x)                                                                                                   \
    ((x - MIN_VELOCITY) / (MAX_VELOCITY - MIN_VELOCITY) * (MAX_SET_DAC_VAL - MIN_SET_DAC_VAL) +                        \
     MIN_SET_DAC_VAL) /**< Speed value */

/**
 * @brief Configures the DAC peripheral
 */
void config_dac();

/**
 * @brief Stablishes the speed of the motor
 * @param level Speed level
 */
void set_motor_speed(uint16_t level);
