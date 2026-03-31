#ifndef TIMER_INT_H_
#define TIMER_INT_H_

/**
 * @file TIMER_INT.h
 * @author Abdelrahman Elzayat
 * @brief Timer Driver Interface for ATmega32
 *
 * This file contains all public APIs, data types, and configuration
 * structures for Timer0, Timer1, and Timer2.
 */

#include "StdTypes.h"
/**
 * @defgroup TIMER_Pins Timer Output Compare Pins
 * @brief Hardware pins used by Timer Output Compare Units (OCU)
 *
 * These pins are automatically controlled by the timer hardware
 * when Output Compare or PWM modes are enabled.
 *
 * @note The corresponding pin must be configured as OUTPUT using DDR register.
 *
 * @{
 */

/**
 * @brief Timer0 Output Compare Pin
 *
 * - Signal: OC0
 * - Port: PORTB
 * - Pin:  PINB3
 *
 * @note Used for PWM and CTC toggle operations
 */
#define TIMER0_OC0_PIN   PINB3

/**
 * @brief Timer1 Output Compare A Pin
 *
 * - Signal: OC1A
 * - Port: PORTD
 * - Pin:  PIND5
 *
 * @note Used for PWM (channel A)
 */
#define TIMER1_OC1A_PIN  PIND5

/**
 * @brief Timer1 Output Compare B Pin
 *
 * - Signal: OC1B
 * - Port: PORTD
 * - Pin:  PIND4
 *
 * @note Used for PWM (channel B)
 */
#define TIMER1_OC1B_PIN  PIND4

/**
 * @brief Timer2 Output Compare Pin
 *
 * - Signal: OC2
 * - Port: PORTD
 * - Pin:  PIND7
 *
 * @note Used for PWM and toggle operations
 */
#define TIMER2_OC2_PIN   PIND7

/** @} */

/**
 * @brief Timer channel selection
 */
typedef enum
{
    TIMER0 = 0, /**< 8-bit Timer0 */
    TIMER1,     /**< 16-bit Timer1 */
    TIMER2      /**< 8-bit Timer2 */
} TIMER_Channel_t;

/**
 * @brief Timer clock prescaler options
 *
 * @note Values must match hardware CS bits
 */
typedef enum
{
    TIMER_NO_CLOCK = 0,       /**< Timer stopped */
    TIMER_PRESCALER_1,        /**< No prescaling */
    TIMER_PRESCALER_8,        /**< clk/8 */
    TIMER_PRESCALER_64,       /**< clk/64 */
    TIMER_PRESCALER_256,      /**< clk/256 */
    TIMER_PRESCALER_1024,     /**< clk/1024 */
    TIMER_EXTERNAL_FALLING,   /**< External clock (falling edge) */
    TIMER_EXTERNAL_RISING     /**< External clock (rising edge) */
} TIMER_Prescaler_t;


/**
 * @brief PWM output behavior
 */
typedef enum
{
    TIMER_PWM_NON_INVERTING, /**< Clear on compare, set at BOTTOM */
    TIMER_PWM_INVERTING      /**< Set on compare, clear at BOTTOM */
} TIMER_PWM_Mode_t;


/**
 * @brief Output Compare behavior for Normal/CTC modes
 */
typedef enum
{
    TIMER_OC_DISCONNECTED = 0, /**< Normal port operation */
    TIMER_OC_TOGGLE,           /**< Toggle OC pin on compare match */
    TIMER_OC_CLEAR,            /**< Clear OC pin on compare match */
    TIMER_OC_SET               /**< Set OC pin on compare match */
} TIMER_OC_Mode_t;

/**
 * @brief Timer0 operating modes
 */
typedef enum
{
    TIMER0_MODE_NORMAL,
    TIMER0_MODE_CTC,
    TIMER0_MODE_FAST_PWM,
    TIMER0_MODE_PHASE_CORRECT_PWM
} TIMER0_Mode_t;

/**
 * @brief Timer0 configuration structure
 */
typedef struct
{
    TIMER0_Mode_t mode;             /**< Timer mode */
    TIMER_Prescaler_t prescaler;    /**< Clock prescaler */

    TIMER_OC_Mode_t oc_mode;        /**< Used in Normal/CTC modes */
    TIMER_PWM_Mode_t pwm_mode;      /**< Used in PWM modes */

    uint8_t initial_value;          /**< TCNT0 initial value (0-255) */
    uint8_t compare_value;          /**< OCR0 value (0-255) */

} TIMER0_Config_t;



/**
 * @brief Timer2 operating modes
 */
typedef enum
{
    TIMER2_MODE_NORMAL,
    TIMER2_MODE_CTC,
    TIMER2_MODE_FAST_PWM,
    TIMER2_MODE_PHASE_CORRECT_PWM
} TIMER2_Mode_t;

/**
 * @brief Timer2 configuration structure
 */
typedef struct
{
    TIMER2_Mode_t mode;
    TIMER_Prescaler_t prescaler;

    TIMER_OC_Mode_t oc_mode;
    TIMER_PWM_Mode_t pwm_mode;

    uint8_t initial_value;   /**< 0-255 */
    uint8_t compare_value;   /**< 0-255 */

} TIMER2_Config_t;


/**
 * @brief Timer1 operating modes
 */
