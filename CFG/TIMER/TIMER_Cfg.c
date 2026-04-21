#include "TIMER_Int.h"
#include "TIMER_Cfg.h"

/* ========================================================================== */
/*                          Timer0 Configuration                              */
/* ========================================================================== */

const TIMER0_Config_t TIMER0_CFG =
{
    .mode          = TIMER0_MODE_FAST_PWM,
    .prescaler     = TIMER_PRESCALER_8,
    .oc_mode       = TIMER_OC_DISCONNECTED,
    .pwm_mode      = TIMER_PWM_NON_INVERTING,
    .initial_value = 0U,
    .compare_value = 128U
};

/* ========================================================================== */
/*                          Timer1 Configuration                              */
/* ========================================================================== */

const TIMER1_Config_t TIMER1_CFG =
{
    .mode           = TIMER1_MODE_FAST_PWM_ICR1,
    .prescaler      = TIMER_PRESCALER_8,

    .oc1a_mode      = TIMER1_OC_DISCONNECTED,
    .oc1b_mode      = TIMER1_OC_DISCONNECTED,

    .oc1a_pwm_mode  = TIMER_PWM_NON_INVERTING,
    .oc1b_pwm_mode  = TIMER_PWM_NON_INVERTING,

    .initial_value  = 0U,

    .ocr1a_value    = 1500U,
    .ocr1b_value    = 1500U,
    .icr1_value     = 20000U
};

/* ========================================================================== */
/*                          Timer2 Configuration                              */
/* ========================================================================== */

const TIMER2_Config_t TIMER2_CFG =
{
    .mode          = TIMER2_MODE_CTC,
    .prescaler     = TIMER_PRESCALER_8,
    .oc_mode       = TIMER_OC_TOGGLE,
    .pwm_mode      = TIMER_PWM_NON_INVERTING,
    .initial_value = 0U,
    .compare_value = 200U
};
