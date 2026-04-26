# SPI Driver for ATmega32

A lightweight **Serial Peripheral Interface (SPI) MCAL driver** for the **ATmega32 AVR microcontroller**.

This driver provides a clean API for initializing and using the SPI peripheral in both **Master** and **Slave** modes. It supports blocking transfer APIs, non-blocking polling APIs, interrupt control, and callback registration.

---

## Target Microcontroller

**ATmega32**

The SPI peripheral is accessed directly through the register definitions provided in `MemMap.h`.

---

## Driver File Structure

```text
ATMEGA32_Drivers
│
├── MCAL
│   └── SPI
│       ├── SPI_Int.h
│       ├── SPI_Private.h
│       └── SPI_Program.c
│
├── CFG
│   └── SPI
│       ├── SPI_Cfg.h
│       └── SPI_Cfg.c
│
├── MemMap.h
└── StdTypes.h
```

### File Description

| File | Description |
|---|---|
| `SPI_Int.h` | Public interface of the SPI driver. Contains enums, configuration structure, callback typedef, and public APIs. |
| `SPI_Private.h` | Private macros and helper definitions used only inside the SPI driver. |
| `SPI_Cfg.h` | External declaration of the SPI configuration object. |
| `SPI_Cfg.c` | User configuration source file for SPI mode, data order, clock polarity, clock phase, clock rate, and interrupt state. |
| `SPI_Program.c` | SPI driver implementation source file. |
| `MemMap.h` | Contains SPI register addresses, SPI bit definitions, and interrupt vector definitions. |
| `StdTypes.h` | Contains standard types, `error_t`, `bool_t`, and bit manipulation macros. |

---

## SPI Pins on ATmega32

| SPI Signal | ATmega32 Pin | Description |
|---|---|---|
| `SS` | `PB4` | Slave Select |
| `MOSI` | `PB5` | Master Out Slave In |
| `MISO` | `PB6` | Master In Slave Out |
| `SCK` | `PB7` | Serial Clock |

---

## Pin Direction Configuration

The SPI driver does **not** configure DIO pins internally.  
SPI pins must be configured by the application using the DIO driver before calling `SPI_Init()`.

### Master Mode Pin Direction

| Pin | Direction |
|---|---|
| `SS / PB4` | Output |
| `MOSI / PB5` | Output |
| `MISO / PB6` | Input |
| `SCK / PB7` | Output |

### Slave Mode Pin Direction

| Pin | Direction |
|---|---|
| `SS / PB4` | Input |
| `MOSI / PB5` | Input |
| `MISO / PB6` | Output |
| `SCK / PB7` | Input |

> **Important:**  
> In Master mode, the `SS` pin should be configured as output.  
> If `SS` is configured as input and driven low while SPI is in Master mode, the hardware clears the `MSTR` bit automatically and switches the SPI peripheral to Slave mode.

---

## Features

- Supports Master mode
- Supports Slave mode
- Supports MSB first and LSB first data order
- Supports SPI clock polarity configuration
- Supports SPI clock phase configuration
- Supports SPI clock prescaler selection
- Supports blocking byte transfer
- Supports non-blocking polling byte transfer
- Supports SPI interrupt enable and disable
- Supports SPI transfer complete callback
- Uses timeout protection in blocking transfer
- Keeps SPI pin configuration separated from the SPI driver

---

## Configuration

The SPI driver is configured through the global configuration object defined in `SPI_Cfg.c`.

Example:

```c
const SPI_Config_t SPI_Config =
{
    .mode = SPI_MASTER_MODE,
    .data_order = SPI_MSB_FIRST,
    .clock_polarity = SPI_IDLE_LOW,
    .clock_phase = SPI_SAMPLE_LEADING,
    .clock_rate = SPI_CLOCK_DIV_16,
    .interrupt_enable = FALSE
};
```

---

## Configuration Options

### SPI Mode

```c
SPI_SLAVE_MODE
SPI_MASTER_MODE
```

### Data Order

```c
SPI_MSB_FIRST
SPI_LSB_FIRST
```

### Clock Polarity

```c
SPI_IDLE_LOW
SPI_IDLE_HIGH
```

### Clock Phase

```c
SPI_SAMPLE_LEADING
SPI_SAMPLE_TRAILING
```

### Clock Rate

Clock rate selection is effective only in Master mode.

```c
SPI_CLOCK_DIV_4
SPI_CLOCK_DIV_16
SPI_CLOCK_DIV_64
SPI_CLOCK_DIV_128
SPI_CLOCK_DIV_2
SPI_CLOCK_DIV_8
SPI_CLOCK_DIV_32
```

For `F_CPU = 8 MHz`, the maximum SPI clock is:

```text
F_CPU / 2 = 4 MHz
```

---

## Public APIs

### Initialization and Control

```c
error_t SPI_Init(void);

void SPI_Enable(void);
void SPI_Disable(void);
```

### Blocking APIs

```c
error_t SPI_SendByte(uint8_t data);

error_t SPI_ReceiveByte(uint8_t *data);

error_t SPI_TransceiveByte(uint8_t tx_data, uint8_t *rx_data);
```

### Non-Blocking Polling APIs

