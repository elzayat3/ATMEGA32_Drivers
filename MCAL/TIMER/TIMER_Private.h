
#ifndef TIMER_PRIVATE_H_
#define TIMER_PRIVATE_H_

/* =========================
 *  Timer Counter Registers
 * ========================= */

/** Set Timer0 counter value */
#define TIMER0_SET_COUNTER(value)     (TCNT0 = (uint8_t)(value))

/** Set Timer1 counter value */
#define TIMER1_SET_COUNTER(value)     (TCNT1 = (uint16_t)(value))

/** Set Timer2 counter value */
#define TIMER2_SET_COUNTER(value)     (TCNT2 = (uint8_t)(value))
/* =========================
 *  Output Compare Registers
 * ========================= */

/** Set Timer0 compare value */
#define TIMER0_SET_COMPARE(value)     (OCR0 = (uint8_t)(value))

/** Set Timer1 Compare A */
#define TIMER1_SET_COMPARE_A(value)   (OCR1A = (uint16_t)(value))

/** Set Timer1 Compare B */
#define TIMER1_SET_COMPARE_B(value)   (OCR1B = (uint16_t)(value))

/** Set Timer2 compare value */
#define TIMER2_SET_COMPARE(value)     (OCR2 = (uint8_t)(value))
/* =========================
 *  Read Registers
 * ========================= */

#define TIMER0_GET_COUNTER()          (TCNT0)
#define TIMER1_GET_COUNTER()          (TCNT1)
#define TIMER2_GET_COUNTER()          (TCNT2)

#define TIMER0_GET_COMPARE()          (OCR0)
#define TIMER1_GET_COMPARE_A()        (OCR1A)
#define TIMER1_GET_COMPARE_B()        (OCR1B)
#define TIMER2_GET_COMPARE()          (OCR2)

#define TIMER1_GET_ICR1()              (ICR1)

/* =========================
 * Timer1 TOP Register
 * ========================= */

/** Set Timer1 TOP value (ICR1) */
#define TIMER1_SET_ICR1(value)         (ICR1 = (uint16_t)(value))



#endif /* TIMER_PRIVATE_H_ */