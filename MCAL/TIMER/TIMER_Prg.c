#include "StdTypes.h"
#include "MemMap.h"
#include "TIMER_Private.h"
#include "TIMER_Int.h"
#include "TIMER_Cfg.h"
static TIMER_Callback_t TIMER0_OVF_Callback   = (TIMER_Callback_t)0;
static TIMER_Callback_t TIMER0_COMP_Callback  = (TIMER_Callback_t)0;

static TIMER_Callback_t TIMER1_OVF_Callback   = (TIMER_Callback_t)0;
static TIMER_Callback_t TIMER1_COMPA_Callback = (TIMER_Callback_t)0;
static TIMER_Callback_t TIMER1_COMPB_Callback = (TIMER_Callback_t)0;
static TIMER_Callback_t TIMER1_ICU_Callback   = (TIMER_Callback_t)0;

static TIMER_Callback_t TIMER2_OVF_Callback   = (TIMER_Callback_t)0;
static TIMER_Callback_t TIMER2_COMP_Callback  = (TIMER_Callback_t)0;

error_t TIMER_Init(void)
{
	error_t status = OK;

	if (TIMER0_Init() != OK)
	{
		status = NOK;
	}
	else
	{
		;
	}

	if (TIMER1_Init() != OK)
	{
		status = NOK;
	}
	else
	{
		;
	}

	if (TIMER2_Init() != OK)
	{
		status = NOK;
	}
	else
	{
		;
	}

	return status;
}

error_t TIMER0_Init(void)
{
	error_t status = OK;

	/* Mode */
	switch (TIMER0_CFG.mode)
	{
		case TIMER0_MODE_NORMAL:
		CLR_BIT(TCCR0, WGM00);
		CLR_BIT(TCCR0, WGM01);
		break;

		case TIMER0_MODE_CTC:
		CLR_BIT(TCCR0, WGM00);
		SET_BIT(TCCR0, WGM01);
		break;

		case TIMER0_MODE_FAST_PWM:
		SET_BIT(TCCR0, WGM00);
		SET_BIT(TCCR0, WGM01);
		break;

		case TIMER0_MODE_PHASE_CORRECT_PWM:
		SET_BIT(TCCR0, WGM00);
		CLR_BIT(TCCR0, WGM01);
		break;

		default:
		status = OUT_OF_RANGE;
		break;
	}

	/* OC / PWM */
	if ((TIMER0_CFG.mode == TIMER0_MODE_FAST_PWM) || (TIMER0_CFG.mode == TIMER0_MODE_PHASE_CORRECT_PWM))
	{
		if (TIMER0_CFG.pwm_mode == TIMER_PWM_NON_INVERTING)
		{
			SET_BIT(TCCR0, COM01);
			CLR_BIT(TCCR0, COM00);
		}
		else
		{
			SET_BIT(TCCR0, COM01);
			SET_BIT(TCCR0, COM00);
		}
	}
	else
	{
		TCCR0 &= (uint8_t)(~((1U << COM00) | (1U << COM01)));
		TCCR0 |= (uint8_t)(TIMER0_CFG.oc_mode << COM00);
	}

	/* Keep timer stopped after init */
	TCCR0 &= (uint8_t)(~TIMER_CLOCK_SELECT_MASK);

	/* Initial values */
	TIMER0_SET_COUNTER(TIMER0_CFG.initial_value);
	TIMER0_SET_COMPARE(TIMER0_CFG.compare_value);

	return status;
}

error_t TIMER2_Init(void)
{
	error_t status = OK;

	/* Mode */
	switch (TIMER2_CFG.mode)
	{
		case TIMER2_MODE_NORMAL:
		CLR_BIT(TCCR2, WGM20);
		CLR_BIT(TCCR2, WGM21);
		break;

		case TIMER2_MODE_CTC:
		CLR_BIT(TCCR2, WGM20);
		SET_BIT(TCCR2, WGM21);
		break;

		case TIMER2_MODE_FAST_PWM:
		SET_BIT(TCCR2, WGM20);
		SET_BIT(TCCR2, WGM21);
		break;

		case TIMER2_MODE_PHASE_CORRECT_PWM:
		SET_BIT(TCCR2, WGM20);
		CLR_BIT(TCCR2, WGM21);
		break;

		default:
		status = OUT_OF_RANGE;
		break;
	}

	/* OC / PWM */
	if ((TIMER2_CFG.mode == TIMER2_MODE_FAST_PWM) || (TIMER2_CFG.mode == TIMER2_MODE_PHASE_CORRECT_PWM))
	{
		if (TIMER2_CFG.pwm_mode == TIMER_PWM_NON_INVERTING)
		{
			SET_BIT(TCCR2, COM21);
			CLR_BIT(TCCR2, COM20);
		}
		else
		{
			SET_BIT(TCCR2, COM21);
			SET_BIT(TCCR2, COM20);
		}
	}
	else
	{
		TCCR2 &= (uint8_t)(~((1U << COM20) | (1U << COM21)));
		TCCR2 |= (uint8_t)(TIMER2_CFG.oc_mode << COM20);
	}

	/* Keep timer stopped after init */
	TCCR2 &= (uint8_t)(~TIMER_CLOCK_SELECT_MASK);

	/* Initial values */
	TIMER2_SET_COUNTER(TIMER2_CFG.initial_value);
	TIMER2_SET_COMPARE(TIMER2_CFG.compare_value);

	return status;
}

