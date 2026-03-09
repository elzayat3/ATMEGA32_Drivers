# ATmega32 Drivers Library

A collection of reusable **low-level drivers for the ATmega32 AVR microcontroller**, implemented in C using a modular **MCAL (Microcontroller Abstraction Layer)** architecture.

This repository provides a set of peripheral drivers designed to simplify hardware interaction while keeping the application layer independent from direct register manipulation.

The project demonstrates structured embedded driver development including:

* Hardware abstraction
* Modular driver architecture
* Clean and reusable APIs
* Configuration separation

---

# Target Microcontroller

**ATmega32**

Architecture: **8-bit AVR**

Peripherals are controlled through direct register access using the memory map defined in `MemMap.h`.

---

# Repository Structure

```id="s9t6j7"
ATMEGA32_Drivers
│
├── MCAL
│   ├── DIO
│   │   ├── DIO_Int.h
│   │   ├── DIO_Private.h
│   │   └── DIO_Program.c
│   │
│   └── ADC
│       ├── ADC_Int.h
│       ├── ADC_Private.h
│       └── ADC_Program.c
│
├── CFG
│   ├── DIO
│   │   ├── DIO_Cfg.c
│   │   └── DIO_Cfg.h
│   │
│   └── ADC
│       ├── ADC_Cfg.c
│       └── ADC_Cfg.h
│
├── MemMap.h
├── StdTypes.h
├── README.md
└── LICENSE
```

---

# Implemented Drivers

## DIO Driver (Digital Input Output)

Provides APIs for controlling GPIO pins and ports.

Features:

* Configure pin direction
* Input floating and pull-up modes
* Write/read digital pins
* Toggle pin state
* Port read/write operations

Location:

```id="q4g8vi"
MCAL/DIO
```

Configuration:

```id="p8i45u"
CFG/DIO
```

---

## ADC Driver (Analog to Digital Converter)

Provides APIs for performing analog-to-digital conversions.

Features:

* Support for **8 ADC channels**
* Configurable **voltage reference**
* Configurable **ADC prescaler**
* **Blocking ADC read**
* **Non-blocking conversion**
* **Periodic polling read**
* Convert ADC value to **millivolts**

Location:

```id="if4dfk"
MCAL/ADC
```

Configuration:

```id="7cc4et"
CFG/ADC
```

---

# Driver Architecture

The drivers follow a layered architecture commonly used in embedded systems.

```id="u1qvxy"
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

# Design Principles

Drivers in this repository follow several embedded software design practices:

* Hardware abstraction
* Modular driver design
* Separation between driver code and configuration
* Readable and reusable APIs
* Consistent coding structure across drivers

---

# Future Drivers

Planned drivers to be added:

* UART Driver
* SPI Driver
* I2C (TWI) Driver
* Timer Driver
* External Interrupt Driver
* PWM Driver

---

# Author

Abdelrahman Elzayat
Embedded Systems Developer
