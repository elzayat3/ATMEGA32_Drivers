# AVR Drivers Library

A collection of reusable **low-level drivers for AVR microcontrollers**, implemented in C using a modular **MCAL-style architecture**.

This repository contains multiple drivers designed to simplify interaction with AVR hardware peripherals while keeping the application layer independent from direct register manipulation.

The goal of this project is to build a structured **embedded driver library** following good embedded software practices such as modular design, configuration separation, and clear APIs.

---

# Target Microcontroller

Primary target:

**ATmega32**

Architecture: **AVR 8-bit**

All drivers are written to interact directly with AVR hardware registers defined in the memory map.

---

# Repository Structure

```id="a8g3e5"
AVR_Drivers
│
├── MCAL
│   ├── DIO
│   │   ├── DIO_Int.h
│   │   ├── DIO_Private.h
│   │   └── DIO_Program.c
│   │
│   └── (Future Drivers)
│       ├── UART
│       ├── SPI
│       ├── ADC
│       └── TIMER
│
├── cfg
│   └── DIO
│       ├── DIO_Cfg.c
│       └── DIO_Cfg.h
│
└── README.md
```

---

# Implemented Drivers

## DIO Driver (GPIO)

Provides APIs to control digital input/output pins of the ATmega32.

Features:

* Pin initialization through configuration array
* Digital read/write operations
* Port read/write operations
* Pin toggle functionality
* Support for all 32 GPIO pins

Driver Location:

```
MCAL/DIO
```

Configuration Location:

```
cfg/DIO
```

---

# Driver Architecture

The drivers follow a layered architecture commonly used in embedded systems.

```
Application Layer
        │
        ▼
    MCAL Drivers
        │
        ▼
 Hardware Registers
```

Applications interact only with the driver APIs while the drivers handle the low-level register operations.

---

# Design Principles

The drivers in this repository follow several embedded software design principles:

* Hardware abstraction
* Modular driver design
* Separation between driver code and configuration
* Readable and reusable APIs
* Structured project organization

---

# Future Drivers

Planned drivers for this library include:

* UART Driver
* SPI Driver
* I2C (TWI) Driver
* ADC Driver
* Timer Driver
* External Interrupt Driver

---

# Author

Abdelrahman Elzayat
Embedded Systems Developer
