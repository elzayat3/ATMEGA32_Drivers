/**
 * @file SPI_Cfg.c
 * @brief Configuration source file for SPI driver on ATmega32.
 *
 * This file contains the user configuration for the SPI peripheral.
 *
 * @author Abdelrahman Elzayat
 */

#include "SPI_Cfg.h"

/* ========================================================================== */
/*                           SPI Configuration                                */
/* ========================================================================== */

/**
 * @brief SPI configuration object.
 *
 * Default configuration:
 * - Master mode
 * - MSB first
 * - Clock idle low
 * - Sample on leading edge
 * - SPI clock = F_CPU / 16
 * - SPI interrupt disabled
 *
 * @note In master mode, SS pin should be configured as output
 *       to prevent hardware from clearing the MSTR bit automatically.
 */
const SPI_Config_t SPI_Config =
{
    .mode = SPI_MASTER_MODE,
    .data_order = SPI_MSB_FIRST,
    .clock_polarity = SPI_IDLE_LOW,
    .clock_phase = SPI_SAMPLE_LEADING,
    .clock_rate = SPI_CLOCK_DIV_16,
    .interrupt_enable = FALSE
};