#include "../lib/communication.h"

uint8_t data_Rx[RX_BUFF_SIZE];
uint8_t data_Tx[TX_BUFF_SIZE];
EXTERNAL_CONTROL_STATUS control_status = IDLE_STATUS;

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

void init_communication()
{
    configure_uart();
    init_dma();
    char message[] = "\n\rBienvendo a Pepsicity.\n\rPuede cambiar el funcionamiento con M, o la velocidad con V y "
                     "solicitar el valor de temperatura T\n\r";
    send_data_dma_uart(message, sizeof(message));
    restart_rx_uart(1);
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

void decimal_to_string(uint16_t data, char* buffer, uint8_t digits)
{
    for (uint8_t i = 0; i < digits; i++)
    {
        buffer[digits - 1 - i] = (data % 10) + '0'; // Extrae el dígito menos significativo y lo convierte a carácter
        data /= 10;                                 // Elimina el dígito menos significativo
    }
}

void restart_rx_uart(uint32_t size_message)
{
    GPDMACfg_RX.TransferSize = size_message;
    GPDMA_Setup(&GPDMACfg_RX);
    GPDMA_ChannelCmd(DMA_RX_CHANNEL, ENABLE);
}

void validate_command()
{
    switch (control_status)
    {
        case IDLE_STATUS:
            switch (data_Rx[0])
            {
                case 'M':
                {
                    control_status = CHANGE_MODE_STATUS;
                    char message[] = "\n\rPor favor, ingrese el modo deseado: A, B o C\n\r";
                    send_data_dma_uart(message, sizeof(message));
                    restart_rx_uart(1);
                }
                break;
                case 'V':
                {
                    control_status = VELOCITY_STATUS;
                    char message[] = "\n\rIngrese la velocidad de 01 a 10\n\r";
                    send_data_dma_uart(message, sizeof(message));
                    restart_rx_uart(2);
                }
                break;
                case 'T':
                {
                    control_status = TEMPERATURE_STATUS;
                    char message[50] = "\n\rEl valor de la temperaturas es:";
                    int a = 54;
                    char num[3];
                    decimal_to_string(a, num, 2);
                    num[2] = '\0';
                    strcat(message, num);
                    send_data_dma_uart(message, sizeof(message));
                    restart_rx_uart(1);
                    control_status = IDLE_STATUS;
                }
                break;
                case 'C':
                {
                    control_status = COUNTER_STATUS;
                    char message[] = "\n\rIngrese el valor del contador de 000 a 999\n\r";
                    send_data_dma_uart(message, sizeof(message));
                    restart_rx_uart(1);
                    control_status = IDLE_STATUS;
                }
                default:
                {
                    char message[] = "\n\rComando no valido, por favor ingrese M,V,C o T\n\r";
                    send_data_dma_uart(message, sizeof(message));
                    restart_rx_uart(1);
                }
                break;
            }
            break;
        case CHANGE_MODE_STATUS:
            switch (data_Rx[0])
            {
                case MODE_A:
                {
                    char message[] = "\n\rModo A seleccionado\n\r";
                    send_data_dma_uart(message, sizeof(message));
                    control_status = IDLE_STATUS;
                    restart_rx_uart(1);
                }
                break;
                case MODE_B:
                {
                    char message[] = "\n\rModo B seleccionado\n\r";
                    send_data_dma_uart(message, sizeof(message));
                    control_status = IDLE_STATUS;
                    restart_rx_uart(1);
                }
                break;
                case MODE_C:
                {
                    char message[] = "\n\rModo C seleccionado\n\r";
                    send_data_dma_uart(message, sizeof(message));
                    control_status = IDLE_STATUS;
                    restart_rx_uart(1);
                } break;
                default:
                {
                    char message[] = "\n\rModo no valido, por favor ingrese A, B o C\n\r";
                    send_data_dma_uart(message, sizeof(message));
                    restart_rx_uart(1);
                }
                break;
            }
            break;
        default: break;
    }
}

void DMA_IRQHandler()
{
    if (GPDMA_IntGetStatus(GPDMA_STAT_INT, DMA_RX_CHANNEL))
    {
        GPDMA_ClearIntPending(GPDMA_STATCLR_INTTC, DMA_RX_CHANNEL);

        validate_command();
    }
    else if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, DMA_RX_CHANNEL) || GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 1))
    {
        while (1);
    }
}
