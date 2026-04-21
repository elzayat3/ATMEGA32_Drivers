#include "UART_Int.h"
#include "UART_Cfg.h"

/**
 * @file UART_Cfg.c
 * @brief UART driver configuration source file for ATmega32.
 *
 * This file contains the user-selected UART configuration object
 * used by UART_Init().
 *
 * @note The selected baud rate enum values are assumed to represent
 *       precomputed UBRR values for:
 *       - F_CPU = 8 MHz
 *       - Asynchronous mode
 *       - Normal speed mode (U2X = 0)
 *
 * @author Abdelrahman Elzayat
 */

const UART_Config_t UART_Config =
{
	.mode      = UART_ASYNCHRONOUS_MODE, /* UART mode */
	.speed     = UART_NORMAL_SPEED,      /* speed mode */
	.parity    = UART_PARITY_DISABLED,   /* parity mode */
	.stop_bits = UART_ONE_STOP_BIT,      /* number of stop bits */
	.char_size = UART_8_BIT_CHAR,        /* frame character size */
	.baud_rate = UART_BAUD_9600,         /* UBRR value for 9600 bps @ 8 MHz */
	.tx_enable = TRUE,                   /* transmitter enable */
	.rx_enable = TRUE                    /* receiver enable */
};