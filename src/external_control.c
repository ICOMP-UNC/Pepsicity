/**
 * @file external_control.c
 * @brief File for external control functions
 *
 * This file contains the implementation of the functions that handle the external control
 */

#include "external_control.h"

MODES mode = MODE_A;
uint8_t send_available = TRUE;
uint16_t match_counter = 0;

void validate_command(uint8_t* command)
{
    switch (command[0])
    {
        case 'M':
        {
            control_status = CHANGE_MODE_STATUS;
            char message[] = "\n\rPor favor, ingrese el modo deseado: A, B o C\n\r";
            send_data_dma_uart(message, sizeof(message));
            restart_rx_uart(1);
            send_available = FALSE;
        }
        break;
        case 'V':
        {
            control_status = VELOCITY_STATUS;
            char message[] = "\n\rIngrese la velocidad de 01 a 10\n\r";
            send_data_dma_uart(message, sizeof(message));
            restart_rx_uart(2);
            send_available = FALSE;
        }
        break;
        case 'T':
        {
            char message[70] = "\n\rEl valor de la temperaturas es:";
            // TODO implementar lectura de temperatura
            strcat(message, "°C\n\rIngrese un nuevo comando\n\r");
            send_data_dma_uart(message, sizeof(message));
            restart_rx_uart(1);
            control_status = IDLE_STATUS;
        }
        break;
        case 'C':
        {
            if (mode == MODE_B)
            {
                char message[70] = "\n\rSe ah contabilizado:";
                // TODO implementar contador
                strcat(message, "°C\n\rIngrese un nuevo comando\n\r");
                send_data_dma_uart(message, sizeof(message));
                restart_rx_uart(1);
                control_status = IDLE_STATUS;
            }
            else
            {
                char message[] = "\n\rEl modo actual no permite la contabilización de objetos\n\rIngrese un "
                                 "nuevo comando\n\r";
                send_data_dma_uart(message, sizeof(message));
                restart_rx_uart(1);
            }
        }
        default:
        {
            char message[] = "\n\rComando no valido, por favor ingrese M,V,C o T\n\r";
            send_data_dma_uart(message, sizeof(message));
            restart_rx_uart(1);
        }
        break;
    }
}

void validate_new_mode(uint8_t* new_mode)
{
    switch (new_mode[0])
    {
        case MODE_A:
        {
            mode = MODE_A;
            char message[] = "\n\rModo A seleccionado\n\r";
            send_data_dma_uart(message, sizeof(message));
            control_status = IDLE_STATUS;
            restart_rx_uart(1);
            send_available = TRUE;
        }
        break;
        case MODE_B:
        {
            mode = MODE_B;
            char message[] = "\n\rModo B seleccionado\n\r";
            send_data_dma_uart(message, sizeof(message));
            control_status = IDLE_STATUS;
            restart_rx_uart(1);
            send_available = TRUE;
        }
        break;
        case MODE_C:
        {
            match_counter = get_decimal_data(new_mode);
            char message[] = "\n\rModo C seleccionado\n\rIngrese la cantidad de objetos a contar (001-999):";
            send_data_dma_uart(message, sizeof(message));
            control_status = COUNTER_STATUS;
            restart_rx_uart(3);
            send_available = FALSE;
        }
        break;
        default:
        {
            char message[] = "\n\rModo no valido, por favor ingrese A, B o C\n\r";
            send_data_dma_uart(message, sizeof(message));
            restart_rx_uart(1);
        }
        break;
    }
}

void validate_new_velocity(uint8_t* new_velocity)
{
    uint16_t velocity = get_decimal_data(new_velocity);
    if (velocity >= 1 && velocity <= 10)
    {
        // TODO implementar cambio de velocidad
        control_status = IDLE_STATUS;
        char message[50] = "\n\rVelocidad cambiada\n\rIngrese un nuevo comando\n\r";
        send_data_dma_uart(message, sizeof(message));
        restart_rx_uart(1);
        send_available = TRUE;
    }
    else
    {
        char message[] = "\n\rVelocidad no valida, por favor ingrese un valor entre 01 y 10\n\r";
        send_data_dma_uart(message, sizeof(message));
        restart_rx_uart(2);
    }
}

void validate_new_counter(uint8_t* new_counter)
{
    match_counter = get_decimal_data(new_counter);
    if (match_counter<1000 | match_counter> 0)
    {
        char message[] = "\n\rCantidad de objetos a contar cambiada\n\rIngrese un nuevo comando\n\r";
        send_data_dma_uart(message, sizeof(message));
        control_status = IDLE_STATUS;
        restart_rx_uart(1);
        send_available = TRUE;
        mode = MODE_C;
    }
    else
    {
        char message[] = "\n\rCantidad de objetos no valida, por favor ingrese un valor entre 001 y 999\n\r";
        send_data_dma_uart(message, sizeof(message));
        restart_rx_uart(3);
    }
}
