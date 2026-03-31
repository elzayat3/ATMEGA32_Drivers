#include "StdTypes.h"
#include "MemMap.h"
#include "TIMER_Private.h"
#include "TIMER_Int.h"
#include "TIMER_Cfg.h"

/* =========================================================
 *  Timer0 Configuration (8-bit)
 * ========================================================= */
const TIMER0_Config_t TIMER0_CFG =
{
    /* =========================
     *  Mode Selection
     * ========================= */
    .mode = TIMER0_MODE_FAST_PWM,
    /*
     * Options:
     * - TIMER0_MODE_NORMAL
     * - TIMER0_MODE_CTC
     * - TIMER0_MODE_FAST_PWM
     * - TIMER0_MODE_PHASE_CORRECT_PWM
     */

    /* =========================
     *  Clock Prescaler
     * ========================= */
    .prescaler = TIMER_PRESCALER_8,
    /*
     * Timer frequency = F_CPU / prescaler
     */

    /* =========================
     *  Output Compare Mode (Non-PWM only)
     * ========================= */
    .oc_mode = TIMER_OC_DISCONNECTED,
    /*
     * Used only in NORMAL and CTC modes:
     * - TIMER_OC_DISCONNECTED
     * - TIMER_OC_TOGGLE
     * - TIMER_OC_CLEAR
     * - TIMER_OC_SET
     */

    /* =========================
     *  PWM Mode
     * ========================= */
    .pwm_mode = TIMER_PWM_NON_INVERTING,
    /*
     * Used only in PWM modes:
     * - TIMER_PWM_NON_INVERTING
     * - TIMER_PWM_INVERTING
     */

    /* =========================
     *  Initial Counter Value
     * ========================= */
    .initial_value = 0U,

    /* =========================
     *  Compare Value
     * ========================= */
    .compare_value = 128U
    /*
     * In PWM:
     * Duty cycle ~= OCR0 / 255
     *
     * In CTC:
     * Determines compare match timing
     */
};

/* =========================================================
 *  Timer1 Configuration (16-bit)
 * ========================================================= */
const TIMER1_Config_t TIMER1_CFG =
{
    /* =========================
     *  Mode Selection
     * ========================= */
    .mode = TIMER1_MODE_FAST_PWM_ICR1,
    /*
     * Options:
     * - TIMER1_MODE_NORMAL
     * - TIMER1_MODE_CTC_OCR1A
     * - TIMER1_MODE_CTC_ICR1
     * - TIMER1_MODE_FAST_PWM_ICR1
     * - TIMER1_MODE_FAST_PWM_OCR1A
     * - TIMER1_MODE_PHASE_CORRECT_ICR1
     */

    /* =========================
     *  Clock Prescaler
     * ========================= */
    .prescaler = TIMER_PRESCALER_8,
    /*
     * Example:
     * If F_CPU = 8 MHz, timer tick = 1 us
     */

    /* =========================
     *  OC1A / OC1B Behavior in Non-PWM Modes
     * ========================= */
    .oc1a_mode = TIMER1_OC_DISCONNECTED,
    .oc1b_mode = TIMER1_OC_DISCONNECTED,
    /*
     * Used only in Normal / CTC modes:
     * - TIMER1_OC_DISCONNECTED
     * - TIMER1_OC_TOGGLE
     * - TIMER1_OC_CLEAR
     * - TIMER1_OC_SET
     */

    /* =========================
     *  OC1A / OC1B Behavior in PWM Modes
     * ========================= */
    .oc1a_pwm_mode = TIMER_PWM_NON_INVERTING,
    .oc1b_pwm_mode = TIMER_PWM_NON_INVERTING,
    /*
     * Each channel now has its own PWM mode:
     * - TIMER_PWM_NON_INVERTING
     * - TIMER_PWM_INVERTING
     */

    /* =========================
     *  Initial Counter Value
     * ========================= */
    .initial_value = 0U,

    /* =========================
     *  Register Values
     * ========================= */
    .ocr1a_value = 1500U,
    .ocr1b_value = 0U,
    .icr1_value  = 20000U
    /*
     * Register meanings depend on selected mode:
     *
     * In TIMER1_MODE_FAST_PWM_ICR1:
     * - ICR1  = TOP
     * - OCR1A = duty / compare value for channel A
     * - OCR1B = duty / compare value for channel B
     *
     * In TIMER1_MODE_FAST_PWM_OCR1A:
     * - OCR1A = TOP
     * - OCR1B = duty / compare value for channel B
     * - ICR1  = not used as TOP
     *
     * In TIMER1_MODE_CTC_OCR1A:
     * - OCR1A = TOP / compare match reference
     *
     * In TIMER1_MODE_CTC_ICR1:
     * - ICR1 = TOP / compare match reference
     */
};

/* =========================================================
 *  Timer2 Configuration (8-bit)
 * ========================================================= */
const TIMER2_Config_t TIMER2_CFG =
{
    /* =========================
     *  Mode Selection
     * ========================= */
    .mode = TIMER2_MODE_CTC,
    /*
     * Options:
     * - TIMER2_MODE_NORMAL
     * - TIMER2_MODE_CTC
     * - TIMER2_MODE_FAST_PWM
     * - TIMER2_MODE_PHASE_CORRECT_PWM
     */

    /* =========================
     *  Clock Prescaler
     * ========================= */
    .prescaler = TIMER_PRESCALER_8,

    /* =========================
     *  Output Compare Mode (Non-PWM only)
     * ========================= */
    .oc_mode = TIMER_OC_TOGGLE,
    /*
     * Used only in NORMAL and CTC modes
     */

    /* =========================
     *  PWM Mode
     * ========================= */
    .pwm_mode = TIMER_PWM_NON_INVERTING,
    /*
     * Used only in PWM modes
     */

    /* =========================
     *  Initial Value
     * ========================= */
    .initial_value = 0U,

    /* =========================
     *  Compare Value
     * ========================= */
    .compare_value = 200U
};