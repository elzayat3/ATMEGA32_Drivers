#ifndef TWI_INT_H_
#define TWI_INT_H_

/**
 * @file TWI_Int.h
 * @brief Public interface for ATmega32 TWI (I2C) driver.
 *
 * This file contains:
 * - Public APIs
 * - Public enums
 * - User accessible declarations
 *
 * @author Abdelrahman Elzayat
 */

#include "StdTypes.h"

/**
 * @enum TWI_Prescaler_t
 * @brief TWI clock prescaler options.
 *
 * These values represent the division factor used
 * in the TWI clock equation.
 */
typedef enum
{
    TWI_PRESCALER_1  = 1,
    TWI_PRESCALER_4  = 4,
    TWI_PRESCALER_16 = 16,
    TWI_PRESCALER_64 = 64

} TWI_Prescaler_t;

/**
 * @brief Initializes TWI peripheral in Master mode.
 *
 * This function configures:
 * - TWI clock frequency
 * - Prescaler value
 * - TWBR register
 * - Enables TWI peripheral
 *
 * @retval OK             Initialization completed successfully.
 * @retval OUT_OF_RANGE   Invalid configuration detected.
 */
error_t TWI_Init(void);

/**
 * @brief Sends START condition on TWI bus.
 *
 * This function initiates communication on the bus.
 *
 * @retval OK        START condition transmitted successfully.
 * @retval NOK       START condition failed.
 * @retval TIMEOUT   Timeout waiting for TWINT flag.
 */
error_t TWI_StartCondition(void);

/**
 * @brief Sends Repeated START condition on TWI bus.
 *
 * This function is used without releasing the bus
 * between write/read operations.
 *
 * @retval OK        Repeated START transmitted successfully.
 * @retval NOK       Repeated START failed.
 * @retval TIMEOUT   Timeout waiting for TWINT flag.
 */
error_t TWI_RepeatedStart(void);

/**
 * @brief Sends STOP condition on TWI bus.
 *
 * This function terminates communication and releases the bus.
 *
 * @retval OK        STOP condition transmitted successfully.
 * @retval TIMEOUT   Timeout waiting for STOP completion.
 */
error_t TWI_StopCondition(void);

/**
 * @brief Sends slave address with WRITE request.
 *
 * The driver automatically appends the WRITE bit.
 *
 * @param copy_u8SlaveAddress  7-bit slave address.
 *
 * @retval OK             Address transmitted and ACK received.
 * @retval NOK            Address transmission failed.
 * @retval OUT_OF_RANGE   Invalid slave address.
 * @retval TIMEOUT        Timeout waiting for TWINT flag.
 */
error_t TWI_SendSlaveAddressWrite(u8 copy_u8SlaveAddress);

/**
 * @brief Sends slave address with READ request.
 *
 * The driver automatically appends the READ bit.
 *
 * @param copy_u8SlaveAddress  7-bit slave address.
 *
 * @retval OK             Address transmitted and ACK received.
 * @retval NOK            Address transmission failed.
 * @retval OUT_OF_RANGE   Invalid slave address.
 * @retval TIMEOUT        Timeout waiting for TWINT flag.
 */
error_t TWI_SendSlaveAddressRead(u8 copy_u8SlaveAddress);

/**
 * @brief Sends one data byte as Master transmitter.
 *
 * @param copy_u8Data  Data byte to transmit.
 *
 * @retval OK        Data transmitted successfully.
 * @retval NOK       Data transmission failed.
 * @retval TIMEOUT   Timeout waiting for TWINT flag.
 */
error_t TWI_MasterSendData(u8 copy_u8Data);

/**
 * @brief Receives one data byte and returns ACK.
 *
 * ACK indicates that the master expects more data bytes.
 *
 * @param ptr_u8Data  Pointer to store received data.
 *
 * @retval OK         Data received successfully.
 * @retval NOK        Data reception failed.
 * @retval NULL_PTR   Null pointer passed.
 * @retval TIMEOUT    Timeout waiting for TWINT flag.
 */
