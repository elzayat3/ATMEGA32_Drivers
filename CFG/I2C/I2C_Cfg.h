#ifndef TWI_CFG_H_
#define TWI_CFG_H_

/**
 * @file TWI_Cfg.h
 * @brief Configuration file for ATmega32 TWI (I2C) driver.
 *
 * This file contains:
 * - TWI clock configuration
 * - Prescaler selection
 * - Timeout configuration
 *
 * @author Abdelrahman Elzayat
 */

/********************************************************************/
/*                       CPU Clock Frequency                        */
/********************************************************************/

/**
 * @brief CPU clock frequency in Hz.
 *
 * Common values:
 * 1000000UL
 * 8000000UL
 * 16000000UL
 */
#define TWI_CPU_FREQ_HZ                ((u32)8000000UL)

/********************************************************************/
/*                        SCL Clock Frequency                       */
/********************************************************************/

/**
 * @brief Desired TWI SCL frequency in Hz.
 *
 * Common values:
 * 100000UL  -> Standard Mode
 * 400000UL  -> Fast Mode
 */
#define TWI_SCL_FREQ_HZ                ((u32)100000UL)

/********************************************************************/
/*                         Prescaler Selection                      */
/********************************************************************/

/**
 * @brief TWI prescaler configuration.
 *
 * Available options:
 * TWI_PRESCALER_1
 * TWI_PRESCALER_4
 * TWI_PRESCALER_16
 * TWI_PRESCALER_64
 */
#define TWI_PRESCALER_CFG              TWI_PRESCALER_1

/********************************************************************/
/*                         Timeout Configuration                    */
/********************************************************************/

/**
 * @brief Timeout counter used in blocking wait loops.
 *
 * Increase this value for slower systems or debugging.
 */
#define TWI_TIMEOUT_COUNTS             ((u32)50000UL)

#endif /* TWI_CFG_H_ */