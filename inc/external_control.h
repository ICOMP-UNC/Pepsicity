/**
 * @file external_control.h
 * @brief This file contains the prototypes for the functions that handle the external control
 *
 * This file contains the prototypes for the functions that handle the external control
 */

#ifndef EXTERNAL_CONTROL_H
#define EXTERNAL_CONTROL_H

#include "communication.h"

/** Modes of operation */
typedef enum
{
    MODE_A = 'A', /**< Mode of operation A*/
    MODE_B = 'B', /**< Mode of operation B*/
    MODE_C = 'C'  /**< Mode of operation C*/
} MODES;

extern MODES mode;             /**< Mode of operation */
extern uint8_t send_available; /**< Flag to indicate if the system is ready to send automatic data */
extern uint16_t match_counter; /**< Counter to match the system stop condition */

/**
 * @brief In case of receiving a command, it interprets the command and executes the corresponding action, only in
 * IDLE_STATUS
 */
void validate_command();

/**
 * @brief In case of receiving a new mode, it interprets the mode and executes the corresponding action, only in
 * CHANGE_MODE_STATUS
 */
void validate_new_mode();

/**
 * @brief In case of receiving a new velocity, it interprets the velocity and executes the corresponding action, only in
 * VELOCITY_STATUS
 */
void validate_new_velocity();

/**
 * @brief In case of receiving a new counter, it interprets the counter and executes the corresponding action, only in
 * COUNTER_STATUS
 */
void validate_new_counter();

#endif
