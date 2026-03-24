#include "StdTypes.h"
#include "MemMap.h"
#include "EXIU_Int.h"
#include "EXIU_Private.h"
#include "EXIU_Cfg.h"

EXTI_Config_t EXTI_ConfigArr[EXTI_CONFIG_SIZE] =
{
	/* pin      , trigger       , state         */
	{EX_INT0   , FALLING_EDGE  , EXTI_ENABLE  },
	{EX_INT1   , RISING_EDGE   , EXTI_ENABLE  },
	{EX_INT2   , FALLING_EDGE  , EXTI_DISABLE }
};
