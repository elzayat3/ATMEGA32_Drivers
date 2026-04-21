/**

* @file    ADC_Private.h
* @author  Abdelrahman Elzayat
* @brief   Private definitions for ADC driver (ATmega32)
*
* @details
* This file contains internal macros and definitions used only inside
* the ADC driver implementation. It should not be included by the application layer.
  */

#ifndef ADC_PRIVATE_H_
#define ADC_PRIVATE_H_

/**

* @brief ADC reference voltage in millivolts
*
* @details
* This macro defines the reference voltage used in ADC calculations,
* especially for converting ADC digital values to millivolts.
*
* Default value: 5000 mV (VCC = 5V)
*
* @note
* * Modify this value according to the actual reference voltage used.
* * If using internal 2.56V reference, update it to 2560.
    */
    #define ADC_REF_MV   5000UL

#endif /* ADC_PRIVATE_H_ */
