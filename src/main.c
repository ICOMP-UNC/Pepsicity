/**
 * @file main.c
 * @brief Main file for the project
 **/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "lpc17xx_adc.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_uart.h"

#define BOTON_PIN (1 << 10)  // Ejemplo de botón en P0.10
#define DEBOUNCE_DELAY 30000 // Retraso en ciclos para el debounce (~30ms)

volatile uint8_t nivel_velocidad = 0;       // Nivel de velocidad actual
volatile uint8_t debounce_activo = 0;       // Indicador de debounce activo

int main() {
    DAC_Init();
    EINT_Config();
    TIMER0_Init();

    set_motor_speed(nivel_velocidad); // Inicializa la velocidad al nivel 0

    while(1) {
    }
}

void DAC_Init() {
    PINSEL_CFG_Type PinCfg;
    PinCfg.Funcnum = 2;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = 26;
    PinCfg.Portnum = 0;
    PINSEL_ConfigPin(&PinCfg);

    DAC_CONVERTER_CFG_Type dacStruct;
    dacStruct.CNT_ENA = SET;
    dacStruct.DBLBUF_ENA = RESET;
    dacStruct.DMA_ENA = RESET;

    DAC_Init(LPC_DAC);
    DAC_SetBias(LPC_DAC, 0);
    DAC_ConfigDAConverterControl(LPC_DAC, &dacStruct);
}

/*
 * Se interrumpe por EXINT0 para cambiar la velocidad del DC Motor en 4 niveles
 */
void EINT_Config(){
	PINSEL_CFG_Type pexti0;
	pexti0.Portnum = 2;
	pexti0.Pinnum = 10;
	pexti0.Funcnum = 1;
	pexti0.Pinmode = PINSEL_PINMODE_PULLUP;
	PINSEL_ConfigPin(&pexti0);

	EXTI_InitTypeDef exti0;
	exti0.EXTI_Line = EXTI_EINT0;
	exti0.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
	exti0.EXTI_polarity = EXTI_POLARITY_HIGH_ACTIVE_OR_RISING_EDGE;

	EXTI_Init();
	EXTI_Config(&exti0);
	EXTI_ClearEXTIFlag(EXTI_EINT0);

	NVIC_EnableIRQ(EINT0_IRQn);
}

void TIMER1_Init() {
    
	TIM_TIMERCFG_Type tim_cfg;
	tim_cfg.PrescaleOption = TIM_PRESCALE_USVAL;
	tim_cfg.PrescaleValue = 1;
	TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &tim_cfg);

	TIM_MATCHCFG_Type match_cfg;
	match_cfg.MatchChannel = 0;
	match_cfg.IntOnMatch = ENABLE;
	match_cfg.StopOnMatch = DISABLE;
	match_cfg.ResetOnMatch = ENABLE;
	match_cfg.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	match_cfg.MatchValue = DEBOUNCE_DELAY;
	TIM_ConfigMatch(LPC_TIM1, &match_cfg);
}

void set_motor_speed(uint8_t nivel) {
    const uint32_t DAC_min = 0;       // Velocidad mínima
    const uint32_t DAC_med = 341;     // Velocidad media
    const uint32_t DAC_high = 682;    // Velocidad alta
    const uint32_t DAC_max = 1023;    // Velocidad máxima

    uint32_t DAC_value = 0;
    switch(nivel) {
        case 0: DAC_value = DAC_min; break;
        case 1: DAC_value = DAC_med; break;
        case 2: DAC_value = DAC_high; break;
        case 3: DAC_value = DAC_max; break;
    }
    DAC_UpdateValue(LPC_DAC, DAC_value);
    }

// Interrupción para la pulsación del botón (EINT0)
void EINT0_IRQHandler(void) {
    if (!debounce_activo) {
        debounce_activo = 1;                // Activa el debounce
        nivel_velocidad = (nivel_velocidad + 1) % 4; // Cambia el nivel de 0 a 3
        set_motor_speed(nivel_velocidad);   // Ajusta la velocidad según el nivel
        LPC_TIM1->TCR = 1;                  // Inicia el temporizador de debounce (LPC_TIM1)
    }
    LPC_SC->EXTINT |= (1 << 0);             // Limpia la interrupción EINT0 correctamente
}

// Interrupción del temporizador para el debounce
void TIMER1_IRQHandler(void) {
    debounce_activo = 0;                    // Desactiva el debounce
    LPC_TIM1->TCR = 0;                      // Detiene el timer1
    LPC_TIM1->TC = 0;                       // Reinicia el conteo del timer1
    LPC_TIM1->IR |= (1 << 0);               // Limpia el flag de int. timer1
}


