#ifndef TIMER_INT_H_
#define TIMER_INT_H_

/**
 * @file TIMER_Int.h
 * @author Abdelrahman Elzayat
 * @brief Public interface for the ATmega32 Timer driver.
 *
 * This file contains the public data types, configuration structures,
 * symbolic constants, and API declarations for Timer0, Timer1, and Timer2
 * on the ATmega32 microcontroller.
 */

#include "StdTypes.h"

/* ========================================================================== */
/*                            Output Compare Pins                             */
/* ========================================================================== */

/**
 * @defgroup TIMER_OutputComparePins Timer Output Compare Pins
 * @brief Hardware pins associated with timer output compare units.
 *
 * These pins are driven by the timer hardware when Output Compare or PWM
 * functionality is enabled.
 *
 * @note The corresponding GPIO pin must be configured as output before use.
 * @{
 */
#define TIMER0_OC0_PIN   PINB3 /**< Timer0 Output Compare pin. */
#define TIMER1_OC1A_PIN  PIND5 /**< Timer1 Output Compare A pin. */
#define TIMER1_OC1B_PIN  PIND4 /**< Timer1 Output Compare B pin. */
#define TIMER2_OC2_PIN   PIND7 /**< Timer2 Output Compare pin. */
/** @} */

/* ========================================================================== */
/*                               Common Types                                 */
/* ========================================================================== */

/**
 * @brief Timer channel identifiers.
 */
typedef enum
{
    TIMER0 = 0, /**< Select Timer0. */
    TIMER1,     /**< Select Timer1. */
    TIMER2      /**< Select Timer2. */
} TIMER_Channel_t;

/**
 * @brief Timer clock source and prescaler options.
 *
 * These values map directly to the clock select bits of the timer hardware.
 */
typedef enum
{
    TIMER_NO_CLOCK = 0,     /**< No clock source; timer stopped. */
    TIMER_PRESCALER_1,      /**< System clock with no prescaling. */
    TIMER_PRESCALER_8,      /**< System clock divided by 8. */
    TIMER_PRESCALER_64,     /**< System clock divided by 64. */
    TIMER_PRESCALER_256,    /**< System clock divided by 256. */
    TIMER_PRESCALER_1024,   /**< System clock divided by 1024. */
    TIMER_EXTERNAL_FALLING, /**< External clock source on falling edge. */
    TIMER_EXTERNAL_RISING   /**< External clock source on rising edge. */
} TIMER_Prescaler_t;

/**
 * @brief PWM output behavior.
 */
typedef enum
{
    TIMER_PWM_NON_INVERTING = 0, /**< Non-inverting PWM mode. */
    TIMER_PWM_INVERTING          /**< Inverting PWM mode. */
} TIMER_PWM_Mode_t;

/**
 * @brief Output Compare behavior for Timer0 and Timer2 in non-PWM modes.
 */
typedef enum
{
    TIMER_OC_DISCONNECTED = 0, /**< Disconnect OC pin from timer compare output. */
    TIMER_OC_TOGGLE,           /**< Toggle OC pin on compare match. */
    TIMER_OC_CLEAR,            /**< Clear OC pin on compare match. */
    TIMER_OC_SET               /**< Set OC pin on compare match. */
} TIMER_OC_Mode_t;

/* ========================================================================== */
/*                                Timer0 Types                                */
/* ========================================================================== */

/**
 * @brief Timer0 operating modes.
 */
typedef enum
{
    TIMER0_MODE_NORMAL = 0,       /**< Normal counting mode. */
    TIMER0_MODE_CTC,              /**< Clear Timer on Compare Match mode. */
    TIMER0_MODE_FAST_PWM,         /**< Fast PWM mode. */
    TIMER0_MODE_PHASE_CORRECT_PWM /**< Phase Correct PWM mode. */
} TIMER0_Mode_t;

/**
 * @brief Timer0 configuration structure.
 */
typedef struct
{
    TIMER0_Mode_t mode;          /**< Timer0 operating mode. */
    TIMER_Prescaler_t prescaler; /**< Timer0 clock source / prescaler. */
    TIMER_OC_Mode_t oc_mode;     /**< Output Compare behavior in Normal/CTC modes. */
    TIMER_PWM_Mode_t pwm_mode;   /**< PWM output behavior in PWM modes. */
    uint8_t initial_value;       /**< Initial value loaded into TCNT0. */
    uint8_t compare_value;       /**< Value loaded into OCR0. */
} TIMER0_Config_t;

