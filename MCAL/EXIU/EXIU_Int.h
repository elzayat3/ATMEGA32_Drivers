/**

* @file    EXIU_INT.h
* @author  Abdelrahman Elzayat
* @brief   External Interrupt Unit (EXIU) driver interface for ATmega32.
*
* This driver provides APIs to configure and control external interrupts:
* INT0, INT1, and INT2. It allows enabling/disabling interrupts, selecting
* trigger conditions, and assigning callback functions executed on interrupt.
*
* @details
* External Interrupt Mapping:
* * EX_INT0 -> PD2
* * EX_INT1 -> PD3
* * EX_INT2 -> PB2
*
* @note
* * INT2 supports only edge-triggered modes (FALLING_EDGE / RISING_EDGE).
* * Callback functions are executed inside ISR context.
*
* @warning
* Global interrupt must be enabled using GLOBAL_ENABLE().
  */

#ifndef EXIU_INT_H_
#define EXIU_INT_H_

#include "StdTypes.h"

/* ======================= Types ======================= */

/**

* @brief External interrupt sources
  */
typedef enum
{
  EX_INT0 = 0, /**< External Interrupt 0 (PD2) */
  EX_INT1,     /**< External Interrupt 1 (PD3) */
  EX_INT2      /**< External Interrupt 2 (PB2) */
}Expin_t;

/**

* @brief External interrupt trigger conditions
  */
typedef enum
{
  LOW_LEVEL,         /**< Low level trigger (INT0, INT1 only) */
  ANY_LOGIC_CHANGE,  /**< Any logical change (INT0, INT1 only) */
  FALLING_EDGE,      /**< Falling edge trigger */
  RISING_EDGE        /**< Rising edge trigger */
}Trigger_t;

/**

* @brief External interrupt state
*
* @details
* Defines whether the interrupt is enabled or disabled during initialization.
  */
typedef enum
{
  EXTI_DISABLE = 0, /**< Interrupt disabled */
  EXTI_ENABLE       /**< Interrupt enabled  */
}EXTI_State_t;

/**

* @brief External interrupt configuration structure
*
* @details
* Used to configure each external interrupt during initialization.
*
* @note
* * INT0 and INT1 support all trigger modes.
* * INT2 supports only FALLING_EDGE and RISING_EDGE.
*
* @par Example
* @code
* EXTI_Config_t EXTI_ConfigArr[3] =
* {
* ```
  {EX_INT0, FALLING_EDGE, EXTI_ENABLE},
  ```
* ```
  {EX_INT1, RISING_EDGE,  EXTI_ENABLE},
  ```
* ```
  {EX_INT2, FALLING_EDGE, EXTI_DISABLE}
  ```
* };
* @endcode
  */
typedef struct
{
  Expin_t      pin;     /**< Interrupt source */
  Trigger_t    trigger; /**< Trigger condition */
  EXTI_State_t state;   /**< Enable/Disable state */
}EXTI_Config_t;

/* ======================= APIs ======================= */

/**

* @brief Initialize EXIU using configuration array
*
* @details
* Initializes all external interrupts based on EXTI_ConfigArr[].
*
* @pre DIO pins must be configured as input.
  */
  void EXIU_Init(void);

/**

* @brief Enable external interrupt
*
* @param[in] pin External interrupt source
  */
  void EXIU_enable(Expin_t pin);

/**

* @brief Disable external interrupt
*
* @param[in] pin External interrupt source
  */
  void EXIU_disable(Expin_t pin);

/**

* @brief Configure trigger condition
*
* @param[in] pin   External interrupt source
* @param[in] edge  Trigger condition
  */
  void EXIU_triggeredge(Expin_t pin, Trigger_t edge);

/**

* @brief Set callback function
*
* @param[in] pin   External interrupt source
* @param[in] local Pointer to callback function
*
* @note
* Callback runs inside ISR → keep it short and non-blocking.
  */
  void EXIU_SetCallback(Expin_t pin, void(*local)(void));

/* ======================= Global Interrupt ======================= */

/**

* @brief Enable global interrupts
  */
  #define GLOBAL_ENABLE()   sei()

/**

* @brief Disable global interrupts
  */
  #define GLOBAL_DISABLE()  cli()

#endif /* EXIU_INT_H_ */