error_t TIMER1_Init(void)
{
	error_t status = OK;

	/* Mode */
	switch (TIMER1_CFG.mode)
	{
		case TIMER1_MODE_NORMAL:
		CLR_BIT(TCCR1A, WGM10);
		CLR_BIT(TCCR1A, WGM11);
		CLR_BIT(TCCR1B, WGM12);
		CLR_BIT(TCCR1B, WGM13);
		break;

		case TIMER1_MODE_CTC_OCR1A:
		CLR_BIT(TCCR1A, WGM10);
		CLR_BIT(TCCR1A, WGM11);
		SET_BIT(TCCR1B, WGM12);
		CLR_BIT(TCCR1B, WGM13);
		break;

		case TIMER1_MODE_CTC_ICR1:
		CLR_BIT(TCCR1A, WGM10);
		CLR_BIT(TCCR1A, WGM11);
		SET_BIT(TCCR1B, WGM12);
		SET_BIT(TCCR1B, WGM13);
		break;

		case TIMER1_MODE_FAST_PWM_ICR1:
		CLR_BIT(TCCR1A, WGM10);
		SET_BIT(TCCR1A, WGM11);
		SET_BIT(TCCR1B, WGM12);
		SET_BIT(TCCR1B, WGM13);
		break;

		case TIMER1_MODE_FAST_PWM_OCR1A:
		SET_BIT(TCCR1A, WGM10);
		SET_BIT(TCCR1A, WGM11);
		SET_BIT(TCCR1B, WGM12);
		SET_BIT(TCCR1B, WGM13);
		break;

		case TIMER1_MODE_PHASE_CORRECT_ICR1:
		CLR_BIT(TCCR1A, WGM10);
		SET_BIT(TCCR1A, WGM11);
		CLR_BIT(TCCR1B, WGM12);
		SET_BIT(TCCR1B, WGM13);
		break;

		default:
		status = OUT_OF_RANGE;
		break;
	}

	/* OC1A behavior */
	if ((TIMER1_CFG.mode == TIMER1_MODE_FAST_PWM_ICR1) || (TIMER1_CFG.mode == TIMER1_MODE_PHASE_CORRECT_ICR1))
	{
		if (TIMER1_CFG.oc1a_pwm_mode == TIMER_PWM_NON_INVERTING)
		{
			SET_BIT(TCCR1A, COM1A1);
			CLR_BIT(TCCR1A, COM1A0);
		}
		else
		{
			SET_BIT(TCCR1A, COM1A1);
			SET_BIT(TCCR1A, COM1A0);
		}
	}
	else if (TIMER1_CFG.mode == TIMER1_MODE_FAST_PWM_OCR1A)
	{
		/* OCR1A is used as TOP, so OC1A cannot be used as normal PWM output */
		TCCR1A &= (uint8_t)(~((1U << COM1A0) | (1U << COM1A1)));
	}
	else
	{
		TCCR1A &= (uint8_t)(~((1U << COM1A0) | (1U << COM1A1)));
		TCCR1A |= (uint8_t)(TIMER1_CFG.oc1a_mode << COM1A0);
	}

	/* OC1B behavior */
	if ((TIMER1_CFG.mode == TIMER1_MODE_FAST_PWM_ICR1) ||
	(TIMER1_CFG.mode == TIMER1_MODE_FAST_PWM_OCR1A) ||
	(TIMER1_CFG.mode == TIMER1_MODE_PHASE_CORRECT_ICR1))
	{
		if (TIMER1_CFG.oc1b_pwm_mode == TIMER_PWM_NON_INVERTING)
		{
			SET_BIT(TCCR1A, COM1B1);
			CLR_BIT(TCCR1A, COM1B0);
		}
		else
		{
			SET_BIT(TCCR1A, COM1B1);
			SET_BIT(TCCR1A, COM1B0);
		}
	}
	else
	{
		TCCR1A &= (uint8_t)(~((1U << COM1B0) | (1U << COM1B1)));
		TCCR1A |= (uint8_t)(TIMER1_CFG.oc1b_mode << COM1B0);
	}

	/* Keep timer stopped after init */
	TCCR1B &= (uint8_t)(~TIMER_CLOCK_SELECT_MASK);

	/* Initial values */
	TIMER1_SET_COUNTER(TIMER1_CFG.initial_value);
	TIMER1_SET_COMPARE_A(TIMER1_CFG.ocr1a_value);
	TIMER1_SET_COMPARE_B(TIMER1_CFG.ocr1b_value);

	/* ICR1 is used only in modes that select it as TOP */
	if ((TIMER1_CFG.mode == TIMER1_MODE_CTC_ICR1) ||
	(TIMER1_CFG.mode == TIMER1_MODE_FAST_PWM_ICR1) ||
	(TIMER1_CFG.mode == TIMER1_MODE_PHASE_CORRECT_ICR1))
	{
		TIMER1_SET_ICR1(TIMER1_CFG.icr1_value);
	}
	else
	{
		;
	}

	return status;
}

