#ifndef ADC_INT_H_
#define ADC_INT_H_
typedef enum{
	VREF_AREF,
	VREF_VCC,
	VREF_256
}ADC_VREF_t;
typedef enum{
	CH_0=0,
	CH_1,
	CH_2,
	CH_3,
	CH_4,
	CH_5,
	CH_6,
	CH_7
}ADC_Channel_t;

typedef enum{
	ADC_SCALER_2=1,
	ADC_SCALER_4,
	ADC_SCALER_8,
	ADC_SCALER_16,
	ADC_SCALER_32,
	ADC_SCALER_64,
	ADC_SCALER_128,
}ADC_Prescaler_t;

void ADC_Init(ADC_VREF_t vref,ADC_Prescaler_t scaler);

u16 ADC_Read(ADC_Channel_t ch);

/* return mili volt*/
u16 ADC_ReadVolt(ADC_Channel_t ch);

void ADC_StartConversion(ADC_Channel_t ch);
u16 ADC_GetRead(void);

error_t ADC_GetReadPerodic(u16*pdata);
u16 ADC_GetReadNoBlock(void);
#endif /* ADC_INT_H_ */