#ifndef SUART_PRIVATE_H_
#define SUART_PRIVATE_H_

/**
 * @file SUART_Private.h
 * @brief Private declarations for the UART service layer on ATmega32.
 *
 * This file contains internal types, state macros, and internal function
 * declarations used only inside the UART service layer implementation.
 *
 * @author Abdelrahman Elzayat
 */

#include "StdTypes.h"

/* =========================================================
 * Internal Queue Entry Types
 * ========================================================= */

/**
 * @struct SUART_TxRequest_t
 * @brief Internal TX queue entry.
 */
typedef struct
{
	const u8 *Data; /**< Pointer to source data buffer */
	u16 Length;     /**< Number of bytes to transmit */
} SUART_TxRequest_t;

/**
 * @struct SUART_RxRequest_t
 * @brief Internal RX queue entry.
 */
typedef struct
{
	u8 *Buffer; /**< Pointer to destination buffer */
	u16 Length; /**< Number of bytes to receive */
} SUART_RxRequest_t;

/* =========================================================
 * Internal State Macros
 * ========================================================= */

/**
 * @def SUART_IDLE
 * @brief Internal state value indicating idle service.
 */
#define SUART_IDLE    0U

/**
 * @def SUART_BUSY
 * @brief Internal state value indicating active service.
 */
#define SUART_BUSY    1U

/* =========================================================
 * Internal Function Prototypes
 * ========================================================= */

/**
 * @brief Internal UART TX empty callback handler used by the service layer.
 *
 * @return void
 */
static void SUART_TxHandler(void);

/**
 * @brief Internal UART RX callback handler used by the service layer.
 *
 * @return void
 */
static void SUART_RxHandler(void);

#endif /* SUART_PRIVATE_H_ */