/* Timer0 */
void TIMER0_SetCounter(uint8_t value)
{
	TIMER0_SET_COUNTER(value);
}

uint8_t TIMER0_GetCounter(void)
{
	return TIMER0_GET_COUNTER();
}

void TIMER0_SetCompare(uint8_t value)
{
	TIMER0_SET_COMPARE(value);
}

uint8_t TIMER0_GetCompare(void)
{
	return TIMER0_GET_COMPARE();
}

/* Timer1 */
void TIMER1_SetCounter(uint16_t value)
{
	TIMER1_SET_COUNTER(value);
}

uint16_t TIMER1_GetCounter(void)
{
	return TIMER1_GET_COUNTER();
}

void TIMER1_SetCompareA(uint16_t value)
{
	TIMER1_SET_COMPARE_A(value);
}

uint16_t TIMER1_GetCompareA(void)
{
	return TIMER1_GET_COMPARE_A();
}

void TIMER1_SetCompareB(uint16_t value)
{
	TIMER1_SET_COMPARE_B(value);
}

uint16_t TIMER1_GetCompareB(void)
{
	return TIMER1_GET_COMPARE_B();
}

uint16_t TIMER1_GetTop(void)
{
	uint16_t Local_TopValue = 0U;

	switch (TIMER1_CFG.mode)
	{
		case TIMER1_MODE_CTC_OCR1A:
		case TIMER1_MODE_FAST_PWM_OCR1A:
		Local_TopValue = TIMER1_GET_COMPARE_A();
		break;

		case TIMER1_MODE_CTC_ICR1:
		case TIMER1_MODE_FAST_PWM_ICR1:
		case TIMER1_MODE_PHASE_CORRECT_ICR1:
		Local_TopValue = TIMER1_GET_ICR1();
		break;

		case TIMER1_MODE_NORMAL:
		default:
		Local_TopValue = 0xFFFFU;
		break;
	}

	return Local_TopValue;
}

void TIMER1_SetTop(uint16_t value)
{
	switch (TIMER1_CFG.mode)
	{
		case TIMER1_MODE_CTC_OCR1A:
		case TIMER1_MODE_FAST_PWM_OCR1A:
		TIMER1_SET_COMPARE_A(value);
		break;

		case TIMER1_MODE_CTC_ICR1:
		case TIMER1_MODE_FAST_PWM_ICR1:
		case TIMER1_MODE_PHASE_CORRECT_ICR1:
		TIMER1_SET_ICR1(value);
		break;

		case TIMER1_MODE_NORMAL:
		default:
		;
		break;
	}
}

/* Timer2 */
void TIMER2_SetCounter(uint8_t value)
{
	TIMER2_SET_COUNTER(value);
}

uint8_t TIMER2_GetCounter(void)
{
	return TIMER2_GET_COUNTER();
}

void TIMER2_SetCompare(uint8_t value)
{
	TIMER2_SET_COMPARE(value);
}

uint8_t TIMER2_GetCompare(void)
{
	return TIMER2_GET_COMPARE();
}

