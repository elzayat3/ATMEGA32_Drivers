# ADC Driver (ATmega32)

Analog to Digital Converter (ADC) driver for the **ATmega32 AVR microcontroller** implemented as part of the **MCAL (Microcontroller Abstraction Layer)**.

The driver provides a clean interface to perform analog-to-digital conversions with support for **blocking**, **non-blocking**, and **periodic polling** methods.

---

# Driver Location

```
MCAL/ADC
```

Configuration files (if used) are located in:

```
CFG/ADC
```

---

# Features

* Support for all **8 ADC channels**
* Configurable **Voltage Reference**
* Configurable **ADC Prescaler**
* **Blocking ADC read**
* **Non-blocking ADC conversion**
* **Periodic polling read**
* **Interrupt-based ADC conversion**
* **Callback mechanism for ADC interrupt**
* Convert ADC value to **millivolts**

---

# Supported Voltage References

| Reference   | Description                  |
| ----------- | ---------------------------- |
| `VREF_AREF` | External AREF pin            |
| `VREF_VCC`  | AVCC with external capacitor |
| `VREF_256`  | Internal 2.56V reference     |

---

# ADC Channels

| Channel | Pin  |
| ------- | ---- |
| CH_0    | ADC0 |
| CH_1    | ADC1 |
| CH_2    | ADC2 |
| CH_3    | ADC3 |
| CH_4    | ADC4 |
| CH_5    | ADC5 |
| CH_6    | ADC6 |
| CH_7    | ADC7 |

---

# ADC Prescaler

The ADC clock is derived from the system clock using a prescaler.

| Prescaler      | Division |
| -------------- | -------- |
| ADC_SCALER_2   | clk/2    |
| ADC_SCALER_4   | clk/4    |
| ADC_SCALER_8   | clk/8    |
| ADC_SCALER_16  | clk/16   |
| ADC_SCALER_32  | clk/32   |
| ADC_SCALER_64  | clk/64   |
| ADC_SCALER_128 | clk/128  |

---

# Driver APIs

## Initialize ADC

```
void ADC_Init(ADC_VREF_t vref, ADC_Prescaler_t scaler);
```

Configures the ADC reference voltage and clock prescaler.

---

## Blocking ADC Read

```
u16 ADC_Read(ADC_Channel_t ch);
```

Performs a complete ADC conversion and waits until the conversion is finished.

Returns the **10-bit ADC result**.

---

## Read Voltage

```
u16 ADC_ReadVolt(ADC_Channel_t ch);
```

Performs an ADC conversion and returns the **voltage in millivolts**.

---

## Start Conversion (Non-Blocking)

```
void ADC_StartConversion(ADC_Channel_t ch);
```

Starts the ADC conversion without waiting for completion.

---

## Get Conversion Result

```
u16 ADC_GetRead(void);
```

Waits until the conversion finishes and returns the ADC result.

---

## Periodic Read

```
error_t ADC_GetReadPeriodic(u16 *pdata);
```

Checks conversion status without blocking.

Return values:

| Value       | Meaning                  |
| ----------- | ------------------------ |
| OK          | Conversion finished      |
| IN_PROGRESS | Conversion still running |

---

## Read ADC Register Directly

```
u16 ADC_GetReadNoBlock(void);
```

Returns the ADC register value without checking conversion status.

---

## ADC Interrupt Enable

```
void ADC_InterruptEnable(void);
```
Enables the ADC Conversion Complete Interrupt.

When the ADC finishes a conversion, an interrupt will be triggered.


## Set ADC Callback Function

```
void ADC_SetCallBack(void (*LocalFptr)(void));
```
Registers a callback function that will be executed when the ADC conversion is completed.

| Parameter | Description |
|----------|-------------|
| `LocalFptr` | Pointer to callback function executed after ADC conversion completes. |

# Interrupt Usage Example

```c
#include "ADC_Int.h"

void ADC_Done(void)
{
    u16 value = ADC_GetReadNoBlock();
}

int main(void)
{
    ADC_Init(VREF_VCC, ADC_SCALER_64);

    ADC_SetCallBack(ADC_Done);
    ADC_InterruptEnable();

    ADC_StartConversion(CH_0);

    while(1)
    {

    }
}
```


# Example Usage

```c
#include "ADC_Int.h"

int main(void)
{
    ADC_Init(VREF_VCC, ADC_SCALER_64);

    while(1)
    {
        u16 adc_value = ADC_Read(CH_0);
    }
}
```

Example reading voltage:

```c
u16 voltage = ADC_ReadVolt(CH_0);
```

---

# Driver Architecture

The ADC driver is implemented as part of the **MCAL layer**.

```
Application Layer
        │
        ▼
    ADC Driver
        │
        ▼
 Hardware Registers
```

The application interacts only with the driver APIs while the driver handles direct register access.

---

# Author

Abdelrahman Elzayat
Embedded Systems Developer
