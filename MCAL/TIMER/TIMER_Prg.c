#include "StdTypes.h"
#include "MemMap.h"
#include "TIMER_Int.h"
#include "TIMER_Private.h"
#include "TIMER_Cfg.h"

error_t TIMER_Init(void)
{
	error_t status = OK;

	status |= TIMER0_Init();
	status |= TIMER1_Init();
	status |= TIMER2_Init();

	return status;
}

error_t TIMER0_Init(void)
{
	error_t status = OK;

	/*  Mode */
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

	/*  OC / PWM */
	if ((TIMER0_CFG.mode == TIMER0_MODE_FAST_PWM) || (TIMER0_CFG.mode == TIMER0_MODE_PHASE_CORRECT_PWM))
	{
		/* PWM */
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
		/* Normal / CTC */
		TCCR0 &= ~((1U << COM00) | (1U << COM01));
		TCCR0 |= (TIMER0_CFG.oc_mode << COM00);
	}

	/*  Prescaler */
	TCCR0 &= ~(0x07);
	TCCR0 |= TIMER0_CFG.prescaler;

	/*  Values */
	TCNT0 = TIMER0_CFG.initial_value;
	OCR0  = TIMER0_CFG.compare_value;

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

	/*  OC / PWM */
	if ((TIMER2_CFG.mode == TIMER2_MODE_FAST_PWM) ||
	(TIMER2_CFG.mode == TIMER2_MODE_PHASE_CORRECT_PWM))
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
		TCCR2 &= ~((1U << COM20) | (1U << COM21));
		TCCR2 |= (TIMER2_CFG.oc_mode << COM20);
	}

	/*  Prescaler */
	TCCR2 &= ~(0x07);
	TCCR2 |= TIMER2_CFG.prescaler;

	/*  Values */
	TCNT2 = TIMER2_CFG.initial_value;
	OCR2  = TIMER2_CFG.compare_value;

	return status;
}
error_t TIMER1_Init(void)
{
	error_t status = OK;

	/*  Mode */
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

		ICR1 = TIMER1_CFG.top_value;

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

		ICR1 = TIMER1_CFG.top_value;

		break;

		default:
		status = OUT_OF_RANGE;
		break;
	}

	/*  OC1A PWM behavior */
	if ((TIMER1_CFG.mode == TIMER1_MODE_FAST_PWM_ICR1) ||(TIMER1_CFG.mode == TIMER1_MODE_FAST_PWM_OCR1A) ||
	(TIMER1_CFG.mode == TIMER1_MODE_PHASE_CORRECT_ICR1))
	{
		if (TIMER1_CFG.pwm_mode == TIMER_PWM_NON_INVERTING)
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
	else
	{
		TCCR1A &= ~((1U << COM1A0) | (1U << COM1A1));
		TCCR1A |= (TIMER1_CFG.oc1a_mode << COM1A0);
	}

	/*  OC1B */
	TCCR1A &= ~((1U << COM1B0) | (1U << COM1B1));
	TCCR1A |= (TIMER1_CFG.oc1b_mode << COM1B0);

	/*  Prescaler */
	TCCR1B &= ~(0x07);
	TCCR1B |= TIMER1_CFG.prescaler;

	/*  Values */
	TCNT1 = TIMER1_CFG.initial_value;
	OCR1A = TIMER1_CFG.compare_A;
	OCR1B = TIMER1_CFG.compare_B;

	return status;
}