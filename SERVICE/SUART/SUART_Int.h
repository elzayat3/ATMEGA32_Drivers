#ifndef SUART_INT_H_
#define SUART_INT_H_

/**
 * @file SUART_Int.h
 * @brief Public interface for the UART service layer on ATmega32.
 *
 * This module provides higher-level UART services built on top of the UART MCAL driver.
 *
 * Supported services include:
 * - Asynchronous transmission of strings
 * - Asynchronous transmission of raw strings without terminator
 * - Queued transmission using array of string pointers
 * - Asynchronous reception of strings using configurable terminator
 * - Asynchronous reception of raw buffers with fixed length
 * - Queued reception using array of buffer pointers
 *
 * @note This service layer depends on the underlying UART MCAL driver.
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
 * @retval OK        Service initialized successfully.
 * @retval NOK       Initialization failed.
 */
error_t SUART_Init(void);

/* =========================================================
 * TX Services
 * ========================================================= */

/**
 * @brief Sends a null-terminated string asynchronously.
 *
 * This function adds the given string to the TX service queue.
 * The configured TX terminator is automatically appended after the string.
 *
 * @param[in] Add_pu8Str Pointer to the string to be transmitted.
 *
 * @retval OK        String added to TX queue successfully.
 * @retval NOK       TX queue is full or service is busy.
 * @retval NULL_PTR  Input pointer is NULL.
 *
 * @note The passed string pointer must remain valid until transmission is completed.
 */
error_t SUART_SendStringAsync(const u8 *Add_pu8Str);

/**
 * @brief Sends multiple null-terminated strings asynchronously.
 *
 * This function adds an array of string pointers to the TX service queue.
 * The configured TX terminator is automatically appended after each string.
 *
 * @param[in] Add_pu8StrArr Pointer to array of string pointers.
 * @param[in] Copy_u8Count  Number of strings to be queued.
 *
 * @retval OK        Strings added to TX queue successfully.
 * @retval NOK       TX queue does not have enough space or service is busy.
 * @retval NULL_PTR  Input pointer is NULL.
 *
 * @note Each passed string pointer must remain valid until transmission is completed.
 */
error_t SUART_SendStringsAsync(const u8 * const Add_pu8StrArr[], u8 Copy_u8Count);

/**
 * @brief Sends a null-terminated string asynchronously without appending TX terminator.
 *
 * This function adds the given raw string to the TX service queue exactly as provided.
 *
 * @param[in] Add_pu8Str Pointer to the raw string to be transmitted.
 *
 * @retval OK        Raw string added to TX queue successfully.
 * @retval NOK       TX queue is full or service is busy.
 * @retval NULL_PTR  Input pointer is NULL.
 *
 * @note The passed string pointer must remain valid until transmission is completed.
 */
error_t SUART_SendRawStringAsync(const u8 *Add_pu8Str);

/**
 * @brief Sends multiple null-terminated raw strings asynchronously without TX terminator.
 *
 * This function adds an array of raw string pointers to the TX service queue.
 * Each string is transmitted exactly as provided without appending the configured TX terminator.
 *
 * @param[in] Add_pu8StrArr Pointer to array of raw string pointers.
 * @param[in] Copy_u8Count  Number of strings to be queued.
 *
 * @retval OK        Raw strings added to TX queue successfully.
 * @retval NOK       TX queue does not have enough space or service is busy.
 * @retval NULL_PTR  Input pointer is NULL.
 *
 * @note Each passed string pointer must remain valid until transmission is completed.
 */
error_t SUART_SendRawStringsAsync(const u8 * const Add_pu8StrArr[], u8 Copy_u8Count);

/* =========================================================
 * RX Services
 * ========================================================= */

