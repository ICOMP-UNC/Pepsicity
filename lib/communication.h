#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "lpc17xx_gpdma.h"
#include "lpc17xx_uart.h"
#include "string.h"
#include <stdlib.h>


/* Canal DMA para utilizar el RX por UART*/
#define DMA_RX_CHANNEL 0
/* Canal DMA para utilizar el TX por UART*/
#define DMA_TX_CHANNEL 1
/*Cantidad de caracteres esperados por UART*/
#define RX_BUFF_SIZE   1
/* Cantidad maxima de caracteres para enviar por UART*/
#define TX_BUFF_SIZE   300
/* Modo de operación A*/
#define MODE_A 'A'
/* Modo de operación B*/
#define MODE_B 'B'
/* Modo de operación C*/
#define MODE_C 'C'


typedef enum
{
    IDLE_STATUS,
    TEMPERATURE_STATUS,
    CHANGE_MODE_STATUS,
    VELOCITY_STATUS,
    COUNTER_STATUS
} EXTERNAL_CONTROL_STATUS;

/* Buffer para la transmisión UART*/
extern uint8_t data_Rx[RX_BUFF_SIZE];
/* Buffer para la recepción UART*/
extern uint8_t data_Tx[TX_BUFF_SIZE];
/* Estado de la comunicación*/
extern EXTERNAL_CONTROL_STATUS control_status;


/**
 * @brief Inicializa la comunicación UART
 */
void init_communication();

/**
 * @brief Configuración de UART2 TX y RX
 */
void configure_uart();

/**
 * @brief Inicializa el canal DMA para la recepción y transmisión de datos por UART
 */
void init_dma();

/**
 * @brief Valida el comando recibido por UART
 */
void restart_rx_uart(uint32_t size_message);

/**
 * @brief Envia datos por UART TX, ejemplo: 
 * char data[] = "Mensaje 2\n\r";   
 * send_data_dma_uart(data, sizeof(data));
 * @param data Datos a enviar
 * @param size Tamaño de los datos a enviar
 */
void send_data_dma_uart(const char *data, uint32_t size);

/**
 * @brief Convierte un arreglo de bytes a un número decimal
 * @param data Arreglo de bytes
 * @return Número decimal
 */
uint16_t get_decimal_data(uint8_t *data);

#endif
