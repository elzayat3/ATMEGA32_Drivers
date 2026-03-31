#include "StdTypes.h"
#include "MemMap.h"
#include "TIMER_Private.h"
#include "TIMER_Int.h"
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

	/*  OC / PWM */
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
		TCCR2 &= ~((1U << COM20) | (1U << COM21));
		TCCR2 |= (TIMER2_CFG.oc_mode << COM20);
	}

	/*  Prescaler */
	TCCR2 &= ~(0x07);
	TCCR2 |= TIMER2_CFG.prescaler;

	/*  Values */
	TIMER2_SET_COUNTER(TIMER2_CFG.initial_value);
	TIMER2_SET_COMPARE(TIMER2_CFG.compare_value);

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
	if ((TIMER1_CFG.mode == TIMER1_MODE_FAST_PWM_ICR1) ||(TIMER1_CFG.mode == TIMER1_MODE_FAST_PWM_OCR1A) || (TIMER1_CFG.mode == TIMER1_MODE_PHASE_CORRECT_ICR1))
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
	TIMER1_SET_COUNTER(TIMER1_CFG.initial_value) ;
	TIMER1_SET_COMPARE_A(TIMER1_CFG.compare_A);
	TIMER1_SET_COMPARE_B(TIMER1_CFG.compare_B);

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
	if (TIMER1_CFG.mode == TIMER1_MODE_CTC_OCR1A || TIMER1_CFG.mode == TIMER1_MODE_FAST_PWM_OCR1A)
	{
		return TIMER1_GET_COMPARE_A(); // TOP is OCR1A
	}
	else
	{
		return TIMER1_GET_ICR1();  // TOP is ICR1
	}
}

void TIMER1_SetTop(uint16_t value)
{
	if (TIMER1_CFG.mode == TIMER1_MODE_CTC_OCR1A || TIMER1_CFG.mode == TIMER1_MODE_FAST_PWM_OCR1A)
	{
		TIMER1_SET_COMPARE_A(value);
	}
	else
	{
		TIMER1_SET_ICR1(value);
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