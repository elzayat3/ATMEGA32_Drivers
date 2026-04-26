#ifndef SPI_CFG_H_
#define SPI_CFG_H_

/**
 * @file SPI_Cfg.h
 * @brief Configuration interface for SPI driver on ATmega32.
 *
 * This file contains the external declaration of the SPI configuration
 * object used by SPI_Init().
 *
 * @author Abdelrahman Elzayat
 */

#include "SPI_Int.h"

/* ========================================================================== */
/*                         External Configuration                             */
/* ========================================================================== */

/**
 * @brief SPI configuration object.
 *
 * This object is defined in SPI_Cfg.c and used by SPI_Init().
 */
extern const SPI_Config_t SPI_Config;

#endif /* SPI_CFG_H_ */