# ATmega32 Drivers Library

A collection of reusable **low-level drivers for the ATmega32 AVR microcontroller**, implemented in C using a modular **MCAL (Microcontroller Abstraction Layer)** architecture.

This repository provides a set of peripheral drivers designed to simplify hardware interaction while keeping the application layer independent from direct register manipulation.

The project demonstrates structured embedded driver development including:

* Hardware abstraction
* Modular driver architecture
* Clean and reusable APIs
* Configuration separation (MCAL / CFG)

---

# Target Microcontroller

**ATmega32**

Architecture: **8-bit AVR**

Peripherals are controlled through direct register access using the memory map defined in `MemMap.h`.

---

# Repository Structure

```text
ATMEGA32_Drivers
│
├── MCAL
│   ├── DIO
│   │   ├── DIO_Int.h
│   │   ├── DIO_Private.h
│   │   └── DIO_Program.c
│   │
│   ├── ADC
│   │   ├── ADC_Int.h
│   │   ├── ADC_Private.h
│   │   └── ADC_Program.c
│   │
│   └── EXIU
│       ├── EXIU_Int.h
│       ├── EXIU_Private.h
│       └── EXIU_Program.c
│
├── CFG
│   ├── DIO
│   │   ├── DIO_Cfg.c
│   │   └── DIO_Cfg.h
│   │
│   ├── ADC
│   │   ├── ADC_Cfg.c
│   │   └── ADC_Cfg.h
│   │
│   └── EXIU
│       ├── EXIU_Cfg.c
│       └── EXIU_Cfg.h
│
├── MemMap.h
├── StdTypes.h
├── README.md
└── LICENSE
```

---

# Implemented Drivers

## 🔹 DIO Driver (Digital Input Output)

Provides APIs for controlling GPIO pins and ports.

### Features

* Configure pin direction (OUTPUT / INFREE / INPULL)
* Write/read digital pins
* Toggle pin state
* Full port read/write operations
* Initialization via configuration array

### Location

```
MCAL/DIO
```

### Configuration

```
CFG/DIO
```

---

## 🔹 ADC Driver (Analog to Digital Converter)

Provides APIs for performing analog-to-digital conversions.

### Features

* Support for **8 ADC channels**
* Configurable **voltage reference**
* Configurable **ADC prescaler**
* Blocking and non-blocking conversion
* Periodic polling read
* Convert ADC value to **millivolts**

### Location

```
MCAL/ADC
```

### Configuration

```
CFG/ADC
```

---

## 🔹 EXTI Driver (External Interrupt Unit)

Provides APIs for handling external interrupts **INT0, INT1, INT2**.

### Features

* Support for all external interrupts:

  * INT0 (PD2)
  * INT1 (PD3)
  * INT2 (PB2)
* Configurable trigger modes:

  * LOW_LEVEL
  * ANY_LOGIC_CHANGE
  * FALLING_EDGE
  * RISING_EDGE
* Enable / Disable control per interrupt
* Callback mechanism for ISR handling
* Configuration-based initialization (no hardcoding)
* Scalable design using callback array (no duplication)

### Location

```
MCAL/EXIU
```

### Configuration

```
CFG/EXIU
```

---

# Driver Architecture

The drivers follow a layered architecture commonly used in embedded systems:

```text
Application Layer
        │
        ▼
    MCAL Drivers
        │
        ▼
 Hardware Registers
```

Applications interact only with driver APIs while the drivers handle low-level register operations.

---

# Configuration Concept

Each driver is divided into:

* **MCAL/** → Driver implementation (reusable)
* **CFG/** → User configuration (project-specific)

Example (EXTI):

```c
EXTI_Config_t EXTI_ConfigArr[EXTI_CONFIG_SIZE] =
{
    {EX_INT0, FALLING_EDGE, EXTI_ENABLE},
    {EX_INT1, RISING_EDGE,  EXTI_ENABLE},
    {EX_INT2, FALLING_EDGE, EXTI_DISABLE}
};
```

---

# Example Usage (EXTI + DIO)

```c
#include "DIO_Int.h"
#include "EXIU_Int.h"

void led_toggle(void)
{
    DIO_ToggelPin(PINC0);
}

int main(void)
{
    /* Initialize GPIO (from config) */
    DIO_Init();

    /* Set interrupt callback */
    EXIU_SetCallback(EX_INT0, led_toggle);

    /* Initialize EXTI (from config) */
    EXIU_Init();

    /* Enable global interrupts */
    GLOBAL_ENABLE();

    while(1)
    {
    }
}
```

---

# Design Principles

Drivers in this repository follow key embedded software practices:

* Hardware abstraction (no direct register access in application)
* Modular driver design
* Separation of configuration and logic
* No magic numbers (enums & macros used)
* Scalable and reusable architecture
* Consistent coding style across all drivers

---

# Future Drivers

Planned drivers to be added:

* UART Driver
* SPI Driver
* I2C (TWI) Driver
* Timer Driver
* PWM Driver

---

# Author

**Abdelrahman Elzayat**
Embedded Systems Developer
