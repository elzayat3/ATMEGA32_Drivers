#ifndef SUART_INT_H_
#define SUART_INT_H_

/**
 * @file SUART_Int.h
 * @brief Service UART (SUART) interface for ATmega32.
 *
 * This module provides a lightweight service layer over the UART MCAL driver.
 *
 * Features:
 * - Asynchronous transmission of null-terminated strings.
 * - TX queue support for multiple pending strings.
 * - Asynchronous reception into a user-provided buffer.
 * - TX/RX completion callbacks.
 *
 * RX Design:
 * - The user passes a buffer and its total size.
 * - The service reserves the last byte for the null terminator.
 * - Therefore, the number of received characters equals (BufferSize - 1).
 *
 * Example:
 * - If BufferSize = 10, the service receives 9 characters
 *   and keeps Buffer[9] = '\0'.
 *
 * @author Abdelrahman Elzayat
 */

#include "StdTypes.h"

/**
 * @brief Function pointer type for SUART callbacks.
 */
typedef void (*SUART_Callback_t)(void);

/**
 * @brief Initializes the SUART service.
 *
 * This function connects SUART internal handlers to the UART MCAL driver
 * and disables UART interrupts until needed.
 *
 * @retval OK   Initialization completed successfully.
 * @retval NOK  Initialization failed.
 */
error_t SUART_Init(void);

/**
 * @brief Queues a null-terminated string for asynchronous transmission.
 *
 * If the transmitter is idle, transmission starts automatically.
 * If the transmitter is busy, the string is stored in the TX queue and
 * transmitted later in FIFO order.
 *
 * @param[in] Data Pointer to a null-terminated string.
 *
 * @note The string is not copied internally.
 *       The user must keep it valid until transmission is completed.
 *
 * @retval OK        String accepted successfully.
 * @retval NULL_PTR  Data is NULLPTR.
 * @retval NOK       Input string is empty.
 * @retval FULL      TX queue is full.
 */
error_t SUART_SendAsync(const c8 *Data);

/**
 * @brief Starts asynchronous reception into a user buffer.
 *
 * The service receives (BufferSize - 1) characters and reserves the last
 * byte for the null terminator.
 *
 * @param[out] srt   Pointer to the destination buffer.
 * @param[in]  size  Total buffer size in bytes.
 *
 * @note The minimum valid size is 2 bytes:
 *       one byte for data and one byte for '\0'.
 *
 * @retval OK            RX request started successfully.
 * @retval NULL_PTR      Buffer pointer is NULLPTR.
 * @retval OUT_OF_RANGE  Buffer size is invalid.
 * @retval IN_PROGRESS   Another RX request is already active.
 */
error_t SUART_ReceiveAsync(c8 *srt, u8 size);

/**
 * @brief Registers a callback for TX completion.
 *
 * The callback is called when one queued string finishes transmission.
 *
 * @param[in] Add_pfCallBack Pointer to callback function.
 *
 * @retval OK        Callback registered successfully.
 * @retval NULL_PTR  Add_pfCallBack is NULLPTR.
 */
error_t SUART_TX_SetCallBack(SUART_Callback_t Add_pfCallBack);

/**
 * @brief Registers a callback for RX completion.
 *
 * The callback is called when the configured receive operation is completed.
 *
 * @param[in] Add_pfCallBack Pointer to callback function.
 *
 * @retval OK        Callback registered successfully.
 * @retval NULL_PTR  Add_pfCallBack is NULLPTR.
 */
error_t SUART_RX_SetCallBack(SUART_Callback_t Add_pfCallBack);

#endif /* SUART_INT_H_ */