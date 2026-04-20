#ifndef UART_INT_H_
#define UART_INT_H_

/**
 * @file UART_Int.h
 * @brief Public interface for the UART driver on ATmega32.
 *
 * This file contains:
 * - UART public APIs
 * - UART public configuration enums
 * - UART configuration structure
 * - UART callback function type
 *
 * The actual configuration object can be defined later in UART_Cfg.c.
 *
 * @author Abdelrahman Elzayat
 */

#include "StdTypes.h"

/**
 * @typedef UART_Callback_t
 * @brief Pointer to function used as UART interrupt callback.
 */
typedef void (*UART_Callback_t)(void);

/**
 * @enum UART_Mode_t
 * @brief UART communication mode selection.
 */
typedef enum
{
    UART_ASYNCHRONOUS_MODE = 0, /**< Asynchronous UART mode */
    UART_SYNCHRONOUS_MODE       /**< Synchronous UART mode   */
} UART_Mode_t;

/**
 * @enum UART_Speed_t
 * @brief UART speed mode selection in asynchronous mode.
 */
typedef enum
{
    UART_NORMAL_SPEED = 0, /**< Normal speed mode (U2X = 0) */
    UART_DOUBLE_SPEED      /**< Double speed mode (U2X = 1) */
} UART_Speed_t;

/**
 * @enum UART_Parity_t
 * @brief UART parity configuration.
 */
typedef enum
{
    UART_PARITY_DISABLED = 0, /**< Parity disabled */
    UART_PARITY_EVEN=2,         /**< Even parity     */
    UART_PARITY_ODD           /**< Odd parity      */
} UART_Parity_t;

/**
 * @enum UART_StopBits_t
 * @brief UART stop bit configuration.
 */
typedef enum
{
    UART_ONE_STOP_BIT = 0, /**< One stop bit */
    UART_TWO_STOP_BITS     /**< Two stop bits */
} UART_StopBits_t;

/**
 * @enum UART_CharSize_t
 * @brief UART character size configuration.
 */
typedef enum
{
    UART_5_BIT_CHAR = 0, /**< 5-bit data frame */
    UART_6_BIT_CHAR,     /**< 6-bit data frame */
    UART_7_BIT_CHAR,     /**< 7-bit data frame */
    UART_8_BIT_CHAR,     /**< 8-bit data frame */
    UART_9_BIT_CHAR=7      /**< 9-bit data frame */
} UART_CharSize_t;

/**
 * @enum UART_BaudRate_t
 * @brief Precomputed UBRR values for UART baud rates at 8 MHz CPU clock.
 *
 * These values are valid only when the UART is configured as:
 * - Asynchronous mode
 * - Normal speed mode (U2X = 0)
 * - F_CPU = 8 MHz
 *
 * Each enum value represents the value to be written to the UBRR register
 * (UBRRL/UBRRH) in order to generate the required baud rate.
 *
 * @warning These values are NOT valid if:
 * - The CPU clock frequency is changed
 * - Double speed mode (U2X = 1) is used
 * - A different UART operating mode is selected
 *
 * @note If the driver is later required to support other CPU frequencies
 *       or double-speed mode, these values must be recalculated.
 *
 * @note High baud rates may introduce higher communication error percentages.
 *       For 8 MHz normal speed mode, 9600, 19200, and 38400 are generally
 *       more reliable than higher baud rates such as 57600 or 115200.
 */
typedef enum
{
    UART_BAUD_2400   = 207, /**< UBRR value for 2400 bps  at 8 MHz, normal speed */
    UART_BAUD_4800   = 103, /**< UBRR value for 4800 bps  at 8 MHz, normal speed */
    UART_BAUD_9600   = 51,  /**< UBRR value for 9600 bps  at 8 MHz, normal speed */
    UART_BAUD_14400  = 34,  /**< UBRR value for 14400 bps at 8 MHz, normal speed */
    UART_BAUD_19200  = 25,  /**< UBRR value for 19200 bps at 8 MHz, normal speed */
    UART_BAUD_28800  = 16,  /**< UBRR value for 28800 bps at 8 MHz, normal speed */
    UART_BAUD_38400  = 12,  /**< UBRR value for 38400 bps at 8 MHz, normal speed */
    UART_BAUD_57600  = 8,   /**< UBRR value for 57600 bps at 8 MHz, normal speed */
    UART_BAUD_76800  = 6,   /**< UBRR value for 76800 bps at 8 MHz, normal speed */
    UART_BAUD_115200 = 3    /**< UBRR value for 115200 bps at 8 MHz, normal speed */
} UART_BaudRate_t;

/**
 * @struct UART_Config_t
 * @brief UART configuration structure.
 *
 * This structure contains all configurable UART parameters that are used
 * during UART initialization.
 */
typedef struct
{
    UART_Mode_t mode;             /**< UART mode selection */
    UART_Speed_t speed;           /**< UART speed selection */
    UART_Parity_t parity;         /**< UART parity mode */
    UART_StopBits_t stop_bits;    /**< Number of stop bits */
    UART_CharSize_t char_size;    /**< Character size */
    UART_BaudRate_t baud_rate;    /**< Baud rate selection */
    bool_t tx_enable;             /**< Enable transmitter */
    bool_t rx_enable;             /**< Enable receiver */
} UART_Config_t;