/* ========================================================================== */
/*                                Timer2 Types                                */
/* ========================================================================== */

/**
 * @brief Timer2 operating modes.
 */
typedef enum
{
    TIMER2_MODE_NORMAL = 0,       /**< Normal counting mode. */
    TIMER2_MODE_CTC,              /**< Clear Timer on Compare Match mode. */
    TIMER2_MODE_FAST_PWM,         /**< Fast PWM mode. */
    TIMER2_MODE_PHASE_CORRECT_PWM /**< Phase Correct PWM mode. */
} TIMER2_Mode_t;

/**
 * @brief Timer2 configuration structure.
 */
typedef struct
{
    TIMER2_Mode_t mode;          /**< Timer2 operating mode. */
    TIMER_Prescaler_t prescaler; /**< Timer2 clock source / prescaler. */
    TIMER_OC_Mode_t oc_mode;     /**< Output Compare behavior in Normal/CTC modes. */
    TIMER_PWM_Mode_t pwm_mode;   /**< PWM output behavior in PWM modes. */
    uint8_t initial_value;       /**< Initial value loaded into TCNT2. */
    uint8_t compare_value;       /**< Value loaded into OCR2. */
} TIMER2_Config_t;

/* ========================================================================== */
/*                                Timer1 Types                                */
/* ========================================================================== */

/**
 * @brief Timer1 operating modes.
 */
typedef enum
{
    TIMER1_MODE_NORMAL = 0,    /**< Normal counting mode. */
    TIMER1_MODE_CTC_OCR1A,     /**< CTC mode with OCR1A as TOP. */
    TIMER1_MODE_CTC_ICR1,      /**< CTC mode with ICR1 as TOP. */
    TIMER1_MODE_FAST_PWM_ICR1, /**< Fast PWM mode with ICR1 as TOP. */
    TIMER1_MODE_FAST_PWM_OCR1A, /**< Fast PWM mode with OCR1A as TOP. */
    TIMER1_MODE_PHASE_CORRECT_ICR1 /**< Phase Correct PWM mode with ICR1 as TOP. */
} TIMER1_Mode_t;

/**
 * @brief Output Compare behavior for Timer1 channels in non-PWM modes.
 */
typedef enum
{
    TIMER1_OC_DISCONNECTED = 0, /**< Disconnect OC1x pin from compare unit. */
    TIMER1_OC_TOGGLE,           /**< Toggle OC1x pin on compare match. */
    TIMER1_OC_CLEAR,            /**< Clear OC1x pin on compare match. */
    TIMER1_OC_SET               /**< Set OC1x pin on compare match. */
} TIMER1_OC_Mode_t;

/**
 * @brief Timer1 configuration structure.
 *
 * This structure stores the values that will be written to Timer1 registers.
 * The meaning of OCR1A and ICR1 depends on the selected Timer1 operating mode.
 */
typedef struct
{
    TIMER1_Mode_t mode;                  /**< Timer1 operating mode. */
    TIMER_Prescaler_t prescaler;         /**< Timer1 clock source / prescaler. */

    TIMER1_OC_Mode_t oc1a_mode;          /**< OC1A behavior in non-PWM modes. */
    TIMER1_OC_Mode_t oc1b_mode;          /**< OC1B behavior in non-PWM modes. */

    TIMER_PWM_Mode_t oc1a_pwm_mode;      /**< OC1A behavior in PWM modes. */
    TIMER_PWM_Mode_t oc1b_pwm_mode;      /**< OC1B behavior in PWM modes. */

    uint16_t initial_value;              /**< Initial value loaded into TCNT1. */

    uint16_t ocr1a_value;                /**< Value loaded into OCR1A. */
    uint16_t ocr1b_value;                /**< Value loaded into OCR1B. */
    uint16_t icr1_value;                 /**< Value loaded into ICR1. */
} TIMER1_Config_t;

/* ========================================================================== */
/*                             Interrupt Sources                              */
/* ========================================================================== */

/**
 * @brief Timer interrupt source identifiers.
 */
typedef enum
{
    TIMER_INT_OVF = 0, /**< Overflow interrupt source. */
    TIMER_INT_COMP_A,  /**< Compare Match A interrupt source. */
    TIMER_INT_COMP_B,  /**< Compare Match B interrupt source. */
    TIMER_INT_ICU      /**< Input Capture Unit interrupt source. */
} TIMER_InterruptSource_t;

/**
 * @brief Timer callback function pointer type.
 */
typedef void (*TIMER_Callback_t)(void);

