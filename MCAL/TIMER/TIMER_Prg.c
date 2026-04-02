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

static uint8_t TIMER1_IsPwmMode(TIMER1_Mode_t mode)
{
	uint8_t Local_Result = 0U;

	if ((mode == TIMER1_MODE_FAST_PWM_ICR1) || (mode == TIMER1_MODE_FAST_PWM_OCR1A) || (mode == TIMER1_MODE_PHASE_CORRECT_ICR1))
	{
		Local_Result = 1U;
	}
	else
	{
		;
	}

	return Local_Result;
}

static uint8_t TIMER1_IsTopIcr1Mode(TIMER1_Mode_t mode)
{
	uint8_t Local_Result = 0U;

	if ((mode == TIMER1_MODE_CTC_ICR1) ||(mode == TIMER1_MODE_FAST_PWM_ICR1) || (mode == TIMER1_MODE_PHASE_CORRECT_ICR1))
	{
		Local_Result = 1U;
	}
	else
	{
		;
	}

	return Local_Result;
}

static uint8_t TIMER1_IsTopOcr1aMode(TIMER1_Mode_t mode)
{
	uint8_t Local_Result = 0U;

	if ((mode == TIMER1_MODE_CTC_OCR1A) ||(mode == TIMER1_MODE_FAST_PWM_OCR1A))
	{
		Local_Result = 1U;
	}
	else
	{
		;
	}

	return Local_Result;
}

error_t TIMER_Init(void)
{
	error_t Local_ErrorState = OK;

	if (TIMER0_Init() != OK)
	{
		Local_ErrorState = NOK;
	}
	else
	{
		;
	}

	if (TIMER1_Init() != OK)
	{
		Local_ErrorState = NOK;
	}
	else
	{
		;
	}

	if (TIMER2_Init() != OK)
	{
		Local_ErrorState = NOK;
	}
	else
	{
		;
	}

	return Local_ErrorState;
}

error_t TIMER0_Init(void)
{
	error_t Local_ErrorState = OK;

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
		Local_ErrorState = OUT_OF_RANGE;
		break;
	}

	if (Local_ErrorState == OK)
	{
		if ((TIMER0_CFG.mode == TIMER0_MODE_FAST_PWM) || (TIMER0_CFG.mode == TIMER0_MODE_PHASE_CORRECT_PWM))
		{
			TCCR0 &= (uint8_t)(~TIMER0_COM_MASK);

			switch (TIMER0_CFG.pwm_mode)
			{
				case TIMER_PWM_NON_INVERTING:
				SET_BIT(TCCR0, COM01);
				break;

				case TIMER_PWM_INVERTING:
				SET_BIT(TCCR0, COM01);
				SET_BIT(TCCR0, COM00);
				break;

				default:
				Local_ErrorState = OUT_OF_RANGE;
				break;
			}
		}
		else
		{
			TCCR0 &= (uint8_t)(~TIMER0_COM_MASK);

			if ((uint8_t)TIMER0_CFG.oc_mode <= (uint8_t)TIMER_OC_SET)
			{
				TCCR0 |= (uint8_t)(TIMER0_CFG.oc_mode << COM00);
			}
			else
			{
				Local_ErrorState = OUT_OF_RANGE;
			}
		}
	}
	else
	{
		;
	}

	if (Local_ErrorState == OK)
	{
		TCCR0 &= (uint8_t)(~TIMER_CLOCK_SELECT_MASK);
		TIMER0_SET_COUNTER(TIMER0_CFG.initial_value);
		TIMER0_SET_COMPARE(TIMER0_CFG.compare_value);
	}
	else
	{
		;
	}

	return Local_ErrorState;
}

