#ifndef SPI_INT_H_
#define SPI_INT_H_

/**
 * @file SPI_Int.h
 * @brief Serial Peripheral Interface (SPI) driver interface for ATmega32.
 *
 * This module provides APIs to initialize and use the SPI peripheral
 * in master or slave mode.
 *
 * The SPI peripheral is full-duplex, meaning each transfer sends and
 * receives data at the same time.
 *
 * @author Abdelrahman Elzayat
 */

#include "StdTypes.h"

/* ========================================================================== */
/*                              Type Definitions                              */
/* ========================================================================== */

/**
 * @enum SPI_Mode_t
 * @brief Defines SPI operating mode.
 */
typedef enum
{
    SPI_SLAVE_MODE = 0,    /**< SPI works as slave device */
    SPI_MASTER_MODE        /**< SPI works as master device */
} SPI_Mode_t;

/**
 * @enum SPI_DataOrder_t
 * @brief Defines SPI data transmission order.
 */
typedef enum
{
    SPI_MSB_FIRST = 0,     /**< Most significant bit is transmitted first */
    SPI_LSB_FIRST          /**< Least significant bit is transmitted first */
} SPI_DataOrder_t;

/**
 * @enum SPI_ClockPolarity_t
 * @brief Defines SPI clock polarity.
 */
typedef enum
{
    SPI_IDLE_LOW = 0,      /**< SCK is low when idle */
    SPI_IDLE_HIGH          /**< SCK is high when idle */
} SPI_ClockPolarity_t;

/**
 * @enum SPI_ClockPhase_t
 * @brief Defines SPI clock phase.
 */
typedef enum
{
    SPI_SAMPLE_LEADING = 0,    /**< Data is sampled on the leading clock edge */
    SPI_SAMPLE_TRAILING        /**< Data is sampled on the trailing clock edge */
} SPI_ClockPhase_t;

/**
 * @enum SPI_ClockRate_t
 * @brief Defines SPI clock rate in master mode.
 *
 * @note Clock rate selection is only effective when SPI works in master mode.
 */
typedef enum
{
    SPI_CLOCK_DIV_4 = 0,    /**< SPI clock = F_CPU / 4 */
    SPI_CLOCK_DIV_16,       /**< SPI clock = F_CPU / 16 */
    SPI_CLOCK_DIV_64,       /**< SPI clock = F_CPU / 64 */
    SPI_CLOCK_DIV_128,      /**< SPI clock = F_CPU / 128 */
    SPI_CLOCK_DIV_2,        /**< SPI clock = F_CPU / 2 */
    SPI_CLOCK_DIV_8,        /**< SPI clock = F_CPU / 8 */
    SPI_CLOCK_DIV_32        /**< SPI clock = F_CPU / 32 */
} SPI_ClockRate_t;

/**
 * @typedef SPI_Callback_t
 * @brief Pointer to function used as SPI interrupt callback.
 */
typedef void (*SPI_Callback_t)(void);

/**
 * @struct SPI_Config_t
 * @brief SPI configuration structure.
 */
typedef struct
{
    SPI_Mode_t mode;                     /**< Master or slave mode */
    SPI_DataOrder_t data_order;           /**< MSB first or LSB first */
    SPI_ClockPolarity_t clock_polarity;   /**< Clock idle state */
    SPI_ClockPhase_t clock_phase;         /**< Sampling edge */
    SPI_ClockRate_t clock_rate;           /**< SPI clock rate in master mode */
    bool_t interrupt_enable;              /**< Enable or disable SPI interrupt */
} SPI_Config_t;

/* ========================================================================== */
/*                              Public APIs                                   */
/* ========================================================================== */

/**
 * @brief Initializes the SPI peripheral using the configuration structure.
 *
 * This function configures:
 * - SPI mode
 * - Data order
 * - Clock polarity
 * - Clock phase
 * - Clock rate
 * - Interrupt state
 *
 * @note SPI pins direction should be configured using DIO driver before
 *       calling this function.
 *
 * @return error_t
 * @retval OK   SPI initialized successfully.
 * @retval NOK  Invalid configuration.
 */
error_t SPI_Init(void);
/**
 * @brief Enables the SPI peripheral.
 *
 * This function sets the SPE bit to enable SPI operations.
 *
 * @return void
 */
void SPI_Enable(void);