error_t TIMER_Start(TIMER_Channel_t timer)
{
	error_t Local_ErrorState = OK;

	switch (timer)
	{
		case TIMER0:
		TCCR0 &= (uint8_t)(~TIMER_CLOCK_SELECT_MASK);
		TCCR0 |= (uint8_t)(TIMER0_CFG.prescaler);
		break;

		case TIMER1:
		TCCR1B &= (uint8_t)(~TIMER_CLOCK_SELECT_MASK);
		TCCR1B |= (uint8_t)(TIMER1_CFG.prescaler);
		break;

		case TIMER2:
		TCCR2 &= (uint8_t)(~TIMER_CLOCK_SELECT_MASK);
		TCCR2 |= (uint8_t)(TIMER2_CFG.prescaler);
		break;

		default:
		Local_ErrorState = OUT_OF_RANGE;
		break;
	}

	return Local_ErrorState;
}

error_t TIMER_Stop(TIMER_Channel_t timer)
{
	error_t Local_ErrorState = OK;

	switch (timer)
	{
		case TIMER0:
		TCCR0 &= (uint8_t)(~TIMER_CLOCK_SELECT_MASK);
		break;

		case TIMER1:
		TCCR1B &= (uint8_t)(~TIMER_CLOCK_SELECT_MASK);
		break;

		case TIMER2:
		TCCR2 &= (uint8_t)(~TIMER_CLOCK_SELECT_MASK);
		break;

		default:
		Local_ErrorState = OUT_OF_RANGE;
		break;
	}

	return Local_ErrorState;
}
error_t TIMER_InterruptEnable(TIMER_Channel_t timer, TIMER_InterruptSource_t source)
{
	error_t Local_ErrorState = OK;

	switch (timer)
	{
		case TIMER0:
		switch (source)
		{
			case TIMER_INT_OVF:
			SET_BIT(TIMSK, TOIE0);
			break;

			case TIMER_INT_COMP_A:
			SET_BIT(TIMSK, OCIE0);
			break;

			default:
			Local_ErrorState = OUT_OF_RANGE;
			break;
		}
		break;

		case TIMER1:
		switch (source)
		{
			case TIMER_INT_OVF:
			SET_BIT(TIMSK, TOIE1);
			break;

			case TIMER_INT_COMP_A:
			SET_BIT(TIMSK, OCIE1A);
			break;

			case TIMER_INT_COMP_B:
			SET_BIT(TIMSK, OCIE1B);
			break;

			case TIMER_INT_ICU:
			SET_BIT(TIMSK, TICIE1);
			break;

			default:
			Local_ErrorState = OUT_OF_RANGE;
			break;
		}
		break;

		case TIMER2:
		switch (source)
		{
			case TIMER_INT_OVF:
			SET_BIT(TIMSK, TOIE2);
			break;

			case TIMER_INT_COMP_A:
			SET_BIT(TIMSK, OCIE2);
			break;

			default:
			Local_ErrorState = OUT_OF_RANGE;
			break;
		}
		break;

		default:
		Local_ErrorState = OUT_OF_RANGE;
		break;
	}

	return Local_ErrorState;
}