/**
 * @brief Receives a string asynchronously until the configured RX terminator is detected.
 *
 * This function registers a buffer in the RX service queue.
 * Reception continues until the configured RX terminator sequence is received.
 * The service then stores a null terminator '\\0' at the end of the received string.
 *
 * @param[out] Add_pu8Str         Pointer to destination buffer.
 * @param[in]  Copy_u16BufferSize Size of destination buffer in bytes.
 *
 * @retval OK            Buffer added to RX queue successfully.
 * @retval NOK           RX queue is full or service is busy.
 * @retval NULL_PTR      Input pointer is NULL.
 * @retval OUT_OF_RANGE  Buffer size is invalid.
 *
 * @note The destination buffer must remain valid until reception is completed.
 * @note Buffer size should be large enough to hold received data plus null terminator.
 */
error_t SUART_ReceiveStringAsync(u8 *Add_pu8Str, u16 Copy_u16BufferSize);

/**
 * @brief Receives multiple strings asynchronously using an array of destination buffers.
 *
 * This function registers multiple buffers in the RX service queue.
 * Each buffer receives one string terminated by the configured RX terminator.
 *
 * @param[out] Add_pu8StrArr           Pointer to array of destination buffer pointers.
 * @param[in]  Add_pu16BufferSizeArr   Pointer to array of buffer sizes.
 * @param[in]  Copy_u8Count            Number of buffers to be queued.
 *
 * @retval OK            Buffers added to RX queue successfully.
 * @retval NOK           RX queue does not have enough space or service is busy.
 * @retval NULL_PTR      Input pointer is NULL.
 * @retval OUT_OF_RANGE  One or more buffer sizes are invalid.
 *
 * @note All destination buffers must remain valid until reception is completed.
 */
error_t SUART_ReceiveStringsAsync(u8 *Add_pu8StrArr[], const u16 Add_pu16BufferSizeArr[], u8 Copy_u8Count);

/**
 * @brief Receives a raw buffer asynchronously with fixed length.
 *
 * This function registers a destination buffer in the RX service queue.
 * Reception continues until the specified number of bytes is received.
 * No null terminator is appended automatically.
 *
 * @param[out] Add_pu8Buffer      Pointer to destination buffer.
 * @param[in]  Copy_u16Length     Number of bytes to receive.
 *
 * @retval OK            Buffer added to RX queue successfully.
 * @retval NOK           RX queue is full or service is busy.
 * @retval NULL_PTR      Input pointer is NULL.
 * @retval OUT_OF_RANGE  Length is invalid.
 *
 * @note The destination buffer must remain valid until reception is completed.
 */
error_t SUART_ReceiveRawBufferAsync(u8 *Add_pu8Buffer, u16 Copy_u16Length);

/**
 * @brief Receives multiple raw buffers asynchronously using an array of destination buffers.
 *
 * This function registers multiple raw buffers in the RX service queue.
 * Each buffer receives the exact number of bytes specified in the corresponding length entry.
 *
 * @param[out] Add_pu8BufferArr      Pointer to array of destination buffer pointers.
 * @param[in]  Add_pu16LengthArr     Pointer to array of receive lengths.
 * @param[in]  Copy_u8Count          Number of buffers to be queued.
 *
 * @retval OK            Buffers added to RX queue successfully.
 * @retval NOK           RX queue does not have enough space or service is busy.
 * @retval NULL_PTR      Input pointer is NULL.
 * @retval OUT_OF_RANGE  One or more lengths are invalid.
 *
 * @note All destination buffers must remain valid until reception is completed.
 */
error_t SUART_ReceiveRawBuffersAsync(u8 *Add_pu8BufferArr[], const u16 Add_pu16LengthArr[], u8 Copy_u8Count);

/* =========================================================
 * Service Notifications
 * ========================================================= */

/**
 * @brief Registers a callback function called when the TX service queue becomes empty.
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
 * This callback is invoked whenever one queued RX buffer is filled successfully,
 * either by receiving the configured terminator sequence in string mode
 * or by reaching the required length in raw mode.
 *
 * @param[in] Add_pfCallBack Pointer to callback function.
 *
 * @retval OK        Callback registered successfully.
 * @retval NULL_PTR  Input pointer is NULL.
 */
error_t SUART_RX_SetCallBack(SUART_Callback_t Add_pfCallBack);

#endif /* SUART_INT_H_ */