/**
 * @brief Timer1 Input Capture edge selection.
 */
typedef enum
{
    TIMER1_ICU_FALLING_EDGE = 0, /**< Capture on falling edge. */
    TIMER1_ICU_RISING_EDGE       /**< Capture on rising edge. */
} TIMER1_ICU_Edge_t;

/* ========================================================================== */
/*                               Initialization                               */
/* ========================================================================== */

/**
 * @brief Initialize all timer modules according to their configuration.
 *
 * This function initializes Timer0, Timer1, and Timer2 using the configuration
 * settings defined in the driver configuration source.
 *
 * @return error_t
 *         - OK  : All timers initialized successfully.
 *         - NOK : One or more timers failed to initialize.
 */
error_t TIMER_Init(void);

/**
 * @brief Initialize Timer0 according to its configuration.
 *
 * This function configures Timer0 registers and loads initial values,
 * but does not start the timer clock. The timer remains stopped until
 * TIMER_Start() is called.
 *
 * @return error_t
 *         - OK           : Timer0 initialized successfully.
 *         - OUT_OF_RANGE : Invalid Timer0 configuration.
 */
error_t TIMER0_Init(void);

/**
 * @brief Initialize Timer1 according to its configuration.
 *
 * This function configures Timer1 registers and loads initial values,
 * but does not start the timer clock. The timer remains stopped until
 * TIMER_Start() is called.
 *
 * @return error_t
 *         - OK           : Timer1 initialized successfully.
 *         - OUT_OF_RANGE : Invalid Timer1 configuration.
 */
error_t TIMER1_Init(void);

/**
 * @brief Initialize Timer2 according to its configuration.
 *
 * This function configures Timer2 registers and loads initial values,
 * but does not start the timer clock. The timer remains stopped until
 * TIMER_Start() is called.
 *
 * @return error_t
 *         - OK           : Timer2 initialized successfully.
 *         - OUT_OF_RANGE : Invalid Timer2 configuration.
 */
error_t TIMER2_Init(void);

/* ========================================================================== */
/*                                Timer0 API                                  */
/* ========================================================================== */

void TIMER0_SetCounter(uint8_t value);
uint8_t TIMER0_GetCounter(void);
void TIMER0_SetCompare(uint8_t value);
uint8_t TIMER0_GetCompare(void);

/* ========================================================================== */
/*                                Timer1 API                                  */
/* ========================================================================== */

void TIMER1_SetCounter(uint16_t value);
uint16_t TIMER1_GetCounter(void);

/**
 * @brief Set Timer1 Compare A register value.
 *
 * @note In TIMER1_MODE_CTC_OCR1A and TIMER1_MODE_FAST_PWM_OCR1A, OCR1A is used
 *       as TOP. Calling this function in these modes changes the top value.
 *
 * @param[in] value Value to be written to OCR1A.
 */
void TIMER1_SetCompareA(uint16_t value);

/**
 * @brief Get Timer1 Compare A register value.
 *
 * @note In TIMER1_MODE_CTC_OCR1A and TIMER1_MODE_FAST_PWM_OCR1A, OCR1A is used
 *       as TOP. The returned value therefore represents the top value.
 *
 * @return uint16_t Current OCR1A value.
 */
uint16_t TIMER1_GetCompareA(void);

void TIMER1_SetCompareB(uint16_t value);
uint16_t TIMER1_GetCompareB(void);

/**
 * @brief Set Timer1 top value according to the configured mode.
 *
 * This function updates the Timer1 top value used by modes that support
 * a programmable top. The value is written to OCR1A or ICR1 depending
 * on the current Timer1 mode configuration.
 *
 * @param[in] value Top value to be configured.
 */
void TIMER1_SetTop(uint16_t value);

/**
 * @brief Get Timer1 top value according to the configured mode.
 *
 * This function returns the current Timer1 top value from OCR1A or ICR1
 * depending on the current Timer1 mode configuration.
 *
 * @return uint16_t Current Timer1 top value.
 */
uint16_t TIMER1_GetTop(void);

/* ========================================================================== */
/*                                Timer2 API                                  */
/* ========================================================================== */

void TIMER2_SetCounter(uint8_t value);
uint8_t TIMER2_GetCounter(void);
void TIMER2_SetCompare(uint8_t value);
uint8_t TIMER2_GetCompare(void);

/* ========================================================================== */
/*                            Timer Control API                               */
/* ========================================================================== */