error_t TIMER_InterruptDisable(TIMER_Channel_t timer, TIMER_InterruptSource_t source)
{
	error_t Local_ErrorState = OK;

	switch (timer)
	{
		case TIMER0:
		switch (source)
		{
			case TIMER_INT_OVF:
			CLR_BIT(TIMSK, TOIE0);
			break;

			case TIMER_INT_COMP_A:
			CLR_BIT(TIMSK, OCIE0);
			break;

			default:
			Local_ErrorState = OUT_OF_RANGE;
			break;
		}
		break;

		case TIMER1:
		switch (source)
		{
			case TIMER_INT_OVF:
			CLR_BIT(TIMSK, TOIE1);
			break;

			case TIMER_INT_COMP_A:
			CLR_BIT(TIMSK, OCIE1A);
			break;

			case TIMER_INT_COMP_B:
			CLR_BIT(TIMSK, OCIE1B);
			break;

			case TIMER_INT_ICU:
			CLR_BIT(TIMSK, TICIE1);
			break;

			default:
			Local_ErrorState = OUT_OF_RANGE;
			break;
		}
		break;

		case TIMER2:
		switch (source)
		{
			case TIMER_INT_OVF:
			CLR_BIT(TIMSK, TOIE2);
			break;

			case TIMER_INT_COMP_A:
			CLR_BIT(TIMSK, OCIE2);
			break;

			default:
			Local_ErrorState = OUT_OF_RANGE;
			break;
		}
		break;

		default:
		Local_ErrorState = OUT_OF_RANGE;
		break;
	}

	return Local_ErrorState;
}
error_t TIMER_SetCallback(TIMER_Channel_t timer, TIMER_InterruptSource_t source, TIMER_Callback_t Copy_pvCallbackFunc)
{
	error_t Local_ErrorState = OK;

	switch (timer)
	{
		case TIMER0:
		switch (source)
		{
			case TIMER_INT_OVF:
			TIMER0_OVF_Callback = Copy_pvCallbackFunc;
			break;

			case TIMER_INT_COMP_A:
			TIMER0_COMP_Callback = Copy_pvCallbackFunc;
			break;

			default:
			Local_ErrorState = OUT_OF_RANGE;
			break;
		}
		break;

		case TIMER1:
		switch (source)
		{
			case TIMER_INT_OVF:
			TIMER1_OVF_Callback = Copy_pvCallbackFunc;
			break;

			case TIMER_INT_COMP_A:
			TIMER1_COMPA_Callback = Copy_pvCallbackFunc;
			break;

			case TIMER_INT_COMP_B:
			TIMER1_COMPB_Callback = Copy_pvCallbackFunc;
			break;

			case TIMER_INT_ICU:
			TIMER1_ICU_Callback = Copy_pvCallbackFunc;
			break;

			default:
			Local_ErrorState = OUT_OF_RANGE;
			break;
		}
		break;

		case TIMER2:
		switch (source)
		{
			case TIMER_INT_OVF:
			TIMER2_OVF_Callback = Copy_pvCallbackFunc;
			break;

			case TIMER_INT_COMP_A:
			TIMER2_COMP_Callback = Copy_pvCallbackFunc;
			break;

			default:
			Local_ErrorState = OUT_OF_RANGE;
			break;
		}
		break;

		default:
		Local_ErrorState = OUT_OF_RANGE;
		break;
	}

	return Local_ErrorState;
}
error_t TIMER1_ICU_SetEdge(TIMER1_ICU_Edge_t edge)
{
	error_t Local_ErrorState = OK;

	switch (edge)
	{
		case TIMER1_ICU_FALLING_EDGE:
		CLR_BIT(TCCR1B, ICES1);
		break;

		case TIMER1_ICU_RISING_EDGE:
		SET_BIT(TCCR1B, ICES1);
		break;

		default:
		Local_ErrorState = OUT_OF_RANGE;
		break;
	}

	return Local_ErrorState;
}

uint16_t TIMER1_ICU_GetCaptureValue(void)
{
	return TIMER1_GET_ICR1();
}

void TIMER1_ICU_ClearFlag(void)
{
	TIMER1_CLEAR_ICU_FLAG();
}

void TIMER1_ICU_EnableNoiseCanceler(void)
{
	SET_BIT(TCCR1B, ICNC1);
}

void TIMER1_ICU_DisableNoiseCanceler(void)
{
	CLR_BIT(TCCR1B, ICNC1);
}
/* Timer2 Compare Match ISR */
ISR(TIMER2_COMP_vect)
{
	if (TIMER2_COMP_Callback != (TIMER_Callback_t)0)
	{
		TIMER2_COMP_Callback();
	}
	else
	{
		;
	}
}

/* Timer2 Overflow ISR */
ISR(TIMER2_OVF_vect)
{
	if (TIMER2_OVF_Callback != (TIMER_Callback_t)0)
	{
		TIMER2_OVF_Callback();
	}
	else
	{
		;
	}
}

/* Timer1 Input Capture ISR */
ISR(TIMER1_ICU_vect)
{
	if (TIMER1_ICU_Callback != (TIMER_Callback_t)0)
	{
		TIMER1_ICU_Callback();
	}
	else
	{
		;
	}
}

/* Timer1 Compare Match A ISR */
ISR(TIMER1_OCA_vect)
{
	if (TIMER1_COMPA_Callback != (TIMER_Callback_t)0)
	{
		TIMER1_COMPA_Callback();
	}
	else
	{
		;
	}
}

/* Timer1 Compare Match B ISR */
ISR(TIMER1_OCB_vect)
{
	if (TIMER1_COMPB_Callback != (TIMER_Callback_t)0)
	{
		TIMER1_COMPB_Callback();
	}
	else
	{
		;
	}
}

/* Timer1 Overflow ISR */
ISR(TIMER1_OVF_vect)
{
	if (TIMER1_OVF_Callback != (TIMER_Callback_t)0)
	{
		TIMER1_OVF_Callback();
	}
	else
	{
		;
	}
}

/* Timer0 Compare Match ISR */
ISR(TIMER0_OC_vect)
{
	if (TIMER0_COMP_Callback != (TIMER_Callback_t)0)
	{
		TIMER0_COMP_Callback();
	}
	else
	{
		;
	}
}

/* Timer0 Overflow ISR */
ISR(TIMER0_OV_vect)
{
	if (TIMER0_OVF_Callback != (TIMER_Callback_t)0)
	{
		TIMER0_OVF_Callback();
	}
	else
	{
		;
	}
}