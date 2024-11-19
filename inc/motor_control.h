#include "lpc17xx_dac.h"

#define BIAS_1M_DAC  0                                         /**< DAC bias at 1MHz */
#define SPEED_VAL(x) ((x - 1) / (10 - 1) * (1023 - 950) + 950) /**< Speed value */

/**
 * @brief Configures the DAC peripheral
 */
void config_dac();

/**
 * @brief Stablishes the speed of the motor
 * @param level Speed level
 */
void set_motor_speed(uint16_t level);