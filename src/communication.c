/**
 * @file communication.c
 * @brief File for communication functions
 *
 * This file contains the implementation of the functions that handle the communication
 */

#include "../inc/communication.h"

uint8_t data_Rx[RX_BUFF_SIZE];                        /**< Buffer for UART reception */
uint8_t data_Tx[TX_BUFF_SIZE];                        /**< Buffer for UART transmission */
EXTERNAL_CONTROL_STATUS control_status = IDLE_STATUS; /**< Communication status */

/** Configuration structure for the DMA RX channel */
GPDMA_Channel_CFG_Type GPDMACfg_RX = {
    DMA_RX_CHANNEL,         // ChannelNum
    RX_BUFF_SIZE,           // TransferSize
    (uint32_t)NULL,         // TransferWidth
    (uint32_t)NULL,         // SrcMemAddr
    (uint32_t)data_Rx,      // DstMemAddr
    GPDMA_TRANSFERTYPE_P2M, // TransferType
    GPDMA_CONN_UART2_Rx,    // SrcConn
    (uint32_t)NULL,         // DstConn
    (uint32_t)NULL          // DMALLI
};

/** Configuration structure for the DMA TX channel */
GPDMA_Channel_CFG_Type GPDMACfg_TX = {
    DMA_TX_CHANNEL,                // ChannelNum
    TX_BUFF_SIZE,                  // TransferSize
    (uint32_t)NULL,                // TransferWidth
    (uint32_t)data_Tx,             // SrcMemAddr
    (uint32_t)NULL,                // DstMemAddr
    GPDMA_TRANSFERTYPE_M2P,        // TransferType
    (uint32_t)NULL,                // SrcConn
    (uint32_t)GPDMA_CONN_UART2_Tx, // DstConn
    (uint32_t)NULL                 // DMALLI
};

void init_communication()
{
    configure_uart();
    init_dma();
    char message[] = "\n\rBienvendo a Pepsicity.\n\rPuede cambiar el funcionamiento con M, o la velocidad con V y "
                     "solicitar el valor de temperatura T\n\r";
    send_data_dma_uart(message, sizeof(message));
    restart_rx_uart(SIZE_MESSAGE);
}

void configure_uart()
{
    UART_CFG_Type UARTConfigStruct;
    UART_FIFO_CFG_Type UARTFIFOConfigStruct;
    UARTFIFOConfigStruct.FIFO_DMAMode = ENABLE;
    UARTFIFOConfigStruct.FIFO_Level = UART_FIFO_TRGLEV2;
    UARTFIFOConfigStruct.FIFO_ResetRxBuf = ENABLE;
    UARTFIFOConfigStruct.FIFO_ResetTxBuf = ENABLE;

    UART_ConfigStructInit(&UARTConfigStruct);
    UART_Init(LPC_UART2, &UARTConfigStruct);
    UART_TxCmd(LPC_UART2, ENABLE);
    UART_FIFOConfig(LPC_UART2, &UARTFIFOConfigStruct);
}

void init_dma()
{
    GPDMA_Init();
    GPDMA_Setup(&GPDMACfg_RX);
    GPDMA_ClearIntPending(GPDMA_STATCLR_INTTC, DMA_RX_CHANNEL);
    GPDMA_ChannelCmd(DMA_RX_CHANNEL, ENABLE);

    GPDMA_Setup(&GPDMACfg_TX);
    GPDMA_ClearIntPending(GPDMA_STATCLR_INTTC, DMA_TX_CHANNEL);
    GPDMA_ChannelCmd(DMA_TX_CHANNEL, DISABLE);
    // Disable the transfer interrupt
    LPC_GPDMACH1->DMACCConfig &= ~(1 << 31);

    NVIC_EnableIRQ(DMA_IRQn);
}

void send_data_dma_uart(const char* data, uint32_t size)
{
    memset(data_Tx, (int)'\0', TX_BUFF_SIZE);
    memcpy(data_Tx, data, size);

    GPDMA_Setup(&GPDMACfg_TX);
    GPDMA_ChannelCmd(DMA_TX_CHANNEL, ENABLE);
}

uint16_t get_decimal_data(uint8_t* data)
{
    return (uint16_t)strtol((char*)data, NULL, BASE_10);
}

void decimal_to_string(uint16_t data, char* buffer, uint8_t digits)
{
    for (uint8_t digit = 0; digit < digits; digit++)
    {
        buffer[digits - 1 - digit] = (data % BASE_10) + '0';
        data /= BASE_10;
    }
}

void restart_rx_uart(uint32_t size_message)
{
    GPDMACfg_RX.TransferSize = size_message;
    GPDMA_Setup(&GPDMACfg_RX);
    GPDMA_ChannelCmd(DMA_RX_CHANNEL, ENABLE);
}

void received_data_interpretation()
{
    switch (control_status)
    {
        case IDLE_STATUS: validate_command(); break;
        case CHANGE_MODE_STATUS: validate_new_mode(); break;
        case VELOCITY_STATUS: validate_new_velocity(); break;
        case COUNTER_STATUS: validate_new_counter(); break;
        default:
        {
            char message[] = "\n\rNo tendrías que poder ver esto, fuera de aquí\n\r";
            send_data_dma_uart(message, sizeof(message));
            restart_rx_uart(SIZE_MESSAGE);
        }
        break;
            memset(data_Rx, '\0', RX_BUFF_SIZE);
    }
}

void DMA_IRQHandler()
{
    if (GPDMA_IntGetStatus(GPDMA_STAT_INT, DMA_RX_CHANNEL))
    {
        GPDMA_ClearIntPending(GPDMA_STATCLR_INTTC, DMA_RX_CHANNEL);

        received_data_interpretation();
    }
    else if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, DMA_RX_CHANNEL) ||
             GPDMA_IntGetStatus(GPDMA_STAT_INTERR, DMA_TX_CHANNEL))
    {
        while (1)
            ;
    }
}
