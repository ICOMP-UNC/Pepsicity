#include "lpc17xx_dac.h"

/* DAC BIAS en 1MHz*/
#define BIAS_1M_DAC 0

/**
 * @brief Configuración inicial del DAC
 */
void config_dac();

/**
 * @brief Establece la velocidad del motor
 * @param level Nivel de velocidad del motor
 */
void set_motor_speed(uint16_t level);