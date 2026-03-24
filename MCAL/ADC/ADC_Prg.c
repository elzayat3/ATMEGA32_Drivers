#include "StdTypes.h"
#include "MemMap.h"
#include "ADC_Int.h"
#include "ADC_Private.h"
#include "ADC_Cfg.h"
static u8	ConversionFlag;
void ADC_Init(ADC_VREF_t vref,ADC_Prescaler_t scaler)
{
	/*vref*/
	switch(vref)
	{
		case VREF_AREF:
		CLR_BIT(ADMUX,REFS0);
		CLR_BIT(ADMUX,REFS1);
		break;
		case VREF_VCC:
		SET_BIT(ADMUX,REFS0);
		CLR_BIT(ADMUX,REFS1);
		break;
		case VREF_256:
		SET_BIT(ADMUX,REFS0);
		SET_BIT(ADMUX,REFS1);
		break;
	}
	/*prescaler*/
	ADCSRA=ADCSRA&0xf8;
	ADCSRA=ADCSRA|scaler;
	/*enable ADC*/
	SET_BIT(ADCSRA,ADEN);
	CLR_BIT(ADMUX,ADLAR);
	CLR_BIT(ADCSRA,ADATE);
}
u16 ADC_Read(ADC_Channel_t ch)
{
	/*select channel*/
	ADMUX=ADMUX&0xE0;
	ADMUX=ADMUX|ch;
	/*start conversion*/
	SET_BIT(ADCSRA,ADSC);
	/*wait until conversion end*/
	//_delay_us(1); OR
	while(GET_BIT(ADCSRA,ADSC));
	/*get read*/
	return ADC;
}
u16 ADC_ReadVolt(ADC_Channel_t ch)
{
	u16 adc=ADC_Read(ch);
	u16 volt;
	volt=((u32)5000*adc)/1023;// if 1023 max volt will be 5000 
	return volt;
}
void ADC_StartConversion(ADC_Channel_t ch)
{
	if(ConversionFlag==0)
	{
		/*select channel*/
		ADMUX=ADMUX&0xE0;
		ADMUX=ADMUX|ch;
		/*start conversion*/
		SET_BIT(ADCSRA,ADSC);
		ConversionFlag=1;
	}
}
u16 ADC_GetRead(void)
{
	/*wait until conversion end*/
	//_delay_us(1); OR
	while(GET_BIT(ADCSRA,ADSC));
	ConversionFlag=0;
	/*get read*/
	return ADC;
}
error_t ADC_GetReadPeriodic(u16*pdata)
{
	error_t r=OK;
	if(!GET_BIT(ADCSRA,ADSC))
	{
		*pdata=ADC;
		ConversionFlag=0;
	}
	else
	{
		r=IN_PROGRESS;
	}
	return r;
}
u16 ADC_GetReadNoBlock(void)
{
	return ADC;
}
void ADC_InterruptEnable(void)
{
	SET_BIT(ADCSRA,ADIE);
}
void ADC_SetCallBack(void (*LocalFptr)(void))
{
	ADC_Fptr = LocalFptr;
}
ISR(ADC_vect)
{
	if (ADC_Fptr!=NULLPTR)
	{
		ADC_Fptr();
	}
	ConversionFlag=0;
}