#ifndef SUART_CFG_H_
#define SUART_CFG_H_

/**
 * @file SUART_Cfg.h
 * @brief Configuration header file for the UART service layer on ATmega32.
 *
 * This file contains:
 * - TX queue size configuration
 * - RX queue size configuration
 * - TX terminator length configuration
 * - RX terminator length configuration
 * - External declarations for configured terminator arrays
 *
 * @note Terminator arrays are defined in SUART_Cfg.c.
 *
 * @author Abdelrahman Elzayat
 */

#include "StdTypes.h"

/* =========================================================
 * Queue Configuration
 * ========================================================= */

/**
 * @def SUART_TX_QUEUE_SIZE
 * @brief Number of queued TX requests supported by the UART service layer.
 */
#define SUART_TX_QUEUE_SIZE           5U

/**
 * @def SUART_RX_QUEUE_SIZE
 * @brief Number of queued RX requests supported by the UART service layer.
 */
#define SUART_RX_QUEUE_SIZE           5U

/* =========================================================
 * Terminator Configuration
 * ========================================================= */

/**
 * @def SUART_TX_TERMINATOR_LEN
 * @brief Number of characters appended automatically after each normal TX string.
 *
 * @note Example:
 *       - 1U for {'\n'}
 *       - 2U for {'\r', '\n'}
 */
#define SUART_TX_TERMINATOR_LEN       1U

/**
 * @def SUART_RX_TERMINATOR_LEN
 * @brief Number of characters used to detect the end of a received string.
 *
 * @note Example:
 *       - 1U for {'\n'}
 *       - 2U for {'\r', '\n'}
 */
#define SUART_RX_TERMINATOR_LEN       1U

/* =========================================================
 * Terminator Arrays
 * ========================================================= */

/**
 * @brief Configured TX terminator sequence.
 *
 * This sequence is appended automatically after each normal transmitted string
 * using the service APIs that are not marked as raw.
 *
 * @note The array length must match SUART_TX_TERMINATOR_LEN.
 */
extern const u8 SUART_TxTerminator[SUART_TX_TERMINATOR_LEN];

/**
 * @brief Configured RX terminator sequence.
 *
 * This sequence is used by the UART service layer to detect the end
 * of received strings in terminator-based reception mode.
 *
 * @note The array length must match SUART_RX_TERMINATOR_LEN.
 */
extern const u8 SUART_RxTerminator[SUART_RX_TERMINATOR_LEN];

#endif /* SUART_CFG_H_ */