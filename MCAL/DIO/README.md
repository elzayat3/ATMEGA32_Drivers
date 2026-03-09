# ATmega32 DIO Driver (GPIO Driver)

A configurable **Digital Input/Output (DIO) driver** for the **ATmega32 AVR microcontroller**, implemented in C using a modular **MCAL-style architecture**.

The driver abstracts direct register manipulation and provides a clean API for controlling GPIO pins and ports.
Pin configuration is separated from the driver implementation to allow flexible project-level customization.

---

# Microcontroller

Target MCU: **ATmega32**

Available GPIO Ports:

* PORTA
* PORTB
* PORTC
* PORTD

Total Pins: **32 GPIO Pins**

---

# Driver Architecture

The driver follows a layered embedded architecture:

```
Application
    │
    ▼
DIO Driver (MCAL)
    │
    ▼
Hardware Registers
```

The application interacts with the **driver API**, while the driver internally manages register access.

---

# Project Structure

```
Project
│
├── MCAL
│   └── DIO
│       ├── DIO_Int.h
│       ├── DIO_Private.h
│       └── DIO_Program.c
│
└── cfg
    └── DIO
        ├── DIO_Cfg.c
        └── DIO_Cfg.h
```

### Description

**MCAL/DIO**

* `DIO_Int.h`
  Public driver interface and user APIs.

* `DIO_Private.h`
  Internal driver definitions and helper functions.

* `DIO_Program.c`
  Implementation of the driver logic.

**cfg/DIO**

* `DIO_Cfg.c`
  Contains the pin configuration array.

* `DIO_Cfg.h`
  Configuration header for the driver.

Separating configuration from the driver allows the same driver to be reused across different projects.

---

# Pin Configuration

GPIO pin configuration is defined in:

```
cfg/DIO/DIO_Cfg.c
```

Pins are configured using the array:

```c
const DIO_Status_t PinsStatusArr[TOTAL_PINS];
```

Example:

```c
const DIO_Status_t PinsStatusArr[TOTAL_PINS] =
{
    INFREE,   /* PA0 */
    OUTPUT,   /* PA1 */
    OUTPUT,   /* PA2 */
    OUTPUT,   /* PA3 */
};
```

### Available Modes

| Mode   | Description                 |
| ------ | --------------------------- |
| OUTPUT | Output pin                  |
| INFREE | Input floating              |
| INPULL | Input with internal pull-up |

---

# Driver APIs

### Initialize Driver

```c
void DIO_Init(void);
```

Initializes all pins according to the configuration array.

---

### Write to Pin

```c
void DIO_WritePin(DIO_Pin_t pin, DIO_Voltage_t volt);
```

Sets a pin output level.

Parameters:

* `pin` → Target pin
* `volt` → `HIGH` or `LOW`

---

### Read Pin

```c
DIO_Voltage_t DIO_ReadPin(DIO_Pin_t pin);
```

Reads the logic level of a pin.

---

### Toggle Pin

```c
void DIO_ToggelPin(DIO_Pin_t pin);
```

Toggles the current state of the pin.

---

### Write Port

```c
void DIO_WritePort(DIO_Port_t port, u8 data);
```

Writes an 8-bit value to a GPIO port.

---

### Read Port

```c
u8 DIO_ReadPort(DIO_Port_t port);
```

Reads the current value of a GPIO port.

---

# Example Usage

```c
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

# Design Highlights

This driver demonstrates common embedded driver design practices:

* Hardware abstraction layer
* Configuration-driven initialization
* Separation between driver and configuration
* Modular embedded driver architecture
* Pin and port abstraction

---

# Author

Abdelrahman Elzayat
Embedded Systems Developer
