#include "StdTypes.h"
#include "MemMap.h"
#include "TIMER_Private.h"
#include "TIMER_Int.h"
#include "TIMER_Cfg.h"

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