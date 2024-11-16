#include "../inc/motor_control.h"

void config_dac()
{
    DAC_Init(LPC_DAC);                 /**< Initialize DAC */
    DAC_SetBias(LPC_DAC, BIAS_1M_DAC); /**< Set bias */
}

void set_motor_speed(uint16_t level)
{
    const uint32_t DAC_min = 0;    /**< Minimum speed */
    const uint32_t DAC_med = 341;  /**< Medium speed */
    const uint32_t DAC_high = 682; /**< High speed */
    const uint32_t DAC_max = 1023; /**< Maximum speed */

    uint32_t DAC_value = 0; /**< DAC value */

    // Set DAC value according to the level
    switch (level)
    {
        case 0: DAC_value = DAC_min; break;
        case 1: DAC_value = DAC_med; break;
        case 2: DAC_value = DAC_high; break;
        case 3: DAC_value = DAC_max; break;
    }

    DAC_UpdateValue(LPC_DAC, DAC_value); /**< Update DAC value */
}
