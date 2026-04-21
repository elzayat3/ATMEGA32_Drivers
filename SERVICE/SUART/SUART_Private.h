#ifndef SUART_PRIVATE_H_
#define SUART_PRIVATE_H_

/**
 * @file SUART_Private.h
 * @brief Private declarations for the UART service layer on ATmega32.
 *
 * This file contains internal types, helper macros, and internal function
 * declarations used only inside the UART service layer implementation.
 *
 * @author Abdelrahman Elzayat
 */

#include "StdTypes.h"

/* =========================================================
 * Internal Request Types
 * ========================================================= */

/**
 * @enum SUART_TxMode_t
 * @brief Internal TX request mode.
 */
typedef enum
{
	SUART_TX_NORMAL = 0, /**< Send string and append configured TX terminator */
	SUART_TX_RAW         /**< Send raw string without TX terminator */
} SUART_TxMode_t;

/**
 * @enum SUART_RxMode_t
 * @brief Internal RX request mode.
 */
typedef enum
{
	SUART_RX_STRING = 0, /**< Receive until configured RX terminator */
	SUART_RX_RAW         /**< Receive fixed-length raw buffer */
} SUART_RxMode_t;

/* =========================================================
 * Internal Queue Entry Types
 * ========================================================= */

/**
 * @struct SUART_TxRequest_t
 * @brief Internal TX queue entry.
 */
typedef struct
{
	const u8 *Str;         /**< Pointer to source string */
	SUART_TxMode_t Mode;   /**< TX mode: normal or raw */
} SUART_TxRequest_t;

/**
 * @struct SUART_RxRequest_t
 * @brief Internal RX queue entry.
 */
typedef struct
{
	u8 *Buffer;            /**< Pointer to destination buffer */
	u16 Length;            /**< Buffer size or raw receive length */
	SUART_RxMode_t Mode;   /**< RX mode: string or raw */
} SUART_RxRequest_t;

/* =========================================================
 * Internal State Macros
 * ========================================================= */

/**
 * @def SUART_IDLE
 * @brief Internal state value indicating idle service.
 */
#define SUART_IDLE      0U

/**
 * @def SUART_BUSY
 * @brief Internal state value indicating active service.
 */
#define SUART_BUSY      1U

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