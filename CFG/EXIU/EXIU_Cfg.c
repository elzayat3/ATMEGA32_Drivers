#include "StdTypes.h"
#include "MemMap.h"
#include "EXIU_Int.h"
#include "EXIU_Private.h"
#include "EXIU_Cfg.h"

/**

* @brief External Interrupt configuration array
*
* @details
* This array defines the configuration for all external interrupts (INT0, INT1, INT2).
* Each element specifies:
* * The interrupt source (pin)
* * The trigger condition
* * The enable/disable state
*
* @note
* * The size of this array must match EXTI_CONFIG_SIZE.
* * INT2 supports only FALLING_EDGE and RISING_EDGE.
*
* @par Example:
* @code
* {EX_INT0, FALLING_EDGE, EXTI_ENABLE}
* @endcode
  */
EXTI_Config_t EXTI_ConfigArr[EXTI_CONFIG_SIZE] =
{
  /* pin      , trigger       , state         */
  {EX_INT0   , FALLING_EDGE  , EXTI_ENABLE  },
  {EX_INT1   , RISING_EDGE   , EXTI_ENABLE  },
  {EX_INT2   , FALLING_EDGE  , EXTI_DISABLE }
 };
