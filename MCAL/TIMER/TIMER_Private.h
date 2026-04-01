#ifndef TIMER_PRIVATE_H_
#define TIMER_PRIVATE_H_

/**
 * @file TIMER_Private.h
 * @author Abdelrahman Elzayat
 * @brief Private helper macros for the ATmega32 Timer driver.
 *
 * This file contains private macros used internally by the timer driver
 * to access timer counter, compare, and TOP registers for Timer0, Timer1,
 * and Timer2.
 *
 * @note This file is intended for internal driver use only.
 */

/* ========================================================================== */
/*                           Timer Counter Registers                          */
/* ========================================================================== */

/**
 * @brief Set Timer0 counter register value.
 *
 * This macro writes a value to the TCNT0 register.
 *
 * @param[in] value Value to be written to TCNT0.
 */
#define TIMER0_SET_COUNTER(value)      (TCNT0 = (uint8_t)(value))

/**
 * @brief Set Timer1 counter register value.
 *
 * This macro writes a value to the TCNT1 register.
 *
 * @param[in] value Value to be written to TCNT1.
 */
#define TIMER1_SET_COUNTER(value)      (TCNT1 = (uint16_t)(value))

/**
 * @brief Set Timer2 counter register value.
 *
 * This macro writes a value to the TCNT2 register.
 *
 * @param[in] value Value to be written to TCNT2.
 */
#define TIMER2_SET_COUNTER(value)      (TCNT2 = (uint8_t)(value))

/* ========================================================================== */
/*                         Output Compare Registers                           */
/* ========================================================================== */

/**
 * @brief Set Timer0 compare register value.
 *
 * This macro writes a value to the OCR0 register.
 *
 * @param[in] value Value to be written to OCR0.
 */
#define TIMER0_SET_COMPARE(value)      (OCR0 = (uint8_t)(value))

/**
 * @brief Set Timer1 Compare A register value.
 *
 * This macro writes a value to the OCR1A register.
 *
 * @param[in] value Value to be written to OCR1A.
 */
#define TIMER1_SET_COMPARE_A(value)    (OCR1A = (uint16_t)(value))

/**
 * @brief Set Timer1 Compare B register value.
 *
 * This macro writes a value to the OCR1B register.
 *
 * @param[in] value Value to be written to OCR1B.
 */
#define TIMER1_SET_COMPARE_B(value)    (OCR1B = (uint16_t)(value))

/**
 * @brief Set Timer2 compare register value.
 *
 * This macro writes a value to the OCR2 register.
 *
 * @param[in] value Value to be written to OCR2.
 */
#define TIMER2_SET_COMPARE(value)      (OCR2 = (uint8_t)(value))

/* ========================================================================== */
/*                              Read Registers                                */
/* ========================================================================== */

/**
 * @brief Get Timer0 counter register value.
 *
 * @return Current value of TCNT0.
 */
#define TIMER0_GET_COUNTER()           (TCNT0)

/**
 * @brief Get Timer1 counter register value.
 *
 * @return Current value of TCNT1.
 */
#define TIMER1_GET_COUNTER()           (TCNT1)

/**
 * @brief Get Timer2 counter register value.
 *
 * @return Current value of TCNT2.
 */
#define TIMER2_GET_COUNTER()           (TCNT2)

/**
 * @brief Get Timer0 compare register value.
 *
 * @return Current value of OCR0.
 */
#define TIMER0_GET_COMPARE()           (OCR0)

/**
 * @brief Get Timer1 Compare A register value.
 *
 * @return Current value of OCR1A.
 */
#define TIMER1_GET_COMPARE_A()         (OCR1A)

/**
 * @brief Get Timer1 Compare B register value.
 *
 * @return Current value of OCR1B.
 */
#define TIMER1_GET_COMPARE_B()         (OCR1B)

/**
 * @brief Get Timer2 compare register value.
 *
 * @return Current value of OCR2.
 */
#define TIMER2_GET_COMPARE()           (OCR2)

/* ========================================================================== */
/*                             Timer1 TOP Register                            */
/* ========================================================================== */

/**
 * @brief Get Timer1 Input Capture Register value.
 *
 * This macro reads the ICR1 register, which is used as TOP in Timer1 modes
 * that depend on a programmable top value.
 *
 * @return Current value of ICR1.
 */
#define TIMER1_GET_ICR1()              (ICR1)

/**
 * @brief Set Timer1 Input Capture Register value.
 *
 * This macro writes a value to the ICR1 register, which is used as TOP
 * in Timer1 modes that depend on a programmable top value.
 *
 * @param[in] value Value to be written to ICR1.
 */
#define TIMER1_SET_ICR1(value)         (ICR1 = (uint16_t)(value))

/**
 * @brief Mask for timer clock select bits.
 *
 * This mask is used to clear the clock source / prescaler selection bits
 * of Timer0, Timer1, and Timer2 before applying a new clock configuration.
 *
 * @note
 * - For Timer0, this mask applies to CS02:CS00 bits in TCCR0.
 * - For Timer1, this mask applies to CS12:CS10 bits in TCCR1B.
 * - For Timer2, this mask applies to CS22:CS20 bits in TCCR2.
 *
 * @author Abdelrahman Elzayat
 */
#define TIMER_CLOCK_SELECT_MASK   ((uint8_t)0x07U)
/**
 * @brief Clear Timer1 Input Capture flag.
 *
 * @note On AVR, the flag is cleared by writing logical one to ICF1.
 */
#define TIMER1_CLEAR_ICU_FLAG()        (SET_BIT(TIFR, ICF1))

#endif /* TIMER_PRIVATE_H_ */