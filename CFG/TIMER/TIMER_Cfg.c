#include "StdTypes.h"
#include "MemMap.h"
#include "TIMER_Int.h"
#include "TIMER_Private.h"
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
     * - NORMAL               → overflow-based timing
     * - CTC                  → interrupt on compare match
     * - FAST_PWM             → high-speed PWM
     * - PHASE_CORRECT_PWM    → symmetric PWM (less distortion)
     */

    /* =========================
     * Clock Prescaler
     * ========================= */
    .prescaler = TIMER_PRESCALER_8,
    /*
     * Timer frequency = F_CPU / prescaler
     * Example:
     * 8 MHz / 8 = 1 MHz timer clock
     */

    /* =========================
     *  Output Compare Mode (Non-PWM only)
     * ========================= */
    .oc_mode = TIMER_OC_DISCONNECTED,
    /*
     * Used only in NORMAL and CTC modes:
     * - DISCONNECTED → OC pin is normal GPIO
     * - TOGGLE       → toggle pin on compare match
     * - CLEAR        → set pin LOW on compare match
     * - SET          → set pin HIGH on compare match
     */

    /* =========================
     *  PWM Mode
     * ========================= */
    .pwm_mode = TIMER_PWM_NON_INVERTING,
    /*
     * NON-INVERTING:
     *   Output HIGH at start, LOW on compare match
     *   duty = OCR / 255
     *
     * INVERTING:
     *   Output LOW at start, HIGH on compare match
     *   duty = 1 - (OCR / 255)
     */

    /* =========================
     *  Initial Counter Value
     * ========================= */
    .initial_value = 0,
    /*
     * Initial value loaded into TCNT0
     * Usually set to 0
     * Can be increased to shorten overflow time
     */

    /* =========================
     *  Compare Value
     * ========================= */
    .compare_value = 128
    /*
     * In PWM:
     *   Duty cycle ≈ 128 / 255 ≈ 50%
     *
     * In CTC:
     *   Determines compare match timing
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
     * Recommended modes:
     * - FAST_PWM_ICR1 → user-defined TOP (best for control systems)
     * - CTC_OCR1A     → simple timing
     */

    /* =========================
     *  Clock Prescaler
     * ========================= */
    .prescaler = TIMER_PRESCALER_8,
    /*
     * Example:
     * 8 MHz / 8 = 1 MHz
     * Timer tick = 1 microseconds
     */

    /* =========================
     *  OC1A / OC1B Behavior
     * ========================= */
    .oc1a_mode = TIMER1_OC_DISCONNECTED,
    .oc1b_mode = TIMER1_OC_DISCONNECTED,
    /*
     * Used in non-PWM modes
     * In PWM, behavior is controlled by pwm_mode instead
     */

    /* =========================
     *  PWM Mode
     * ========================= */
    .pwm_mode = TIMER_PWM_NON_INVERTING,
    /*
     * Same concept as Timer0 PWM
     */

    /* =========================
     * Initial Counter Value
     * ========================= */
    .initial_value = 0,

    /* =========================
     * Compare Values
     * ========================= */
    .compare_A = 1500,
    .compare_B = 0,
    /*
     * OCR1A controls duty cycle
     * OCR1B optional (second channel)
     */

    /* =========================
     * TOP Value
     * ========================= */
    .top_value = 20000
    /*
     * PWM Period Calculation:
     *
     * period = TOP × timer_tick
     *
     * Example:
     * tick = 0.5 µs
     * TOP = 20000
     *
     * period = 20000 × 0.5µs = 10 ms
     *
     * For 20 ms (servo signal):
     * TOP = 40000
     */
};
/* =========================================================
 * 🔹 Timer2 Configuration (8-bit)
 * ========================================================= */
const TIMER2_Config_t TIMER2_CFG =
{
    /* =========================
     *  Mode Selection
     * ========================= */
    .mode = TIMER2_MODE_CTC,
    /*
     * Best for periodic interrupts
     */

    /* =========================
     *  Clock Prescaler
     * ========================= */
    .prescaler = TIMER_PRESCALER_8,
    /*
     * Slower timer → longer intervals
     */

    /* =========================
     *  Output Compare Mode
     * ========================= */
    .oc_mode = TIMER_OC_TOGGLE,
    /*
     * Toggle OC pin on compare match
     * Useful for generating square waves
     */

    /* =========================
     *  PWM Mode (Not used here)
     * ========================= */
    .pwm_mode = TIMER_PWM_NON_INVERTING,

    /* =========================
     *  Initial Value
     * ========================= */
    .initial_value = 0,

    /* =========================
     *  Compare Value
     * ========================= */
    .compare_value = 200
    /*
     * Determines interrupt frequency in CTC mode
     */
};
	