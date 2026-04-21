#ifndef UART_CFG_H_
#define UART_CFG_H_

/**
 * @file UART_Cfg.h
 * @brief UART driver configuration header file for ATmega32.
 *
 * This file contains the external declaration of the UART configuration object
 * used by the UART driver during initialization.
 *
 * @note The UART_Config object is defined in UART_Cfg.c.
 *
 * @author Abdelrahman Elzayat
 */

#include "UART_Int.h"

/**
 * @brief External declaration of the UART configuration object.
 *
 * This object holds the user-selected UART configuration parameters
 * that are used by UART_Init().
 */
extern const UART_Config_t UART_Config;

#endif /* UART_CFG_H_ */