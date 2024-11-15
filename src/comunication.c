#include "../lib/comunication.h"

/* Buffers */
static uint8_t data_Rx[RX_BUFF_SIZE];
static uint8_t data_Tx[TX_BUFF_SIZE];

/* GPDMA Config */
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

/* GPDMA Config */
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

/**
 * @brief Initialize UART and DMA
 */
void init_comunication()
{
    configure_uart();
    init_dma();
}

/**
 * @brief Configure UART for comunication
 */
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

/**
 * @brief   Configure DMA for transmission and reception data
 */
void init_dma()
{
    GPDMA_Init();
    GPDMA_Setup(&GPDMACfg_RX);
    GPDMA_ClearIntPending(GPDMA_STATCLR_INTTC, DMA_RX_CHANNEL);
    GPDMA_ChannelCmd(DMA_RX_CHANNEL, ENABLE);

    GPDMA_Setup(&GPDMACfg_TX);
    GPDMA_ClearIntPending(GPDMA_STATCLR_INTTC, DMA_TX_CHANNEL);
    GPDMA_ChannelCmd(DMA_TX_CHANNEL, DISABLE);
    // Deshabilitar la interrupción de transferencia
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
	return (uint16_t)strtol((char*)data, NULL, 10);
}

void DMA_IRQHandler()
{
    if (GPDMA_IntGetStatus(GPDMA_STAT_INT, DMA_RX_CHANNEL))
    {
        GPDMA_ClearIntPending(GPDMA_STATCLR_INTTC, DMA_RX_CHANNEL);
        GPDMA_Setup(&GPDMACfg_RX);
        GPDMA_ChannelCmd(DMA_RX_CHANNEL, ENABLE);
    }
    else if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, DMA_RX_CHANNEL) || GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 1))
    {
        while (1);
    }
}
