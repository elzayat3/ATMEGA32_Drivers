/**

* @file    EXIU_Cfg.h
* @author  Abdelrahman Elzayat
* @brief   Configuration header for EXIU driver (ATmega32).
*
* This file contains user-configurable parameters for the External Interrupt
* Unit (EXIU) driver, including the configuration array size and external
* configuration declaration.
*
* @note
* The actual configuration is defined in EXIU_Cfg.c.
  */

#ifndef EXIU_CFG_H_
#define EXIU_CFG_H_

#include "EXIU_Int.h"

/**

* @brief Number of configured external interrupts
*
* @details
* Defines the size of the EXTI configuration array.
* This value should match the number of elements defined in EXIU_Cfg.c.
  */
  #define EXTI_CONFIG_SIZE   3

/**

* @brief External Interrupt configuration array
*
* @details
* This array is defined in EXIU_Cfg.c and used by EXIU_Init()
* to initialize all configured external interrupts.
  */
  extern EXTI_Config_t EXTI_ConfigArr[EXTI_CONFIG_SIZE];

#endif /* EXIU_CFG_H_ */