typedef enum
{
    TIMER1_MODE_NORMAL,
    TIMER1_MODE_CTC_OCR1A,
    TIMER1_MODE_CTC_ICR1,
    TIMER1_MODE_FAST_PWM_ICR1,
    TIMER1_MODE_FAST_PWM_OCR1A,
    TIMER1_MODE_PHASE_CORRECT_ICR1
} TIMER1_Mode_t;

/**
 * @brief Timer1 Output Compare behavior
 */
typedef enum
{
    TIMER1_OC_DISCONNECTED = 0,
    TIMER1_OC_TOGGLE,
    TIMER1_OC_CLEAR,
    TIMER1_OC_SET
} TIMER1_OC_Mode_t;

/**
 * @brief Timer1 configuration structure
 */
typedef struct
{
    TIMER1_Mode_t mode;             /**< Timer1 mode */
    TIMER_Prescaler_t prescaler;    /**< Clock prescaler */

    TIMER1_OC_Mode_t oc1a_mode;     /**< OC1A behavior */
    TIMER1_OC_Mode_t oc1b_mode;     /**< OC1B behavior */

    TIMER_PWM_Mode_t pwm_mode;      /**< PWM type */

    uint16_t initial_value;         /**< TCNT1 (0-65535) */

    uint16_t compare_A;             /**< OCR1A */
    uint16_t compare_B;             /**< OCR1B */

		/**
	 * @brief TOP value for Timer1
	 *
	 * Used as:
	 * - ICR1 in modes: FAST_PWM_ICR1 / PHASE_CORRECT_ICR1
	 * - OCR1A in modes: FAST_PWM_OCR1A
	 */
	uint16_t top_value;

} TIMER1_Config_t;

/**
 * @brief Timer interrupt sources
 */
typedef enum
{
    TIMER_INT_OVF,      /**< Overflow interrupt */
    TIMER_INT_COMP_A,   /**< Compare Match A */
    TIMER_INT_COMP_B,   /**< Compare Match B */
    TIMER_INT_ICU       /**< Input Capture Unit */
} TIMER_InterruptSource_t;


/* =========================================================
 *  APIs
 * ========================================================= */

/**
 * @brief Initialize all timers based on configuration
 *
 * @return error_t
 */
error_t TIMER_Init(void);

/**
 * @brief Initialize Timer0
 *
 * @return error_t
 */
error_t TIMER0_Init(void);

/**
 * @brief Initialize Timer1
 *
 * @return error_t
 */
error_t TIMER1_Init(void);

/**
 * @brief Initialize Timer2
 *
 * @return error_t
 */
error_t TIMER2_Init(void);
/* =========================
 * Timer0 API
 * ========================= */

/**
 * @brief Set Timer0 counter value (TCNT0)
 * @param[in] value Counter value (0-255)
 */
void TIMER0_SetCounter(uint8_t value);

/**
 * @brief Get Timer0 counter value (TCNT0)
 * @return Current TCNT0 value
 */
uint8_t TIMER0_GetCounter(void);

/**
 * @brief Set Timer0 compare value (OCR0)
 * @param[in] value Compare value (0-255)
 */
void TIMER0_SetCompare(uint8_t value);

/**
 * @brief Get Timer0 compare value (OCR0)
 * @return Current OCR0 value
 */
uint8_t TIMER0_GetCompare(void);

/* =========================
 * Timer1 API
 * ========================= */

/**
 * @brief Set Timer1 counter value (TCNT1)
 * @param[in] value Counter value (0-65535)
 */
void TIMER1_SetCounter(uint16_t value);

/**
 * @brief Get Timer1 counter value (TCNT1)
 * @return Current TCNT1 value
 */
uint16_t TIMER1_GetCounter(void);

/**
 * @brief Set Timer1 Compare A value (OCR1A)
 * @param[in] value Compare value (0-65535)
 */
void TIMER1_SetCompareA(uint16_t value);

/**
 * @brief Get Timer1 Compare A value (OCR1A)
 * @return Current OCR1A value
 */
uint16_t TIMER1_GetCompareA(void);

/**
 * @brief Set Timer1 Compare B value (OCR1B)
 * @param[in] value Compare value (0-65535)
 */
void TIMER1_SetCompareB(uint16_t value);

/**
 * @brief Get Timer1 Compare B value (OCR1B)
 * @return Current OCR1B value
 */
uint16_t TIMER1_GetCompareB(void);

/**
 * @brief Set Timer1 top value (ICR1)
 * @param[in] value Top value (0-65535)
 */
void TIMER1_SetTop(uint16_t value);

/**
 * @brief Get Timer1 top value (ICR1)
 * @return Current ICR1 value
 */
uint16_t TIMER1_GetTop(void);

/* =========================
 * Timer2 API
 * ========================= */

/**
 * @brief Set Timer2 counter value (TCNT2)
 * @param[in] value Counter value (0-255)
 */
void TIMER2_SetCounter(uint8_t value);

/**
 * @brief Get Timer2 counter value (TCNT2)
 * @return Current TCNT2 value
 */
uint8_t TIMER2_GetCounter(void);

/**
 * @brief Set Timer2 compare value (OCR2)
 * @param[in] value Compare value (0-255)
 */
void TIMER2_SetCompare(uint8_t value);

/**
 * @brief Get Timer2 compare value (OCR2)
 * @return Current OCR2 value
 */
uint8_t TIMER2_GetCompare(void);


#endif /* TIMER_INT_H_ */