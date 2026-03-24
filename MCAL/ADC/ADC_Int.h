#ifndef ADC_INT_H_
#define ADC_INT_H_


/**
 * @file ADC_Int.h
 * @brief Interface for ADC driver (ATmega32)
 *
 * This file contains the public APIs and user data types
 * for the Analog to Digital Converter (ADC) driver.
 *
 * The driver supports:
 * - Blocking ADC read
 * - Non-blocking ADC read
 * - Periodic polling
 * - Voltage conversion
 *
 * @author Abdelrahman Elzayat
 */
/**
 * @enum ADC_VREF_t
 * @brief ADC voltage reference selection.
 *
 * This enum defines the possible voltage reference sources
 * used by the ADC module.
 */
typedef enum{
	VREF_AREF,  /**< External AREF pin */
	VREF_VCC,   /**< AVCC with external capacitor at AREF pin */
	VREF_256=3    /**< Internal 2.56V reference */
}ADC_VREF_t;
/**
 * @enum ADC_Channel_t
 * @brief ADC input channels.
 *
 * Represents the available analog input channels
 * for the ATmega32 ADC module.
 */
typedef enum{
	CH_0=0, /**< ADC Channel 0 */
	CH_1,   /**< ADC Channel 1 */
	CH_2,   /**< ADC Channel 2 */
	CH_3,   /**< ADC Channel 3 */
	CH_4,   /**< ADC Channel 4 */
	CH_5,   /**< ADC Channel 5 */
	CH_6,   /**< ADC Channel 6 */
	CH_7    /**< ADC Channel 7 */
}ADC_Channel_t;

/**
 * @enum ADC_Prescaler_t
 * @brief ADC clock prescaler values.
 *
 * These values control the division factor applied
 * to the system clock to generate the ADC clock.
 */
typedef enum{
	ADC_SCALER_2=1,   /**< Division factor 2 */
	ADC_SCALER_4,     /**< Division factor 4 */
	ADC_SCALER_8,     /**< Division factor 8 */
	ADC_SCALER_16,    /**< Division factor 16 */
	ADC_SCALER_32,    /**< Division factor 32 */
	ADC_SCALER_64,    /**< Division factor 64 */
	ADC_SCALER_128,   /**< Division factor 128 */
}ADC_Prescaler_t;

/**
 * @brief Initializes the ADC peripheral.
 *
 * Configures the voltage reference and ADC clock prescaler.
 *
 * @param vref   Selected voltage reference.
 * @param scaler ADC clock prescaler.
 */
void ADC_Init(ADC_VREF_t vref,ADC_Prescaler_t scaler);

/**
 * @brief Performs a blocking ADC conversion.
 *
 * This function selects the given ADC channel,
 * starts conversion, and waits until the conversion
 * is complete before returning the result.
 *
 * @param ch ADC channel to read from.
 *
 * @return 10-bit ADC conversion result.
 */
u16 ADC_Read(ADC_Channel_t ch);;

/**
 * @brief Reads the analog voltage from a channel.
 *
 * Performs an ADC conversion and converts the result
 * into millivolts.
 *
 * @param ch ADC channel.
 *
 * @return Voltage value in millivolts.
 */
u16 ADC_ReadVolt(ADC_Channel_t ch);

/**
 * @brief Starts ADC conversion (non-blocking).
 *
 * This function starts the ADC conversion process
 * without waiting for completion.
 *
 * @param ch ADC channel.
 */
void ADC_StartConversion(ADC_Channel_t ch);
/**
 * @brief Retrieves ADC result after conversion.
 *
 * Waits until the ADC conversion is finished
 * and returns the conversion value.
 *
 * @return 10-bit ADC result.
 */
u16 ADC_GetRead(void);

/**
 * @brief Periodically checks ADC conversion result.
 *
 * This function checks if the ADC conversion has finished.
 * If finished, the result is written to the provided pointer.
 *
 * @param pdata Pointer to store ADC result.
 *
 * @return error_t
 *         - OK if conversion finished
 *         - IN_PROGRESS if conversion still running
 */
error_t ADC_GetReadPeriodic(u16*pdata);
/**
 * @brief Reads ADC data register without checking conversion status.
 *
 * This function simply returns the current value
 * of the ADC data register.
 *
 * @return ADC data register value.
 */
u16 ADC_GetReadNoBlock(void);

/**
 * @brief Enables the ADC interrupt.
 *
 * This function enables the ADC Conversion Complete Interrupt.
 * When the ADC finishes a conversion, the ADC interrupt will be triggered
 * and the user-defined callback function will be executed.
 */
void ADC_InterruptEnable(void);

/**
 * @brief Sets the callback function for the ADC interrupt.
 *
 * This function registers a user-defined function that will be called
 * automatically when the ADC conversion complete interrupt occurs.
 *
 * @param LocalFptr Pointer to the callback function.
 *
 * @note The callback function must take no parameters and return void.
 */
void ADC_SetCallBack(void (*LocalFptr)(void));
#endif /* ADC_INT_H_ */