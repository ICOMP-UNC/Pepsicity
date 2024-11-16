#include "lpc17xx_dac.h"

#define BIAS_1M_DAC 0   /**< DAC bias en 1MHz */

/**
 * @brief Configures the DAC peripheral
 */
void config_dac();

/**
 * @brief Stablishes the speed of the motor
 * @param level Speed level
 */
void set_motor_speed(uint16_t level);