error_t TIMER1_Init(void)
{
	error_t Local_ErrorState = OK;

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
		Local_ErrorState = OUT_OF_RANGE;
		break;
	}

	if (Local_ErrorState == OK)
	{
		TCCR1A &= (uint8_t)(~TIMER1_COM1A_MASK);

		if ((TIMER1_CFG.mode == TIMER1_MODE_FAST_PWM_ICR1) || (TIMER1_CFG.mode == TIMER1_MODE_PHASE_CORRECT_ICR1))
		{
			switch (TIMER1_CFG.oc1a_pwm_mode)
			{
				case TIMER_PWM_NON_INVERTING:
				SET_BIT(TCCR1A, COM1A1);
				break;

				case TIMER_PWM_INVERTING:
				SET_BIT(TCCR1A, COM1A1);
				SET_BIT(TCCR1A, COM1A0);
				break;

				default:
				Local_ErrorState = OUT_OF_RANGE;
				break;
			}
		}
		else if (TIMER1_CFG.mode == TIMER1_MODE_FAST_PWM_OCR1A)
		{
			;
		}
		else if ((uint8_t)TIMER1_CFG.oc1a_mode <= (uint8_t)TIMER1_OC_SET)
		{
			TCCR1A |= (uint8_t)(TIMER1_CFG.oc1a_mode << COM1A0);
		}
		else
		{
			Local_ErrorState = OUT_OF_RANGE;
		}
	}
	else
	{
		;
	}

	if (Local_ErrorState == OK)
	{
		TCCR1A &= (uint8_t)(~TIMER1_COM1B_MASK);

		if (TIMER1_IsPwmMode(TIMER1_CFG.mode) == 1U)
		{
			switch (TIMER1_CFG.oc1b_pwm_mode)
			{
				case TIMER_PWM_NON_INVERTING:
				SET_BIT(TCCR1A, COM1B1);
				break;

				case TIMER_PWM_INVERTING:
				SET_BIT(TCCR1A, COM1B1);
				SET_BIT(TCCR1A, COM1B0);
				break;

				default:
				Local_ErrorState = OUT_OF_RANGE;
				break;
			}
		}
		else if ((uint8_t)TIMER1_CFG.oc1b_mode <= (uint8_t)TIMER1_OC_SET)
		{
			TCCR1A |= (uint8_t)(TIMER1_CFG.oc1b_mode << COM1B0);
		}
		else
		{
			Local_ErrorState = OUT_OF_RANGE;
		}
	}
	else
	{
		;
	}

	if (Local_ErrorState == OK)
	{
		TCCR1B &= (uint8_t)(~TIMER_CLOCK_SELECT_MASK);

		TIMER1_SET_COUNTER(TIMER1_CFG.initial_value);
		TIMER1_SET_COMPARE_A(TIMER1_CFG.ocr1a_value);
		TIMER1_SET_COMPARE_B(TIMER1_CFG.ocr1b_value);

		if (TIMER1_IsTopIcr1Mode(TIMER1_CFG.mode) == 1U)
		{
			TIMER1_SET_ICR1(TIMER1_CFG.icr1_value);
		}
		else
		{
			;
		}
	}
	else
	{
		;
	}

	return Local_ErrorState;
}

error_t TIMER2_Init(void)
{
	error_t Local_ErrorState = OK;

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
		Local_ErrorState = OUT_OF_RANGE;
		break;
	}

	if (Local_ErrorState == OK)
	{
		if ((TIMER2_CFG.mode == TIMER2_MODE_FAST_PWM) || (TIMER2_CFG.mode == TIMER2_MODE_PHASE_CORRECT_PWM))
		{
			TCCR2 &= (uint8_t)(~TIMER2_COM_MASK);

			switch (TIMER2_CFG.pwm_mode)
			{
				case TIMER_PWM_NON_INVERTING:
				SET_BIT(TCCR2, COM21);
				break;

				case TIMER_PWM_INVERTING:
				SET_BIT(TCCR2, COM21);
				SET_BIT(TCCR2, COM20);
				break;

				default:
				Local_ErrorState = OUT_OF_RANGE;
				break;
			}
		}
		else
		{
			TCCR2 &= (uint8_t)(~TIMER2_COM_MASK);

			if ((uint8_t)TIMER2_CFG.oc_mode <= (uint8_t)TIMER_OC_SET)
			{
				TCCR2 |= (uint8_t)(TIMER2_CFG.oc_mode << COM20);
			}
			else
			{
				Local_ErrorState = OUT_OF_RANGE;
			}
		}
	}
	else
	{
		;
	}

	if (Local_ErrorState == OK)
	{
		TCCR2 &= (uint8_t)(~TIMER_CLOCK_SELECT_MASK);
		TIMER2_SET_COUNTER(TIMER2_CFG.initial_value);
		TIMER2_SET_COMPARE(TIMER2_CFG.compare_value);
	}
	else
	{
		;
	}

	return Local_ErrorState;
}

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

void TIMER1_SetTop(uint16_t value)
{
	if (TIMER1_IsTopOcr1aMode(TIMER1_CFG.mode) == 1U)
	{
		TIMER1_SET_COMPARE_A(value);
	}
	else if (TIMER1_IsTopIcr1Mode(TIMER1_CFG.mode) == 1U)
	{
		TIMER1_SET_ICR1(value);
	}
	else
	{
		;
	}
}

uint16_t TIMER1_GetTop(void)
{
	uint16_t Local_TopValue = 0xFFFFU;

	if (TIMER1_IsTopOcr1aMode(TIMER1_CFG.mode) == 1U)
	{
		Local_TopValue = TIMER1_GET_COMPARE_A();
	}
	else if (TIMER1_IsTopIcr1Mode(TIMER1_CFG.mode) == 1U)
	{
		Local_TopValue = TIMER1_GET_ICR1();
	}
	else
	{
		;
	}

	return Local_TopValue;
}

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

error_t TIMER_SetCallback(TIMER_Channel_t timer,
TIMER_InterruptSource_t source,
TIMER_Callback_t Copy_pvCallbackFunc)
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