/**
 * @brief Initializes the UART peripheral.
 *
 * This function configures the UART peripheral according to the selected
 * configuration object.
 *
 * @note If UART_BaudRate_t enum values are used directly as UBRR values,
 *       the selected baud rate values are assumed to be precomputed for
 *       8 MHz CPU clock, asynchronous mode, and normal speed mode (U2X = 0).
 *
 * @warning Changing F_CPU or enabling double speed mode requires
 *          recalculating the baud rate register value.
 */
void UART_Init(void);

/**
 * @brief Sends one byte using blocking mode.
 *
 * This function waits until the UART data register becomes empty,
 * then writes the given byte to the UART data register.
 *
 * @param[in] Copy_u8Data Byte to be transmitted.
 *
 * @return void
 */
void UART_Send(u8 Copy_u8Data);

/**
 * @brief Receives one byte using blocking mode.
 *
 * This function waits until a new byte is received, then returns it.
 *
 * @return u8 Received byte.
 */
u8 UART_Receive(void);

/**
 * @brief Sends one byte using non-blocking mode.
 *
 * This function checks if the UART data register is ready.
 * If ready, the byte is transmitted immediately.
 *
 * @param[in] Copy_u8Data Byte to be transmitted.
 *
 * @retval OK   Byte transmitted successfully.
 * @retval NOK  UART data register is not ready.
 */
error_t UART_SendNoBlock(u8 Copy_u8Data);

/**
 * @brief Receives one byte using non-blocking mode.
 *
 * This function checks if a new byte has been received.
 * If available, the received byte is stored through the pointer.
 *
 * @param[out] Add_pu8Data Pointer to store the received byte.
 *
 * @retval OK    Data received successfully.
 * @retval NOK   No new data is available.
 * @retval NULL  Input pointer is NULL.
 */
error_t UART_ReceiveNoBlock(u8 *Add_pu8Data);

/**
 * @brief Enables UART RX Complete interrupt.
 *
 * @return void
 */
void UART_RX_InterruptEnable(void);

/**
 * @brief Disables UART RX Complete interrupt.
 *
 * @return void
 */
void UART_RX_InterruptDisable(void);

/**
 * @brief Enables UART TX Complete interrupt.
 *
 * @return void
 */
void UART_TX_InterruptEnable(void);

/**
 * @brief Disables UART TX Complete interrupt.
 *
 * @return void
 */
void UART_TX_InterruptDisable(void);

/**
 * @brief Enables UART Data Register Empty interrupt.
 *
 * @return void
 */
void UART_UDRE_InterruptEnable(void);

/**
 * @brief Disables UART Data Register Empty interrupt.
 *
 * @return void
 */
void UART_UDRE_InterruptDisable(void);

/**
 * @brief Registers a callback function for UART RX interrupt.
 *
 * @param[in] Add_pfCallBack Pointer to callback function.
 *
 * @retval OK    Callback registered successfully.
 * @retval NULL  Input pointer is NULL.
 */
error_t UART_RX_SetCallBack(UART_Callback_t Add_pfCallBack);

/**
 * @brief Registers a callback function for UART TX interrupt.
 *
 * @param[in] Add_pfCallBack Pointer to callback function.
 *
 * @retval OK    Callback registered successfully.
 * @retval NULL  Input pointer is NULL.
 */
error_t UART_TX_SetCallBack(UART_Callback_t Add_pfCallBack);

/**
 * @brief Registers a callback function for UART UDRE interrupt.
 *
 * @param[in] Add_pfCallBack Pointer to callback function.
 *
 * @retval OK    Callback registered successfully.
 * @retval NULL  Input pointer is NULL.
 */
error_t UART_UDRE_SetCallBack(UART_Callback_t Add_pfCallBack);
/**
 * @brief Sends one byte directly to the UART data register.
 *
 * This function writes the given byte directly to UDR without checking
 * whether the transmit data register is empty.
 *
 * @param[in] Copy_u8Data Byte to be transmitted.
 *
 * @return void
 *
 * @note This function should only be used when the UART transmitter is
 *       already ready to accept a new byte.
 *
 * @warning This function does not check the UDRE flag before writing.
 *          Calling it when UDR is not ready may overwrite pending data.
 *
 * @warning This function is mainly intended for controlled use cases,
 *          such as interrupt-driven transmission inside UART_UDRE_vect.
 */
void UART_SendDirect(u8 Copy_u8Data);

/**
 * @brief Receives one byte directly from the UART data register.
 *
 * This function reads the received byte directly from UDR without checking
 * whether new data is available.
 *
 * @return u8 Received byte.
 *
 * @note This function should only be used when received data is already
 *       available in the UART receive buffer.
 *
 * @warning This function does not check the RXC flag before reading.
 *          Calling it when no new data is available may return invalid data.
 *
 * @warning This function is mainly intended for controlled use cases,
 *          such as interrupt-driven reception inside UART_RX_vect callback.
 */
u8 UART_ReceiveDirect(void);

#endif /* UART_INT_H_ */