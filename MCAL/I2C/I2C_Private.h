#ifndef TWI_PRIVATE_H_
#define TWI_PRIVATE_H_

/**
 * @file TWI_Private.h
 * @brief Private definitions for ATmega32 TWI (I2C) driver.
 *
 * This file contains:
 * - Internal macros
 * - Status codes
 * - Bit masks
 * - Internal helper definitions
 *
 * @author Abdelrahman Elzayat
 */

/********************************************************************/
/*                       Status Register Mask                       */
/********************************************************************/

/**
 * @brief Mask used to extract TWI status bits from TWSR register.
 *
 * Lower bits contain prescaler configuration and must be ignored.
 */
#define TWI_STATUS_MASK                 ((u8)0xF8u)

/********************************************************************/
/*                         TWI Status Codes                         */
/********************************************************************/

/**
 * @brief START condition transmitted successfully.
 */
#define TWI_START_STATUS                ((u8)0x08u)

/**
 * @brief Repeated START condition transmitted successfully.
 */
#define TWI_REPEATED_START_STATUS       ((u8)0x10u)

/**
 * @brief SLA+W transmitted and ACK received.
 */
#define TWI_MT_SLA_W_ACK_STATUS         ((u8)0x18u)

/**
 * @brief Data transmitted and ACK received.
 */
#define TWI_MT_DATA_ACK_STATUS          ((u8)0x28u)

/**
 * @brief SLA+R transmitted and ACK received.
 */
#define TWI_MR_SLA_R_ACK_STATUS         ((u8)0x40u)

/**
 * @brief Data received and ACK returned.
 */
#define TWI_MR_DATA_ACK_STATUS          ((u8)0x50u)

/**
 * @brief Data received and NACK returned.
 */
#define TWI_MR_DATA_NACK_STATUS         ((u8)0x58u)

/********************************************************************/
/*                       Address Configuration                      */
/********************************************************************/

/**
 * @brief Number of bits used to shift slave address.
 */
#define TWI_ADDRESS_SHIFT               ((u8)1u)

/**
 * @brief WRITE operation bit value.
 */
#define TWI_WRITE_BIT                   ((u8)0u)

/**
 * @brief READ operation bit value.
 */
#define TWI_READ_BIT                    ((u8)1u)

/**
 * @brief Maximum valid 7-bit slave address.
 */
#define TWI_MAX_7BIT_ADDRESS            ((u8)0x7Fu)

/********************************************************************/
/*                       Prescaler Bit Values                       */
/********************************************************************/

/**
 * @brief Prescaler bits value for division factor 1.
 */
#define TWI_PRESCALER_BITS_1            ((u8)0x00u)

/**
 * @brief Prescaler bits value for division factor 4.
 */
#define TWI_PRESCALER_BITS_4            ((u8)0x01u)

/**
 * @brief Prescaler bits value for division factor 16.
 */
#define TWI_PRESCALER_BITS_16           ((u8)0x02u)

/**
 * @brief Prescaler bits value for division factor 64.
 */
#define TWI_PRESCALER_BITS_64           ((u8)0x03u)

/**
 * @brief Mask for TWI prescaler bits inside TWSR register.
 */
#define TWI_PRESCALER_BITS_MASK         ((u8)0x03u)

/********************************************************************/
/*                      Clock Equation Constants                    */
/********************************************************************/

/**
 * @brief Constant value used in TWI clock equation.
 *
 * SCL = F_CPU / (16 + 2*TWBR*Prescaler)
 */
#define TWI_FREQ_BASE_VALUE             ((u32)16UL)

/**
 * @brief Multiplier value used in TWI clock equation.
 */
#define TWI_FREQ_MULTIPLIER_VALUE       ((u32)2UL)

/**
 * @brief Maximum valid value for 8-bit register.
 */
#define TWI_MAX_U8_VALUE                ((u32)255UL)

/********************************************************************/
/*                         Utility Macros                           */
/********************************************************************/

/**
 * @brief Generates bit mask for specific bit number.
 *
 * @param BIT_NUM Bit position.
 */
#define TWI_BIT_MASK(BIT_NUM)           ((u8)(1u << (BIT_NUM)))

/********************************************************************/
/*                       TWCR Command Values                        */
/********************************************************************/

/**
 * @brief Clears TWINT flag and enables TWI peripheral.
 */
#define TWI_CLEAR_FLAG_ENABLE           ((u8)( TWI_BIT_MASK(TWINT) | \
                                               TWI_BIT_MASK(TWEN) ))

/**
 * @brief Command used to transmit START condition.
 */
#define TWI_START_COMMAND               ((u8)( TWI_BIT_MASK(TWINT) | \
                                               TWI_BIT_MASK(TWSTA) | \
                                               TWI_BIT_MASK(TWEN) ))

/**
 * @brief Command used to transmit STOP condition.
 */
#define TWI_STOP_COMMAND                ((u8)( TWI_BIT_MASK(TWINT) | \
                                               TWI_BIT_MASK(TWSTO) | \
                                               TWI_BIT_MASK(TWEN) ))

/**
 * @brief Command used to receive data and return ACK.
 */
#define TWI_READ_ACK_COMMAND            ((u8)( TWI_BIT_MASK(TWINT) | \
                                               TWI_BIT_MASK(TWEA)  | \
                                               TWI_BIT_MASK(TWEN) ))

/**
 * @brief Command used to receive data and return NACK.
 */
#define TWI_READ_NACK_COMMAND           ((u8)( TWI_BIT_MASK(TWINT) | \
                                               TWI_BIT_MASK(TWEN) ))

/**
 * @brief Enables TWI peripheral.
 */
#define TWI_ENABLE_COMMAND              ((u8)( TWI_BIT_MASK(TWEN) ))

#endif /* TWI_PRIVATE_H_ */