/**
 * @brief Start the selected timer.
 *
 * This function starts the specified timer by restoring its configured clock
 * source and prescaler settings.
 *
 * @param[in] timer Timer channel to start.
 *
 * @return error_t
 *         - OK           : Timer started successfully.
 *         - OUT_OF_RANGE : Invalid timer channel.
 */
error_t TIMER_Start(TIMER_Channel_t timer);

/**
 * @brief Stop the selected timer.
 *
 * This function stops the specified timer by clearing its clock select bits,
 * which disconnects the timer clock source.
 *
 * @param[in] timer Timer channel to stop.
 *
 * @return error_t
 *         - OK           : Timer stopped successfully.
 *         - OUT_OF_RANGE : Invalid timer channel.
 */
error_t TIMER_Stop(TIMER_Channel_t timer);

/* ========================================================================== */
/*                           Interrupt Management                             */
/* ========================================================================== */

/**
 * @brief Enable the specified timer interrupt source.
 *
 * This function enables the selected interrupt source for the specified timer
 * by setting the corresponding interrupt enable bit in TIMSK.
 *
 * @param[in] timer  Timer channel to configure.
 * @param[in] source Interrupt source to enable.
 *
 * @return error_t
 *         - OK           : Interrupt enabled successfully.
 *         - OUT_OF_RANGE : Invalid timer channel or unsupported interrupt source
 *                          for the selected timer.
 */
error_t TIMER_InterruptEnable(TIMER_Channel_t timer, TIMER_InterruptSource_t source);

/**
 * @brief Disable the specified timer interrupt source.
 *
 * This function disables the selected interrupt source for the specified timer
 * by clearing the corresponding interrupt enable bit in TIMSK.
 *
 * @param[in] timer  Timer channel to configure.
 * @param[in] source Interrupt source to disable.
 *
 * @return error_t
 *         - OK           : Interrupt disabled successfully.
 *         - OUT_OF_RANGE : Invalid timer channel or unsupported interrupt source
 *                          for the selected timer.
 */
error_t TIMER_InterruptDisable(TIMER_Channel_t timer, TIMER_InterruptSource_t source);

/* ========================================================================== */
/*                              Callback API                                  */
/* ========================================================================== */

/**
 * @brief Register a callback function for the specified timer interrupt source.
 *
 * This function assigns a user callback function to the selected interrupt
 * source of the specified timer.
 *
 * @param[in] timer Timer channel to configure.
 * @param[in] source Interrupt source associated with the callback.
 * @param[in] Copy_pvCallbackFunc Pointer to the callback function.
 *                                Passing NULL clears the registered callback.
 *
 * @return error_t
 *         - OK           : Callback registered successfully.
 *         - OUT_OF_RANGE : Invalid timer channel or unsupported interrupt source
 *                          for the selected timer.
 *
 * @note This function only registers the callback. The corresponding interrupt
 *       must be enabled separately using TIMER_InterruptEnable().
 * @note Global interrupts must also be enabled by the application.
 */
error_t TIMER_SetCallback(TIMER_Channel_t timer,
                          TIMER_InterruptSource_t source,
                          TIMER_Callback_t Copy_pvCallbackFunc);

/* ========================================================================== */
/*                               Timer1 ICU API                               */
/* ========================================================================== */

/**
 * @brief Set Timer1 Input Capture trigger edge.
 *
 * This function selects whether Timer1 Input Capture event is triggered
 * on a rising edge or a falling edge on the ICU pin.
 *
 * @param[in] edge Capture trigger edge selection.
 *
 * @return error_t
 *         - OK           : Edge configured successfully.
 *         - OUT_OF_RANGE : Invalid edge selection.
 */
error_t TIMER1_ICU_SetEdge(TIMER1_ICU_Edge_t edge);

/**
 * @brief Read the captured Timer1 value.
 *
 * This function returns the value latched in ICR1 during the last
 * Input Capture event.
 *
 * @return uint16_t Captured Timer1 count value.
 */
uint16_t TIMER1_ICU_GetCaptureValue(void);

/**
 * @brief Clear Timer1 Input Capture flag.
 *
 * This function clears the Input Capture flag (ICF1) in TIFR.
 *
 * @note On AVR, the flag is cleared by writing logical one to it.
 */
void TIMER1_ICU_ClearFlag(void);

/**
 * @brief Enable Timer1 Input Capture noise canceler.
 */
void TIMER1_ICU_EnableNoiseCanceler(void);

/**
 * @brief Disable Timer1 Input Capture noise canceler.
 */
void TIMER1_ICU_DisableNoiseCanceler(void);

#endif /* TIMER_INT_H_ */