error_t TWI_MasterReceiveDataAck(u8 *ptr_u8Data);

/**
 * @brief Receives one data byte and returns NACK.
 *
 * NACK indicates that this is the last byte to receive.
 *
 * @param ptr_u8Data  Pointer to store received data.
 *
 * @retval OK         Data received successfully.
 * @retval NOK        Data reception failed.
 * @retval NULL_PTR   Null pointer passed.
 * @retval TIMEOUT    Timeout waiting for TWINT flag.
 */
error_t TWI_MasterReceiveDataNack(u8 *ptr_u8Data);

/**
 * @brief Gets current masked TWI status code.
 *
 * This function returns the current TWI status
 * after masking prescaler bits.
 *
 * @return Current TWI status code.
 */
u8 TWI_GetStatus(void);

/**
 * @brief Writes one byte to a slave device.
 *
 * Internal sequence:
 * START -> SLA+W -> DATA -> STOP
 *
 * @param copy_u8SlaveAddress  7-bit slave address.
 * @param copy_u8Data          Data byte to transmit.
 *
 * @retval OK             Byte written successfully.
 * @retval NOK            Write operation failed.
 * @retval OUT_OF_RANGE   Invalid slave address.
 * @retval TIMEOUT        Timeout occurred.
 */
error_t TWI_MasterWriteByte(u8 copy_u8SlaveAddress, u8 copy_u8Data);

/**
 * @brief Reads one byte from a slave device.
 *
 * Internal sequence:
 * START -> SLA+R -> READ + NACK -> STOP
 *
 * @param copy_u8SlaveAddress  7-bit slave address.
 * @param ptr_u8Data           Pointer to store received data.
 *
 * @retval OK             Byte read successfully.
 * @retval NOK            Read operation failed.
 * @retval NULL_PTR       Null pointer passed.
 * @retval OUT_OF_RANGE   Invalid slave address.
 * @retval TIMEOUT        Timeout occurred.
 */
error_t TWI_MasterReadByte(u8 copy_u8SlaveAddress, u8 *ptr_u8Data);

/**
 * @brief Writes multiple bytes to a slave device.
 *
 * Internal sequence:
 * START -> SLA+W -> DATA0 -> DATA1 -> ... -> STOP
 *
 * @param copy_u8SlaveAddress  7-bit slave address.
 * @param ptr_u8Buffer         Pointer to transmit buffer.
 * @param copy_u8Size          Number of bytes to transmit.
 *
 * @retval OK             Buffer written successfully.
 * @retval NOK            Write operation failed.
 * @retval NULL_PTR       Null pointer passed.
 * @retval OUT_OF_RANGE   Invalid parameters.
 * @retval TIMEOUT        Timeout occurred.
 */
error_t TWI_MasterWriteBuffer(u8 copy_u8SlaveAddress,
                              const u8 *ptr_u8Buffer,
                              u8 copy_u8Size);

/**
 * @brief Reads multiple bytes from a slave device.
 *
 * Internal sequence:
 * START -> SLA+R -> READ + ACK ... READ + NACK -> STOP
 *
 * @param copy_u8SlaveAddress  7-bit slave address.
 * @param ptr_u8Buffer         Pointer to receive buffer.
 * @param copy_u8Size          Number of bytes to receive.
 *
 * @retval OK             Buffer read successfully.
 * @retval NOK            Read operation failed.
 * @retval NULL_PTR       Null pointer passed.
 * @retval OUT_OF_RANGE   Invalid parameters.
 * @retval TIMEOUT        Timeout occurred.
 */
error_t TWI_MasterReadBuffer(u8 copy_u8SlaveAddress,
                             u8 *ptr_u8Buffer,
                             u8 copy_u8Size);

#endif /* TWI_INT_H_ */