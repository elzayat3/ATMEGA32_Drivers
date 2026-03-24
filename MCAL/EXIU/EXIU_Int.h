/**

* @file    EXIU_INT.h
* @author  Abdelrahman Elzayat
* @brief   External Interrupt Unit (EXIU) driver interface for ATmega32.
*
* This driver provides APIs to configure and control external interrupts:
* INT0, INT1, and INT2. It allows enabling/disabling interrupts, selecting
* trigger conditions, and assigning callback functions to be executed
* when an interrupt occurs.
*
* @details
* External Interrupt Mapping:
* * EX_INT0 -> PIND2
* * EX_INT1 -> PIND3
* * EX_INT2 -> PINB2
*
* @note
* * INT2 supports only edge-triggered modes (FALLING_EDGE / RISING_EDGE).
* * Callback functions are executed inside ISR context.
*
* @warning
* Global interrupt must be enabled using GLOBAL_ENABLE() for interrupts to work.
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
  EX_INT0=0,  /**< External Interrupt 0 (PIND2) */
  EX_INT1,  /**< External Interrupt 1 (PIND3) */
  EX_INT2   /**< External Interrupt 2 (PINB2) */
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

* @brief External Interrupt state configuration
*
* @details
* This enum is used to define whether a specific external interrupt
* is enabled or disabled during initialization.
  */
typedef enum
{
  EXTI_DISABLE = 0, /**< External interrupt is disabled */
  EXTI_ENABLE       /**< External interrupt is enabled  */
}EXTI_State_t;

/**

* @brief External Interrupt configuration structure
*
* @details
* This structure is used to configure each external interrupt (INT0, INT1, INT2)
* during initialization phase.
*
* It defines:
* * Which interrupt pin to configure
* * The trigger condition
* * Whether the interrupt is enabled or disabled
*
* @note
* * INT0 and INT1 support all trigger modes.
* * INT2 supports only FALLING_EDGE and RISING_EDGE.
*
* @par Example usage:
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
  Expin_t      pin;     /**< External interrupt source (INT0, INT1, INT2) */
  Trigger_t    trigger; /**< Trigger condition */
  EXTI_State_t state;   /**< Enable/Disable state */
}EXTI_Config_t;

/* ======================= APIs ======================= */

/**

* @brief Initialize EXIU with default configuration
*
* @details
* This function sets the default trigger condition for all external interrupts.
* Current default:
* * INT0 -> FALLING_EDGE
* * INT1 -> FALLING_EDGE
* * INT2 -> FALLING_EDGE
*
* @pre DIO pins must be configured as input before calling this function.
  */
void EXIU_Init(void);

/**

* @brief Enable external interrupt
*
* @param[in] pin External interrupt source (EX_INT0, EX_INT1, EX_INT2)
*
* @post The selected interrupt will start responding to trigger events.
  */
void EXIU_enable(Expin_t pin);

/**

* @brief Disable external interrupt
*
* @param[in] pin External interrupt source
*
* @post The selected interrupt will no longer generate interrupts.
  */
void EXIU_disable(Expin_t pin);

/**

* @brief Configure trigger condition for external interrupt
*
* @param[in] pin   External interrupt source
* @param[in] edge  Trigger condition
*
* @note
* * INT0 and INT1 support all trigger modes.
* * INT2 supports only FALLING_EDGE and RISING_EDGE.
    */
void EXIU_triggeredge(Expin_t pin, Trigger_t edge);

/**

* @brief Set callback function for external interrupt
*
* @param[in] pin   External interrupt source
* @param[in] local Pointer to callback function
*
* @note
* * Callback is executed inside ISR context.
* * Keep the callback short and non-blocking.
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
