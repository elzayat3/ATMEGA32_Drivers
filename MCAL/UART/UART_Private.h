#ifndef UART_PRIVATE_H_
#define UART_PRIVATE_H_

/**
 * @file UART_Private.h
 * @brief Private macros for UART driver on ATmega32.
 *
 * This file contains internal helper macros used only inside
 * the UART driver implementation.
 *
 * @author Abdelrahman Elzayat
 */

#include "StdTypes.h"
#include "MemMap.h"

/* =========================================
 * UCSRC Register Selection
 * ========================================= */

/**
 * @def UART_UCSRC_SELECT
 * @brief Used to select UCSRC during write access.
 *
 * @note UCSRC and UBRRH share the same I/O address in ATmega32.
 *       URSEL must be set when writing to UCSRC.
 */
#define UART_UCSRC_SELECT        ((u8)(1U << URSEL))

/* =========================================
 * UART Mode Configuration
 * ========================================= */

/**
 * @def UART_ASYNC_MODE_CFG
 * @brief Asynchronous mode configuration.
 */
#define UART_ASYNC_MODE_CFG      ((u8)0x00U)

/**
 * @def UART_SYNC_MODE_CFG
 * @brief Synchronous mode configuration.
 */
#define UART_SYNC_MODE_CFG       ((u8)(1U << UMSEL))

/* =========================================
 * UART Parity Configuration
 * ========================================= */

/**
 * @def UART_PARITY_DISABLED_CFG
 * @brief Parity disabled configuration.
 */
#define UART_PARITY_DISABLED_CFG ((u8)0x00U)

/**
 * @def UART_PARITY_EVEN_CFG
 * @brief Even parity configuration.
 */
#define UART_PARITY_EVEN_CFG     ((u8)(1U << UPM1))

/**
 * @def UART_PARITY_ODD_CFG
 * @brief Odd parity configuration.
 */
#define UART_PARITY_ODD_CFG      ((u8)((1U << UPM1) | (1U << UPM0)))

/* =========================================
 * UART Stop Bit Configuration
 * ========================================= */

/**
 * @def UART_ONE_STOP_BIT_CFG
 * @brief One stop bit configuration.
 */
#define UART_ONE_STOP_BIT_CFG    ((u8)0x00U)

/**
 * @def UART_TWO_STOP_BITS_CFG
 * @brief Two stop bits configuration.
 */
#define UART_TWO_STOP_BITS_CFG   ((u8)(1U << USBS))

/* =========================================
 * UART Character Size Configuration
 * ========================================= */

/**
 * @def UART_5_BIT_CHAR_CFG
 * @brief 5-bit character size configuration.
 */
#define UART_5_BIT_CHAR_CFG      ((u8)0x00U)

/**
 * @def UART_6_BIT_CHAR_CFG
 * @brief 6-bit character size configuration.
 */
#define UART_6_BIT_CHAR_CFG      ((u8)(1U << UCSZ0))

/**
 * @def UART_7_BIT_CHAR_CFG
 * @brief 7-bit character size configuration.
 */
#define UART_7_BIT_CHAR_CFG      ((u8)(1U << UCSZ1))

/**
 * @def UART_8_BIT_CHAR_CFG
 * @brief 8-bit character size configuration.
 */
#define UART_8_BIT_CHAR_CFG      ((u8)((1U << UCSZ1) | (1U << UCSZ0)))

/**
 * @def UART_9_BIT_CHAR_CFG
 * @brief 9-bit character size configuration for UCSRC.
 *
 * @note 9-bit mode also requires setting UCSZ2 in UCSRB.
 */
#define UART_9_BIT_CHAR_CFG      ((u8)((1U << UCSZ1) | (1U << UCSZ0)))

/**
 * @def UART_UCSZ2_CLEAR_CFG
 * @brief Clear UCSZ2 bit for character sizes from 5-bit to 8-bit.
 */
#define UART_UCSZ2_CLEAR_CFG     ((u8)0x00U)

/**
 * @def UART_UCSZ2_SET_CFG
 * @brief Set UCSZ2 bit for 9-bit character size.
 */
#define UART_UCSZ2_SET_CFG       ((u8)(1U << UCSZ2))

#endif /* UART_PRIVATE_H_ */