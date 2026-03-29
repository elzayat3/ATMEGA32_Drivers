#ifndef TIMER_CFG_H_
#define TIMER_CFG_H_

/**
 * @file TIMER_CFG.h
 * @author Abdelrahman Elzayat
 * @brief Timer configuration declarations for ATmega32 Timer driver
 *
 * This file contains external declarations for timer configuration
 * structures used by the driver. The actual definitions are located
 * in TIMER_Cfg.c.
 *
 * The configuration follows a pre-compile configuration approach,
 * where all timer parameters are defined statically before compilation.
 */

#include "TIMER_INT.h"

/* =========================================================
 *  External Configuration Structures
 * ========================================================= */

/**
 * @brief Timer0 configuration instance
 *
 * This structure holds all configuration parameters for Timer0,
 * including mode, prescaler, PWM behavior, and initial values.
 */
extern const TIMER0_Config_t TIMER0_CFG;

/**
 * @brief Timer1 configuration instance
 *
 * This structure holds all configuration parameters for Timer1,
 * including advanced PWM settings, TOP value, and compare channels.
 */
extern const TIMER1_Config_t TIMER1_CFG;

/**
 * @brief Timer2 configuration instance
 *
 * This structure holds all configuration parameters for Timer2,
 * similar to Timer0 but using Timer2 registers.
 */
extern const TIMER2_Config_t TIMER2_CFG;

#endif /* TIMER_CFG_H_ */