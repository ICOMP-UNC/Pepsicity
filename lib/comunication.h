#include "lpc17xx_gpdma.h"
#include "lpc17xx_uart.h"
#include "string.h"

/* Canal DMA para utilizar el RX por UART*/
#define DMA_RX_CHANNEL 0
/* Canal DMA para utilizar el TX por UART*/
#define DMA_TX_CHANNEL 1
/*Cantidad de caracteres esperados por UART*/
#define RX_BUFF_SIZE   10
/* Cantidad maxima de caracteres para enviar por UART*/
#define TX_BUFF_SIZE   300

/* Buffer para la transmisión UART*/
extern uint8_t data_Rx[RX_BUFF_SIZE];
/* Buffer para la recepción UART*/
extern uint8_t data_Tx[TX_BUFF_SIZE];

/**
 * @brief Inicializa la comunicación UART
 */
void init_comunication();

/**
 * @brief Configuración de UART2 TX y RX
 */
void config_uart();

/**
 * @brief Inicializa el canal DMA para la recepción y transmisión de datos por UART
 */
void init_dma();

/**
 * @brief Envia datos por UART TX, ejemplo: 
 * char data[] = "Mensaje 2\n\r";   
 * send_data_dma_uart(data, sizeof(data));
 * @param data Datos a enviar
 * @param size Tamaño de los datos a enviar
 */
void send_data_dma_uart(const char *data, uint32_t size);
