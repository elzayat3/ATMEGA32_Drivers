#ifndef TIMER_CFG_H_
#define TIMER_CFG_H_

/**
 * @file TIMER_Cfg.h
 * @author Abdelrahman Elzayat
 * @brief Timer configuration declarations for the ATmega32 Timer driver.
 *
 * This file contains external declarations for timer configuration
 * structures used by the driver. The actual definitions are located
 * in TIMER_Cfg.c.
 *
 * The driver follows a pre-compile configuration approach, where all
 * timer parameters are defined statically before compilation.
 */

#include "TIMER_Int.h"

/* ========================================================================== */
/*                      External Configuration Declarations                    */
/* ========================================================================== */

extern const TIMER0_Config_t TIMER0_CFG; /**< Timer0 configuration instance. */
extern const TIMER1_Config_t TIMER1_CFG; /**< Timer1 configuration instance. */
extern const TIMER2_Config_t TIMER2_CFG; /**< Timer2 configuration instance. */

#endif /* TIMER_CFG_H_ */
