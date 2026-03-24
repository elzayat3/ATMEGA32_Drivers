#include "StdTypes.h"
#include "MemMap.h"
#include "EXIU_Int.h"
#include "EXIU_Private.h"
#include "EXIU_Cfg.h"
static void (*exiu_callback[3])(void) = {NULLPTR};
void EXIU_enable(Expin_t pin)
{
    switch(pin)
    {
        case EX_INT0:
            SET_BIT(GICR, INT0);
            break;

        case EX_INT1:
            SET_BIT(GICR, INT1);
            break;

        case EX_INT2:
            SET_BIT(GICR, INT2);
            break;

        default:
            /* Do nothing */
            break;
    }
}
void EXIU_disable(Expin_t pin)
{
    switch(pin)
    {
        case EX_INT0:
            CLR_BIT(GICR, INT0);
            break;

        case EX_INT1:
            CLR_BIT(GICR, INT1);
            break;

        case EX_INT2:
            CLR_BIT(GICR, INT2);
            break;

        default:
            /* Do nothing */
            break;
    }
}
void EXIU_Init(void)
{
	EXIU_triggeredge(EX_INT0,FALLING_EDGE);
	EXIU_triggeredge(EX_INT1,FALLING_EDGE);
	EXIU_triggeredge(EX_INT2,FALLING_EDGE);
}
void EXIU_triggeredge(Expin_t pin,Trigger_t edge)
{
	if(pin==EX_INT0)
	{
		switch(edge)
		{
			case LOW_LEVEL:
			CLR_BIT(MCUCR,ISC00);
			CLR_BIT(MCUCR,ISC01);
			break;
			case ANY_LOGIC_CHANGE:
			SET_BIT(MCUCR,ISC00);
			CLR_BIT(MCUCR,ISC01);
			break;
			case FALLING_EDGE:
			CLR_BIT(MCUCR,ISC00);
			SET_BIT(MCUCR,ISC01);
			break;
			case RISING_EDGE:
			SET_BIT(MCUCR,ISC00);
			SET_BIT(MCUCR,ISC01);
			break;
			default:
			CLR_BIT(MCUCR,ISC00);
			SET_BIT(MCUCR,ISC01);
			break;
		}
	}
	else if(pin==EX_INT1)
	{
		switch(edge)
		{
			case LOW_LEVEL:
			CLR_BIT(MCUCR,ISC10);
			CLR_BIT(MCUCR,ISC11);
			break;
			case ANY_LOGIC_CHANGE:
			SET_BIT(MCUCR,ISC10);
			CLR_BIT(MCUCR,ISC11);
			break;
			case FALLING_EDGE:
			CLR_BIT(MCUCR,ISC10);
			SET_BIT(MCUCR,ISC11);
			break;
			case RISING_EDGE:
			SET_BIT(MCUCR,ISC10);
			SET_BIT(MCUCR,ISC11);
			break;
			default:
			CLR_BIT(MCUCR,ISC10);
			SET_BIT(MCUCR,ISC11);
			break;
			
		}
	}
	else if(pin==EX_INT2)
	{
		switch(edge)
		{
			case FALLING_EDGE:
			CLR_BIT(MCUCSR,ISC2);
			break;
			case RISING_EDGE:
			SET_BIT(MCUCSR,ISC2);
			break;
			default:
			CLR_BIT(MCUCSR,ISC2);
			break;
		}
	}
	else{;}
}
ISR(INT0_vect)
{
	if(exiu_callback[EX_INT0] != NULLPTR)
	{
		exiu_callback[EX_INT0]();
	}
}
ISR(INT1_vect)
{
	if(exiu_callback[EX_INT1] != NULLPTR)
	{
		exiu_callback[EX_INT1]();
	}
}
ISR(INT2_vect)
{
	if(exiu_callback[EX_INT2] != NULLPTR)
	{
		exiu_callback[EX_INT2]();
	}
}
void EXIU_SetCallback(Expin_t pin, void(*local)(void))
{
	if(local != NULLPTR)
	{
		exiu_callback[pin] = local;
	}
}