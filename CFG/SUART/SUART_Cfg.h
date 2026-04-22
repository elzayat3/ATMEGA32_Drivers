#ifndef SUART_CFG_H_
#define SUART_CFG_H_

/**
 * @file SUART_Cfg.h
 * @brief Configuration header file for the UART service layer on ATmega32.
 *
 * This file contains queue size configuration used by the UART service layer.
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
#define SUART_TX_QUEUE_SIZE    5U

/**
 * @def SUART_RX_QUEUE_SIZE
 * @brief Number of queued RX requests supported by the UART service layer.
 */
#define SUART_RX_QUEUE_SIZE    5U

#endif /* SUART_CFG_H_ */