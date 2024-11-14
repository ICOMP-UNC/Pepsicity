#include "../lib/motor_control.h"

void config_dac()
{
    DAC_Init(LPC_DAC);
    DAC_SetBias(LPC_DAC, BIAS_1M_DAC);
}

void set_motor_speed(uint16_t level)
{
    const uint32_t DAC_min = 0;    // Velocidad mínima
    const uint32_t DAC_med = 341;  // Velocidad media
    const uint32_t DAC_high = 682; // Velocidad alta
    const uint32_t DAC_max = 1023; // Velocidad máxima

    uint32_t DAC_value = 0;
    switch (level)
    {
        case 0: DAC_value = DAC_min; break;
        case 1: DAC_value = DAC_med; break;
        case 2: DAC_value = DAC_high; break;
        case 3: DAC_value = DAC_max; break;
    }
    DAC_UpdateValue(LPC_DAC, DAC_value);
}
