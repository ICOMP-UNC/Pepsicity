/**
 * @file motor_control.c
 * @brief File for motor control functions
 *
 * This file contains the implementation of the functions that handle the motor control
 */
#include "../inc/motor_control.h"

void config_dac()
{
    DAC_Init(LPC_DAC);                 /**< Initialize DAC */
    DAC_SetBias(LPC_DAC, BIAS_1M_DAC); /**< Set bias */
}

void set_motor_speed(uint16_t level)
{
    if (level == STOP_MOTOR_VAL)
    {
        DAC_UpdateValue(LPC_DAC, STOP_MOTOR_VAL); /**< Set DAC value to 0 if level is 0 */
    }
    else
    {
        uint16_t DAC_value = SPEED_VAL(level); /**< Calculate DAC value based on speed level */

        DAC_UpdateValue(LPC_DAC, DAC_value); /**< Update DAC with calculated value */
    }
}
