#include "SUART_Cfg.h"

/**
 * @file SUART_Cfg.c
 * @brief Configuration source file for the UART service layer on ATmega32.
 *
 * This file contains the configured TX and RX terminator sequences
 * used by the UART service layer.
 *
 * @author Abdelrahman Elzayat
 */

const u8 SUART_TxTerminator[SUART_TX_TERMINATOR_LEN] = {'\n'};
const u8 SUART_RxTerminator[SUART_RX_TERMINATOR_LEN] = {'\n'};