#ifndef SUART_INT_H_
#define SUART_INT_H_

/**
 * @file SUART_Int.h
 * @brief Public interface for the UART service layer on ATmega32.
 *
 * This module provides higher-level asynchronous UART services built
 * on top of the UART MCAL driver.
 *
 * The service layer works using:
 * - Buffer pointer + length for transmission
 * - Buffer pointer + length for reception
 * - Internal TX and RX queues
 * - Interrupt-driven operation using UART MCAL callbacks
 *
 * This design does not append any extra data during transmission
 * and does not wait for any terminator during reception.
 *
 * @author Abdelrahman Elzayat
 */

#include "StdTypes.h"

/**
 * @typedef SUART_Callback_t
 * @brief Pointer to function used as a service notification callback.
 */
typedef void (*SUART_Callback_t)(void);

/**
 * @brief Initializes the UART service layer.
 *
 * This function initializes internal service state, resets TX/RX queues,
 * and links the required UART MCAL callbacks used by the service layer.
 *
 * @retval OK   Service initialized successfully.
 * @retval NOK  Initialization failed.
 */
error_t SUART_Init(void);

/* =========================================================
 * TX Services
 * ========================================================= */

/**
 * @brief Sends a buffer asynchronously.
 *
 * This function adds the given TX request to the internal TX queue.
 * Data is transmitted exactly as provided without appending any extra bytes.
 *
 * @param[in] Add_pu8Data      Pointer to source data buffer.
 * @param[in] Copy_u16Length   Number of bytes to be transmitted.
 *
 * @retval OK            TX request added successfully.
 * @retval NOK           TX queue is full.
 * @retval NULL_PTR      Input pointer is NULL.
 * @retval OUT_OF_RANGE  Length is invalid.
 *
 * @note The source buffer must remain valid until transmission is completed.
 */
error_t SUART_SendAsync(const u8 *Add_pu8Data, u16 Copy_u16Length);

/**
 * @brief Sends multiple buffers asynchronously.
 *
 * This function adds multiple TX requests to the internal TX queue.
 * Each buffer is transmitted exactly as provided.
 *
 * @param[in] Add_pu8BufferArr     Pointer to array of source buffer pointers.
 * @param[in] Add_pu16LengthArr    Pointer to array of buffer lengths.
 * @param[in] Copy_u8Count         Number of buffers to be queued.
 *
 * @retval OK            All TX requests added successfully.
 * @retval NOK           TX queue does not have enough space.
 * @retval NULL_PTR      One or more input pointers are NULL.
 * @retval OUT_OF_RANGE  One or more lengths are invalid.
 *
 * @note All source buffers must remain valid until transmission is completed.
 */
error_t SUART_SendBuffersAsync(const u8 * const Add_pu8BufferArr[], const u16 Add_pu16LengthArr[], u8 Copy_u8Count);

/* =========================================================
 * RX Services
 * ========================================================= */

/**
 * @brief Receives a buffer asynchronously.
 *
 * This function adds the given RX request to the internal RX queue.
 * Reception continues until the specified number of bytes is received.
 *
 * @param[out] Add_pu8Buffer     Pointer to destination buffer.
 * @param[in]  Copy_u16Length    Number of bytes to be received.
 *
 * @retval OK            RX request added successfully.
 * @retval NOK           RX queue is full.
 * @retval NULL_PTR      Input pointer is NULL.
 * @retval OUT_OF_RANGE  Length is invalid.
 *
 * @note The destination buffer must remain valid until reception is completed.
 */
error_t SUART_ReceiveAsync(u8 *Add_pu8Buffer, u16 Copy_u16Length);

/**
 * @brief Receives multiple buffers asynchronously.
 *
 * This function adds multiple RX requests to the internal RX queue.
 * Each buffer receives exactly the number of bytes specified
 * in the corresponding length entry.
 *
 * @param[out] Add_pu8BufferArr    Pointer to array of destination buffer pointers.
 * @param[in]  Add_pu16LengthArr   Pointer to array of receive lengths.
 * @param[in]  Copy_u8Count        Number of buffers to be queued.
 *
 * @retval OK            All RX requests added successfully.
 * @retval NOK           RX queue does not have enough space.
 * @retval NULL_PTR      One or more input pointers are NULL.
 * @retval OUT_OF_RANGE  One or more lengths are invalid.
 *
 * @note All destination buffers must remain valid until reception is completed.
 */
error_t SUART_ReceiveBuffersAsync(u8 *Add_pu8BufferArr[], const u16 Add_pu16LengthArr[], u8 Copy_u8Count);

/* =========================================================
 * Service Notifications
 * ========================================================= */

/**
 * @brief Registers a callback function called when the TX queue becomes empty.
 *
 * @param[in] Add_pfCallBack Pointer to callback function.
 *
 * @retval OK        Callback registered successfully.
 * @retval NULL_PTR  Input pointer is NULL.
 */
error_t SUART_TX_SetCallBack(SUART_Callback_t Add_pfCallBack);

/**
 * @brief Registers a callback function called when one RX request is completed.
 *
 * This callback is invoked whenever one queued RX buffer is filled successfully.
 *
 * @param[in] Add_pfCallBack Pointer to callback function.
 *
 * @retval OK        Callback registered successfully.
 * @retval NULL_PTR  Input pointer is NULL.
 */
error_t SUART_RX_SetCallBack(SUART_Callback_t Add_pfCallBack);

#endif /* SUART_INT_H_ */