/**
 * @brief Disables the SPI peripheral.
 *
 * This function clears the SPE bit to disable SPI operations.
 *
 * @return void
 */
void SPI_Disable(void);

/**
 * @brief Sends one byte through SPI using blocking mode.
 *
 * This function waits until the SPI transfer is complete.
 *
 * @param data Byte to be transmitted.
 *
 * @return error_t
 * @retval OK   Byte transmitted successfully.
 */
error_t SPI_SendByte(uint8_t data);

/**
 * @brief Receives one byte through SPI using blocking mode.
 *
 * In master mode, this function sends a dummy byte to generate the SPI clock.
 *
 * @param data Pointer to store the received byte.
 *
 * @return error_t
 * @retval OK        Byte received successfully.
 * @retval NULL_PTR  Null pointer is passed.
 */
error_t SPI_ReceiveByte(uint8_t *data);

/**
 * @brief Sends and receives one byte through SPI using blocking mode.
 *
 * SPI is full-duplex, so each transfer transmits and receives data
 * at the same time.
 *
 * @param tx_data Byte to be transmitted.
 * @param rx_data Pointer to store the received byte.
 *
 * @return error_t
 * @retval OK        Transfer completed successfully.
 * @retval NULL_PTR  Null pointer is passed.
 */
error_t SPI_TransceiveByte(uint8_t tx_data, uint8_t *rx_data);

/**
 * @brief Sends one byte through SPI using non-blocking polling mode.
 *
 * This function starts SPI transmission if SPI is idle and returns immediately.
 * The function must be called repeatedly until it returns OK.
 *
 * On the first call, the byte is written to SPDR and the function returns
 * IN_PROGRESS because the transfer has started but is not completed yet.
 *
 * When the transfer is completed, the function reads SPDR to clear the SPIF flag
 * and returns OK.
 *
 * @note The data parameter is used only when a new transfer is started.
 *       While SPI is busy, the passed data value is ignored.
 *
 * @note This function should not be used together with SPI interrupt mode,
 *       because the ISR may clear the busy state before this function reads SPDR.
 *
 * @param data Byte to be transmitted when SPI is idle.
 *
 * @return error_t
 * @retval OK           Transfer completed successfully.
 * @retval IN_PROGRESS  Transfer has started or is still in progress.
 */
error_t SPI_SendByteNoBlock(uint8_t data);

/**
 * @brief Receives one byte through SPI using non-blocking mode.
 *
 * This function reads received data only if transfer is complete.
 *
 * @param data Pointer to store the received byte.
 *
 * @return error_t
 * @retval OK           Data received successfully.
 * @retval NULL_PTR     Null pointer is passed.
 * @retval IN_PROGRESS  Transfer is not complete yet.
 */
error_t SPI_ReceiveByteNoBlock(uint8_t *data);

/**
 * @brief Sends and receives one byte through SPI using non-blocking mode.
 *
 * This function starts a transfer if SPI is ready and returns immediately.
 *
 * @param tx_data Byte to be transmitted.
 * @param rx_data Pointer to store the received byte if transfer is complete.
 *
 * @return error_t
 * @retval OK           Transfer completed successfully.
 * @retval NULL_PTR     Null pointer is passed.
 * @retval IN_PROGRESS  Transfer is still in progress.
 */
error_t SPI_TransceiveByteNoBlock(uint8_t tx_data, uint8_t *rx_data);

/**
 * @brief Enables SPI interrupt.
 *
 * This function sets the SPIE bit in SPCR register.
 *
 * @note This function only enables SPI interrupt from the peripheral side.
 *       Global interrupt must be enabled separately using sei().
 *
 * @return void
 */
void SPI_InterruptEnable(void);

/**
 * @brief Disables SPI interrupt.
 *
 * This function clears the SPIE bit in SPCR register.
 *
 * @return void
 */
void SPI_InterruptDisable(void);

/**
 * @brief Sets callback function for SPI interrupt.
 *
 * The callback function is executed inside the SPI interrupt service routine
 * when SPI transfer is complete.
 *
 * @param callback Pointer to callback function.
 *
 * @return error_t
 * @retval OK        Callback registered successfully.
 * @retval NULL_PTR  Null pointer is passed.
 */
error_t SPI_SetCallBack(SPI_Callback_t callback);

#endif /* SPI_INT_H_ */