#ifndef SPI_PRIVATE_H_
#define SPI_PRIVATE_H_

/**
 * @file SPI_Private.h
 * @brief Private definitions for SPI driver on ATmega32.
 *
 * This file contains only the private helper macros needed by SPI driver.
 *
 * @author Abdelrahman Elzayat
 */

#include "StdTypes.h"
#include "MemMap.h"
#include "SPI_Int.h"

/* ========================================================================== */
/*                              Private Values                                */
/* ========================================================================== */

/**
 * @brief Dummy byte used by master to generate SPI clock while receiving.
 */
#define SPI_DUMMY_BYTE              ((u8)0xFF)

/**
 * @brief Mask for SPI clock rate bits SPR0 and SPR1 in SPCR register.
 */
#define SPI_CLOCK_RATE_MASK         ((u8)((1u << SPR0) | (1u << SPR1)))

/**
 * @brief Mask for SPI double speed bit SPI2X in SPSR register.
 */
#define SPI_DOUBLE_SPEED_MASK       ((u8)(1u << SPI2X))

/* ========================================================================== */
/*                              Private Macros                                */
/* ========================================================================== */

/**
 * @brief Clears selected bits in a register using mask.
 */
#define SPI_CLR_MASK(REG, MASK)     ((REG) &= (u8)(~(MASK)))

/**
 * @brief Checks if SPI transfer is complete.
 */
#define SPI_IS_TRANSFER_DONE()      (GET_BIT(SPSR, SPIF))

/**
 * @brief Checks if SPI write collision occurred.
 */
#define SPI_IS_WRITE_COLLISION()    (GET_BIT(SPSR, WCOL))

/* ========================================================================== */
/*                         Configuration Validation                           */
/* ========================================================================== */

#define SPI_IS_VALID_MODE(MODE)             (((MODE) == SPI_SLAVE_MODE) || \
                                             ((MODE) == SPI_MASTER_MODE))

#define SPI_IS_VALID_DATA_ORDER(ORDER)      (((ORDER) == SPI_MSB_FIRST) || \
                                             ((ORDER) == SPI_LSB_FIRST))

#define SPI_IS_VALID_CLOCK_POLARITY(POL)    (((POL) == SPI_IDLE_LOW) || \
                                             ((POL) == SPI_IDLE_HIGH))

#define SPI_IS_VALID_CLOCK_PHASE(PHASE)     (((PHASE) == SPI_SAMPLE_LEADING) || \
                                             ((PHASE) == SPI_SAMPLE_TRAILING))

#define SPI_IS_VALID_CLOCK_RATE(RATE)       ((RATE) <= SPI_CLOCK_DIV_32)

#define SPI_IS_VALID_BOOL(STATE)            (((STATE) == FALSE) || \
                                             ((STATE) == TRUE))

#endif /* SPI_PRIVATE_H_ */