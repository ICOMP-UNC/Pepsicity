/**
 * @file external_control.c
 * @brief File for external control functions
 *
 * This file contains the implementation of the functions that handle the external control
 */

#include "../inc/external_control.h"

MODES mode = MODE_A;
uint8_t send_available = TRUE;
uint16_t match_counter = 0;

void validate_command()
{
    switch (data_Rx[0])
    {
        case 'M':
        {
            control_status = CHANGE_MODE_STATUS;
            char message[] = "\n\rPor favor, ingrese el modo deseado: A, B o C\n\r";
            send_data_dma_uart(message, sizeof(message));
            restart_rx_uart(RX_BUFF_SIZE_1);
            send_available = FALSE;
        }
        break;
        case 'V':
        {
            control_status = VELOCITY_STATUS;
            char message[] = "\n\rIngrese la velocidad de 01 a 10\n\r";
            send_data_dma_uart(message, sizeof(message));
            restart_rx_uart(RX_BUFF_SIZE_2);
            send_available = FALSE;
        }
        break;
        case 'T':
        {
            char message[STANDARD_MESS_SIZE] = "\n\rEl valor de la temperaturas es:";
            // TODO implementar lectura de temperatura
            strcat(message, "°C\n\rIngrese un nuevo comando\n\r");
            send_data_dma_uart(message, sizeof(message));
            restart_rx_uart(RX_BUFF_SIZE_1);
            control_status = IDLE_STATUS;
        }
        break;
        case 'C':
        {
            if (mode == MODE_B)
            {
                char message[STANDARD_MESS_SIZE] = "\n\rSe ah contabilizado:";
                // TODO implementar contador
                strcat(message, "\n\rIngrese un nuevo comando\n\r");
                send_data_dma_uart(message, sizeof(message));
                restart_rx_uart(RX_BUFF_SIZE_1);
                control_status = IDLE_STATUS;
            }
            else
            {
                char message[] = "\n\rEl modo actual no permite la contabilización de objetos\n\rIngrese un "
                                 "nuevo comando\n\r";
                send_data_dma_uart(message, sizeof(message));
                restart_rx_uart(RX_BUFF_SIZE_1);
            }
        }
        break;
        default:
        {
            char message[] = "\n\rComando no valido, por favor ingrese M,V,C o T\n\r";
            send_data_dma_uart(message, sizeof(message));
            restart_rx_uart(RX_BUFF_SIZE_1);
        }
        break;
    }
}

void validate_new_mode()
{
    switch (data_Rx[0])
    {
        case MODE_A:
        {
            mode = MODE_A;
            char message[] = "\n\rModo A seleccionado\n\r";
            send_data_dma_uart(message, sizeof(message));
            control_status = IDLE_STATUS;
            restart_rx_uart(RX_BUFF_SIZE_1);
            send_available = TRUE;
        }
        break;
        case MODE_B:
        {
            mode = MODE_B;
            char message[] = "\n\rModo B seleccionado\n\r";
            send_data_dma_uart(message, sizeof(message));
            control_status = IDLE_STATUS;
            restart_rx_uart(RX_BUFF_SIZE_1);
            send_available = TRUE;
        }
        break;
        case MODE_C:
        {
            char message[] = "\n\rModo C seleccionado\n\rIngrese la cantidad de objetos a contar (001-999):";
            send_data_dma_uart(message, sizeof(message));
            control_status = COUNTER_STATUS;
            restart_rx_uart(RX_BUFF_SIZE_3);
            send_available = FALSE;
        }
        break;
        default:
        {
            char message[] = "\n\rModo no valido, por favor ingrese A, B o C\n\r";
            send_data_dma_uart(message, sizeof(message));
            restart_rx_uart(RX_BUFF_SIZE_1);
        }
        break;
    }
}

void validate_new_velocity()
{
    uint16_t velocity = get_decimal_data(data_Rx);
    if (velocity >= 1 && velocity <= 10) // TODO cambiar valores definidos en motor control
    {
        // TODO implementar cambio de velocidad
        control_status = IDLE_STATUS;
        char message[STANDARD_MESS_SIZE] = "\n\rVelocidad cambiada\n\rIngrese un nuevo comando\n\r";
        send_data_dma_uart(message, sizeof(message));
        restart_rx_uart(RX_BUFF_SIZE_1);
        send_available = TRUE;
    }
    else
    {
        char message[] = "\n\rVelocidad no valida, por favor ingrese un valor entre 01 y 10\n\r";
        send_data_dma_uart(message, sizeof(message));
        restart_rx_uart(RX_BUFF_SIZE_2);
    }
}

void validate_new_counter()
{
    match_counter = get_decimal_data(data_Rx);
    if (match_counter < 1000 && match_counter > 0) // TODO cambiar valores definidos en counting module
    {
        char message[] = "\n\rCantidad de objetos a contar cambiada\n\rIngrese un nuevo comando\n\r";
        send_data_dma_uart(message, sizeof(message));
        control_status = IDLE_STATUS;
        restart_rx_uart(RX_BUFF_SIZE_1);
        send_available = TRUE;
        mode = MODE_C;
    }
    else
    {
        char message[] = "\n\rCantidad de objetos no valida, por favor ingrese un valor entre 001 y 999\n\r";
        send_data_dma_uart(message, sizeof(message));
        restart_rx_uart(RX_BUFF_SIZE_3);
    }
}
