# DIO Driver (ATmega32)

Digital Input/Output driver for **ATmega32 AVR microcontroller** implemented as part of the **MCAL layer**.

This driver provides an abstraction layer for controlling GPIO pins and ports without directly accessing hardware registers in the application layer.

---

# Driver Location

```
MCAL/DIO
```

Configuration files are located in:

```
CFG/DIO
```

---

# Driver Features

* Configure GPIO pins as:

  * Output
  * Input Floating
  * Input Pull-up

* Read and write digital pins

* Toggle pin state

* Read/write entire ports

* Support for all **32 GPIO pins**

---

# File Structure

```
MCAL/DIO
│
├── DIO_Int.h
├── DIO_Private.h
└── DIO_Program.c
```

### Description

**DIO_Int.h**

Public driver interface containing:

* API declarations
* enums
* user types

**DIO_Private.h**

Internal driver definitions and helper functions.

**DIO_Program.c**

Driver implementation including:

* Pin initialization
* Pin read/write
* Port operations

---

# Configuration

GPIO configuration is separated from the driver and located in:

```
CFG/DIO/DIO_Cfg.c
```

Pins are configured using:

```
PinsStatusArr[TOTAL_PINS]
```

Example:

```c
const DIO_Status_t PinsStatusArr[TOTAL_PINS] =
{
    OUTPUT,  /* PA0 */
    OUTPUT,  /* PA1 */
    INPULL,  /* PA2 */
};
```

### Available Pin Modes

| Mode   | Description                 |
| ------ | --------------------------- |
| OUTPUT | Output mode                 |
| INFREE | Input floating              |
| INPULL | Input with internal pull-up |

---

# Driver APIs

Initialize driver

```
void DIO_Init(void);
```

Write pin

```
void DIO_WritePin(DIO_Pin_t pin, DIO_Voltage_t volt);
```

Read pin

```
DIO_Voltage_t DIO_ReadPin(DIO_Pin_t pin);
```

Toggle pin

```
void DIO_ToggelPin(DIO_Pin_t pin);
```

Write port

```
void DIO_WritePort(DIO_Port_t port, u8 data);
```

Read port

```
u8 DIO_ReadPort(DIO_Port_t port);
```

---

# Example Usage

```
#include "DIO_Int.h"

int main(void)
{
    DIO_Init();

    while(1)
    {
        DIO_ToggelPin(PINA1);
    }
}
```

---

# Author

Abdelrahman Elzayat
Embedded Systems Developer
