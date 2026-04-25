# ATmega32 Drivers Library

A collection of reusable **low-level drivers for the ATmega32 AVR microcontroller**, implemented in C using a modular **MCAL (Microcontroller Abstraction Layer)** architecture.

This repository provides a set of peripheral drivers designed to simplify hardware interaction while keeping the application layer independent from direct register manipulation.

The project demonstrates structured embedded driver development including:

* Hardware abstraction
* Modular driver architecture
* Clean and reusable APIs
* Configuration separation (MCAL / CFG)
* Service layer support for higher-level communication features

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
│   ├── EXIU
│   │   ├── EXIU_Int.h
│   │   ├── EXIU_Private.h
│   │   └── EXIU_Program.c
│   │
│   ├── TIMER
│   │   ├── TIMER_Int.h
│   │   ├── TIMER_Private.h
│   │   └── TIMER_Program.c
│   │
│   └── UART
│       ├── UART_Int.h
│       ├── UART_Private.h
│       └── UART_Prg.c
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
│   ├── EXIU
│   │   ├── EXIU_Cfg.c
│   │   └── EXIU_Cfg.h
│   │
│   ├── TIMER
│   │   ├── TIMER_Cfg.c
│   │   └── TIMER_Cfg.h
│   │
│   ├── UART
│   │   ├── UART_Cfg.c
│   │   └── UART_Cfg.h
│   │
│   └── SUART
│       ├── SUART_Cfg.c
│       └── SUART_Cfg.h
│
├── SERVICE
│   └── SUART
│       ├── SUART_Int.h
│       ├── SUART_Private.h
│       ├── SUART_Prg.c
│       └── README.md
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

```text
MCAL/DIO
```

### Configuration

```text
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

```text
MCAL/ADC
```

### Configuration

```text
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

```text
MCAL/EXIU
```

### Configuration

```text
CFG/EXIU
```

---

## 🔹 TIMER Driver

Provides APIs for configuring and controlling the internal timers of ATmega32.

### Features

* Support for all AVR timers:
  * **Timer0** (8-bit)
  * **Timer1** (16-bit)
  * **Timer2** (8-bit)

* Support for multiple operating modes, including:
  * **Normal Mode**
  * **CTC Mode**
  * **Fast PWM Mode**
  * **Phase Correct PWM Mode**

* Timer1 advanced support for programmable TOP selection using:
  * **ICR1**
  * **OCR1A**

* Configurable clock source / prescaler
* Configurable output compare behavior for:
  * **OC0**
  * **OC1A**
  * **OC1B**
  * **OC2**
* Start / Stop control APIs for each timer
* Access APIs/macros for timer registers such as:
  * Counter register read/write
  * Compare register read/write
  * Timer1 TOP register read/write
* Modular configuration-based initialization
* Clean abstraction over timer register manipulation

### Location

```text
MCAL/TIMER
```

### Configuration

```text
CFG/TIMER
```

---

## 🔹 UART Driver

Provides APIs for configuring and controlling the USART peripheral of ATmega32.

### Features

* Configuration-based initialization using `UART_Config_t`
* Configurable UART parameters:
  * Communication mode
  * Speed mode
  * Parity
  * Stop bits
  * Character size
  * Baud rate
  * TX/RX enable control
* Blocking byte transmission and reception
* Non-blocking byte transmission and reception
* Direct send/receive APIs for controlled interrupt-driven use cases
* RX, TX, and UDRE interrupt enable/disable APIs
* Callback registration for:
  * RX Complete interrupt
  * TX Complete interrupt
  * Data Register Empty interrupt
* ISR-based callback handling for interrupt-driven communication

### Notes

* Current baud-rate enum values are precomputed **UBRR** values for:
  * **F_CPU = 8 MHz**
  * **Asynchronous mode**
  * **Normal speed mode (U2X = 0)**
* If CPU frequency or speed mode changes, baud-rate values must be recalculated.
* `UART_SendDirect()` and `UART_ReceiveDirect()` are intended for controlled use cases such as ISR-driven communication.
* In the current RX interrupt design, the registered RX callback is responsible for reading `UDR`.
* In the current UDRE interrupt design, the registered callback should either:
  * write the next byte to `UDR`, or
  * disable UDRE interrupt when transmission is complete.

### Location

```text
MCAL/UART
```

### Configuration

```text
CFG/UART
```

---

## 🔹 UART Service Layer (SUART)

Provides a lightweight higher-level UART service built on top of the UART MCAL driver.

### Features

* Asynchronous transmission of **null-terminated strings**
* FIFO **TX queue** for multiple pending string requests
* Interrupt-driven TX using **UART UDRE interrupt**
* Asynchronous reception into a **user-provided buffer**
* Fixed-capacity RX request model
* RX completion and TX completion callbacks
* Simple API focused on practical application use

### Notes

