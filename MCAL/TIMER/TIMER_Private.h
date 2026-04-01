#ifndef TIMER_PRIVATE_H_
#define TIMER_PRIVATE_H_

/**
 * @file TIMER_Private.h
 * @author Abdelrahman Elzayat
 * @brief Private helper macros for the ATmega32 Timer driver.
 *
 * This file contains private macros used internally by the timer driver
 * to access timer counter, compare, capture, and control fields.
 *
 * @note This file is intended for internal driver use only.
 */

/* ========================================================================== */
/*                           Timer Counter Registers                          */
/* ========================================================================== */

#define TIMER0_SET_COUNTER(value)      (TCNT0 = (uint8_t)(value))
#define TIMER1_SET_COUNTER(value)      (TCNT1 = (uint16_t)(value))
#define TIMER2_SET_COUNTER(value)      (TCNT2 = (uint8_t)(value))

#define TIMER0_GET_COUNTER()           (TCNT0)
#define TIMER1_GET_COUNTER()           (TCNT1)
#define TIMER2_GET_COUNTER()           (TCNT2)

/* ========================================================================== */
/*                         Output Compare Registers                           */
/* ========================================================================== */

#define TIMER0_SET_COMPARE(value)      (OCR0 = (uint8_t)(value))
#define TIMER1_SET_COMPARE_A(value)    (OCR1A = (uint16_t)(value))
#define TIMER1_SET_COMPARE_B(value)    (OCR1B = (uint16_t)(value))
#define TIMER2_SET_COMPARE(value)      (OCR2 = (uint8_t)(value))

#define TIMER0_GET_COMPARE()           (OCR0)
#define TIMER1_GET_COMPARE_A()         (OCR1A)
#define TIMER1_GET_COMPARE_B()         (OCR1B)
#define TIMER2_GET_COMPARE()           (OCR2)

/* ========================================================================== */
/*                        Timer1 Capture Register                             */
/* ========================================================================== */

#define TIMER1_GET_ICR1()              (ICR1)
#define TIMER1_SET_ICR1(value)         (ICR1 = (uint16_t)(value))

/* ========================================================================== */
/*                               Bit Masks                                    */
/* ========================================================================== */

#define TIMER_CLOCK_SELECT_MASK        ((uint8_t)0x07U)

#define TIMER0_COM_MASK                ((uint8_t)((1U << COM00) | (1U << COM01)))
#define TIMER1_COM1A_MASK              ((uint8_t)((1U << COM1A0) | (1U << COM1A1)))
#define TIMER1_COM1B_MASK              ((uint8_t)((1U << COM1B0) | (1U << COM1B1)))
#define TIMER2_COM_MASK                ((uint8_t)((1U << COM20) | (1U << COM21)))

/* ========================================================================== */
/*                               Flags                                        */
/* ========================================================================== */

/**
 * @brief Clear Timer1 Input Capture flag.
 *
 * @note On AVR, the flag is cleared by writing logical one to ICF1.
 */
#define TIMER1_CLEAR_ICU_FLAG()        (SET_BIT(TIFR, ICF1))

#endif /* TIMER_PRIVATE_H_ */
