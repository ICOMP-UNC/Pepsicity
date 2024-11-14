#include "../lib/comunication.h"

uint8_t data_Rx[RX_BUFF_SIZE];
uint8_t data_Tx[TX_BUFF_SIZE];

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

void init_comunication()
{
    config_uart();
    init_dma();
}

void config_uart()
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

    NVIC_EnableIRQ(DMA_IRQn);
}

void send_data_dma_uart(const char* data, uint32_t size)
{
    memset(data_Tx, (int)'\0', TX_BUFF_SIZE);
    memcpy(data_Tx, data, size);

    GPDMA_Setup(&GPDMACfg_TX);
    GPDMA_ChannelCmd(DMA_TX_CHANNEL, ENABLE);
}

void DMA_IRQHandler()
{
    if (GPDMA_IntGetStatus(GPDMA_STAT_INT, 0))
    {
        GPDMA_ClearIntPending(GPDMA_STATCLR_INTTC, 0);
        GPDMA_Setup(&GPDMACfg_RX);
        GPDMA_ChannelCmd(DMA_RX_CHANNEL, ENABLE);
        // Ejemplo de uso de la función send_data_dma_uart, \n es un salto de línea y \r es un retorno al inicio de la línea
        char data[] = "Mensaje 2\n\r";
        send_data_dma_uart(data, sizeof(data));
    }
    else if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 0) || GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 1))
    {
        while (1);
    }
}