* `SUART_SendAsync()` accepts a pointer to a null-terminated string.
* TX queue stores **string pointers only**, so the source string must remain valid until transmission is completed.
* `SUART_ReceiveAsync()` takes a buffer pointer and the **total buffer size**.
* RX reserves the **last byte** of the buffer for `\0`.
* Therefore, if the buffer size is `N`, the number of received characters is `N - 1`.
* Only **one RX request** can be active at a time.
* If an RX request is already active, `SUART_ReceiveAsync()` returns `IN_PROGRESS`.

### Location

```text
SERVICE/UART
```

### Configuration

```text
CFG/SUART
```

---

# Driver Architecture

The repository follows a layered architecture commonly used in embedded systems:

```text
Application Layer
        │
        ▼
   Service Layer
        │
        ▼
    MCAL Drivers
        │
        ▼
 Hardware Registers
```

Applications may use either the MCAL APIs directly or higher-level service modules when needed.

---

# Configuration Concept

Each driver is divided into:

* **MCAL/** → Driver implementation (reusable)
* **CFG/** → User configuration (project-specific)
* **SERVICE/** → Higher-level services built on top of MCAL drivers

Example (EXTI):

```c
EXTI_Config_t EXTI_ConfigArr[EXTI_CONFIG_SIZE] =
{
    {EX_INT0, FALLING_EDGE, EXTI_ENABLE},
    {EX_INT1, RISING_EDGE,  EXTI_ENABLE},
    {EX_INT2, FALLING_EDGE, EXTI_DISABLE}
};
```

Example (TIMER):

```c
TIMER0_Config_t timer0_cfg =
{
    /* user-defined Timer0 mode, prescaler, compare behavior, preload, compare value */
};

TIMER1_Config_t timer1_cfg =
{
    /* user-defined Timer1 mode, prescaler, compare output behavior, initial value, compare values, top value */
};

TIMER2_Config_t timer2_cfg =
{
    /* user-defined Timer2 mode, prescaler, compare behavior, preload, compare value */
};
```

Example (UART):

```c
const UART_Config_t UART_Config =
{
    .mode      = UART_ASYNCHRONOUS_MODE,
    .speed     = UART_NORMAL_SPEED,
    .parity    = UART_PARITY_DISABLED,
    .stop_bits = UART_ONE_STOP_BIT,
    .char_size = UART_8_BIT_CHAR,
    .baud_rate = UART_BAUD_9600,
    .tx_enable = TRUE,
    .rx_enable = TRUE
};
```

Example (SUART):

```c
#define SUART_TX_QUEUE_SIZE    5U
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

# Example Usage (TIMER)

```c
#include "TIMER_Int.h"

int main(void)
{
    TIMER_Init();
    TIMER_Start(TIMER0);

    while(1)
    {
    }
}
```

---

# Example Usage (UART)

```c
#include "UART_Int.h"

int main(void)
{
    UART_Init();

    UART_Send('A');

    while(1)
    {
        /* Application loop */
    }
}
```

Example (UART interrupt-driven receive):

```c
#include "UART_Int.h"

static volatile u8 Global_u8ReceivedData = 0U;

void UART_RxTask(void)
{
    Global_u8ReceivedData = UART_ReceiveDirect();
}

int main(void)
{
    UART_Init();
    UART_RX_SetCallBack(UART_RxTask);
    UART_RX_InterruptEnable();
    GLOBAL_ENABLE();

    while(1)
    {
        /* Application loop */
    }
}
```

---

# Example Usage (SUART)

```c
#include "UART_Int.h"
#include "SUART_Int.h"

static const c8 Msg1[] = "HELLO";
static const c8 Msg2[] = "DONE";
static c8 RxBuffer[8];

void TxDone(void)
{
    /* One queued string finished transmission */
}

void RxDone(void)
{
    /* RX request completed */
}

int main(void)
{
    UART_Init();
    SUART_Init();

    SUART_TX_SetCallBack(TxDone);
    SUART_RX_SetCallBack(RxDone);

    GLOBAL_ENABLE();

    SUART_SendAsync(Msg1);
    SUART_SendAsync(Msg2);
    SUART_ReceiveAsync(RxBuffer, sizeof(RxBuffer));

    while(1)
    {
        /* Application loop */
    }
}
```

In this example:

* `Msg1` and `Msg2` are queued and transmitted in order.
* `RxBuffer` has total size `8`, so SUART receives **7 characters** and reserves the last byte for `\0`.

---

# Design Principles

Drivers in this repository follow key embedded software practices:

* Hardware abstraction (no direct register access in application)
* Modular driver design
* Separation of configuration and logic
* Reusable MCAL and service-layer architecture
* No magic numbers (enums & macros used)
* Scalable and reusable architecture
* Consistent coding style across all drivers

---

# Future Drivers

Planned drivers to be added:

* SPI Driver
* I2C (TWI) Driver

---

# Author

**Abdelrahman Elzayat**  
Embedded Systems Developer