```c
error_t SPI_SendByteNoBlock(uint8_t data);

error_t SPI_ReceiveByteNoBlock(uint8_t *data);

error_t SPI_TransceiveByteNoBlock(uint8_t tx_data, uint8_t *rx_data);
```

### Interrupt APIs

```c
void SPI_InterruptEnable(void);

void SPI_InterruptDisable(void);

error_t SPI_SetCallBack(SPI_Callback_t callback);
```

---

## Return Values

The driver uses `error_t` from `StdTypes.h`.

| Return Value | Meaning |
|---|---|
| `OK` | Operation completed successfully. |
| `NOK` | Invalid configuration or generic error. |
| `NULL_PTR` | Null pointer is passed to a function. |
| `IN_PROGRESS` | Non-blocking operation is still in progress. |
| `TIMEOUT` | Blocking operation exceeded timeout limit. |

---

## Blocking Transfer Behavior

SPI is full-duplex.  
Each SPI transfer sends and receives data at the same time.

The main blocking API is:

```c
error_t SPI_TransceiveByte(uint8_t tx_data, uint8_t *rx_data);
```

`SPI_SendByte()` is implemented by transmitting data and ignoring the received byte.

```c
error_t SPI_SendByte(uint8_t data);
```

`SPI_ReceiveByte()` is implemented by sending a dummy byte to generate the SPI clock and receive data.

```c
error_t SPI_ReceiveByte(uint8_t *data);
```

The dummy byte is defined privately inside `SPI_Private.h`.

---

## Non-Blocking Polling Behavior

The non-blocking APIs are designed to be called repeatedly.

Example behavior of `SPI_SendByteNoBlock()`:

```text
First call:
- Starts SPI transfer
- Returns IN_PROGRESS

Next calls:
- Returns IN_PROGRESS while transfer is still running
- Returns OK when transfer is complete
```

> **Note:**  
> Non-blocking polling APIs should not be mixed with SPI interrupt mode.  
> If SPI interrupt is enabled, the ISR may clear the busy state before the polling API reads `SPDR`.

---

## Interrupt Behavior

The SPI interrupt is triggered when SPI transfer is complete.

To use interrupt mode:

1. Register a callback using `SPI_SetCallBack()`.
2. Enable SPI interrupt using `SPI_InterruptEnable()` or through `SPI_Config`.
3. Enable global interrupts using `sei()`.

Example:

```c
void SPI_TransferCompleteCallback(void)
{
    /* User code */
}

int main(void)
{
    SPI_Init();

    SPI_SetCallBack(SPI_TransferCompleteCallback);

    SPI_InterruptEnable();

    sei();

    while (1)
    {
        /* Application code */
    }
}
```

> **Important:**  
> `SPI_InterruptEnable()` enables only the SPI peripheral interrupt.  
> Global interrupt must be enabled separately using `sei()`.

---

## Usage Example: Master Blocking Transfer

```c
#include "StdTypes.h"
#include "DIO_Int.h"
#include "SPI_Int.h"

int main(void)
{
    uint8_t received_data = 0u;

    /*
     * Configure SPI pins using DIO driver before SPI_Init().
     * Master mode:
     * SS   -> Output
     * MOSI -> Output
     * MISO -> Input
     * SCK  -> Output
     */

    DIO_InitPin(PINB4, OUTPUT);
    DIO_InitPin(PINB5, OUTPUT);
    DIO_InitPin(PINB6, INFREE);
    DIO_InitPin(PINB7, OUTPUT);

    DIO_WritePin(PINB4, HIGH);

    SPI_Init();

    SPI_TransceiveByte(0x55u, &received_data);

    while (1)
    {
        /* Application loop */
    }
}
```

---

## Usage Example: Receive Byte in Master Mode

```c
uint8_t data = 0u;

SPI_ReceiveByte(&data);
```

In Master mode, this function sends a dummy byte internally to generate the SPI clock.

---

## Usage Example: Non-Blocking Polling

```c
uint8_t rx_data = 0u;
error_t state = IN_PROGRESS;

state = SPI_TransceiveByteNoBlock(0x55u, &rx_data);

if (state == OK)
{
    /* Transfer completed and rx_data is ready */
}
else if (state == IN_PROGRESS)
{
    /* Call the function again later */
}
else
{
    /* Error handling */
}
```

---

## Usage Notes

- Configure SPI pins using DIO before calling `SPI_Init()`.
- In Master mode, configure `SS` as output to avoid accidental switching to Slave mode.
- Blocking APIs use polling and timeout protection.
- Blocking APIs are simple and suitable for short transfers.
- For long transfers in RTOS-based applications, consider building a service layer using mutexes, semaphores, or callbacks.
- Do not mix polling non-blocking APIs with SPI interrupt mode.
- Keep `interrupt_enable = FALSE` when using blocking or polling APIs only.

---

## Recommended Service Layer Usage

For a simple SPI service layer, it is recommended to use the blocking APIs:

```c
SPI_SendByte();
SPI_ReceiveByte();
SPI_TransceiveByte();
```

This keeps the service layer easy to use and avoids exposing low-level SPI state handling to the application.

For advanced RTOS usage, a future service layer can be built using:

- SPI interrupt
- Callback
- Semaphore
- Mutex
- Queue

---

## Author

**Abdelrahman Elzayat**
