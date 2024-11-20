/**
 * @file communication.h
 * @brief Header file for communication.c
 *
 * This file contains the prototypes for the functions that handle the communication
 */

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "../lib/CMSISv2p00_LPC17xx/drivers/include/lpc17xx_gpdma.h"
#include "../lib/CMSISv2p00_LPC17xx/drivers/include/lpc17xx_uart.h"
#include "string.h"
#include <stdlib.h>

#define DMA_RX_CHANNEL     0                     /**< DMA channel for using RX via UART */
#define DMA_TX_CHANNEL     1                     /**< DMA channel for using TX via UART */
#define RX_BUFF_SIZE       3                     /**< Number of characters expected via UART */
#define TX_BUFF_SIZE       300                   /**< Maximum number of characters to send via UART */
#define STANDARD_MESS_SIZE 70                    /**< Standard message size */
#define BUFF_SIZE_3        3                     /**< Number of characters for buffer */
#define BUFF_SIZE_2        2                     /**< Number of characters for buffer */
#define BUFF_SIZE_1        1                     /**< Number of characters for buffer */
#define BASE_10            10                    /**< The base of the number system used for conversion */
#define TX_INT             ((uint32_t)(1 << 31)) /**< TX interrupt */

/** Types of communication states*/
typedef enum
{
    IDLE_STATUS,        /**< Get new command status */
    CHANGE_MODE_STATUS, /**< Get change mode status */
    VELOCITY_STATUS,    /**< Get new velocity status */
    COUNTER_STATUS      /**< Send counter status */
} EXTERNAL_CONTROL_STATUS;

extern uint8_t data_Rx[RX_BUFF_SIZE];          /**< Buffer for UART reception */
extern uint8_t data_Tx[TX_BUFF_SIZE];          /**< Buffer for UART transmission */
extern EXTERNAL_CONTROL_STATUS control_status; /**< Communication status */

/**
 * @brief Initializes UART communication
 */
void init_communication();

/**
 * @brief Configuration of UART2 TX and RX
 */
void configure_uart();

/**
 * @brief Initializes the DMA channel for receiving and transmitting data via UART
 */
void init_dma();

/**
 * @brief Restarts the DMA channel for receiving data via UART
 * @param size_message Size of the message to receive
 */
void restart_rx_uart(uint32_t size_message);

/**
 * @brief Sends data via UART TX, example:
 * char data[] = "Message 2\n\r";
 * send_data_dma_uart(data, sizeof(data));
 * @param data Data to send
 * @param size Size of the data to send
 */
void send_data_dma_uart(const char* data, uint32_t size);

/**
 * @brief Converts an array of bytes to a decimal number
 * @param data Array of bytes
 * @return Decimal number
 */
uint16_t get_decimal_data(uint8_t* data);

/**
 * @brief Converts a decimal number to an array of bytes
 * @param data Decimal number
 * @param buffer Result array of bytes
 * @param digits Number of digits of the decimal number
 */
void decimal_to_string(uint16_t data, char* buffer, uint8_t digits);

/**
 * @brief Concatenates a decimal number to a message
 * @param data Decimal number
 * @param message_dest String to concatenate
 * @param digits Number of digits of the decimal number
 */
void concat_decimal_to_string(uint16_t data, char* message_dest, uint8_t digits);

/**
 * @brief Interpretation of the message received by the UART receiver, based on the current state
 */
void received_data_interpretation();

/**
 * @brief DMA interrupt handler
 */
void DMA_IRQHandler();

#endif
