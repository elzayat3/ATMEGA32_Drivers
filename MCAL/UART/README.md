# ATmega32 UART Driver

A reusable **MCAL UART driver** for the **ATmega32** microcontroller, implemented in C using direct register access.

This driver provides a clean and modular interface for:

- UART initialization through a configuration object
- Blocking transmit and receive
- Non-blocking transmit and receive
- Interrupt control for RX, TX, and UDRE
- Callback registration for UART interrupts
- Direct register access APIs intended for controlled interrupt-driven use cases

The driver is designed to fit into a layered embedded architecture where:

- **MCAL** handles register-level byte-oriented UART operations
- **Service layer** can later build higher-level features such as string transmission, buffered communication, or protocol handling

---

# Target Microcontroller

**ATmega32**

Architecture: **8-bit AVR**

Communication peripheral: **USART / UART**

---

# Driver Files

```text
ATMEGA32_Drivers
├── MCAL
│   └── UART
│       ├── UART_Int.h
│       ├── UART_Private.h
│       └── UART_Prg.c
│
└── CFG
    └── UART
        ├── UART_Cfg.h
        └── UART_Cfg.c
```

## File Roles

### `MCAL/UART/UART_Int.h`
Contains:
- Public APIs
- Public configuration enums
- UART configuration structure
- UART callback typedef

### `MCAL/UART/UART_Private.h`
Contains:
- Internal helper macros
- Internal UART register configuration masks
- Private implementation support definitions

### `CFG/UART/UART_Cfg.h`
Contains:
- External declaration of the UART configuration object

### `CFG/UART/UART_Cfg.c`
Contains:
- User-selected UART configuration object used by `UART_Init()`

### `MCAL/UART/UART_Prg.c`
Contains:
- UART driver implementation
- Internal callback function pointers
- Interrupt service routines

---

# Current Supported Features

## 1) UART Initialization
The driver initializes UART using a configuration object of type `UART_Config_t`.

Supported configuration parameters:
- UART mode
- UART speed mode
- UART parity
- Stop bits
- Character size
- Baud rate
- Transmitter enable
- Receiver enable

## 2) Blocking APIs
The driver supports byte-based blocking communication:
- `UART_Send()`
- `UART_Receive()`

## 3) Non-Blocking APIs
The driver supports byte-based non-blocking communication:
- `UART_SendNoBlock()`
- `UART_ReceiveNoBlock()`

## 4) Interrupt Support
The driver supports enabling and disabling:
- RX Complete interrupt
- TX Complete interrupt
- Data Register Empty interrupt (UDRE)

## 5) Callback Registration
The driver provides callback registration APIs for:
- RX interrupt
- TX interrupt
- UDRE interrupt

## 6) Direct Access APIs
The driver includes direct register access helpers for controlled use cases:
- `UART_SendDirect()`
- `UART_ReceiveDirect()`

These APIs are mainly intended for interrupt-driven workflows.

---

# Public Types

## Callback Type

```c
typedef void (*UART_Callback_t)(void);
```

Used to register interrupt callback handlers.

---

## UART Mode

```c
typedef enum
{
    UART_ASYNCHRONOUS_MODE = 0,
    UART_SYNCHRONOUS_MODE
} UART_Mode_t;
```

---

## UART Speed

```c
typedef enum
{
    UART_NORMAL_SPEED = 0,
    UART_DOUBLE_SPEED
} UART_Speed_t;
```

---

## UART Parity

```c
typedef enum
{
    UART_PARITY_DISABLED = 0,
    UART_PARITY_EVEN = 2,
    UART_PARITY_ODD
} UART_Parity_t;
```

---

## UART Stop Bits

```c
typedef enum
{
    UART_ONE_STOP_BIT = 0,
    UART_TWO_STOP_BITS
} UART_StopBits_t;
```

---

## UART Character Size

```c
typedef enum
{
    UART_5_BIT_CHAR = 0,
    UART_6_BIT_CHAR,
    UART_7_BIT_CHAR,
    UART_8_BIT_CHAR,
    UART_9_BIT_CHAR = 7
} UART_CharSize_t;
```

---

## UART Baud Rate

The current baud-rate enum values are **precomputed UBRR values**.

These values are valid only when UART is configured as:
- **Asynchronous mode**
- **Normal speed mode** (`U2X = 0`)
- **`F_CPU = 8 MHz`**

```c
typedef enum
{
    UART_BAUD_2400   = 207,
    UART_BAUD_4800   = 103,
    UART_BAUD_9600   = 51,
    UART_BAUD_14400  = 34,
    UART_BAUD_19200  = 25,
    UART_BAUD_28800  = 16,
    UART_BAUD_38400  = 12,
    UART_BAUD_57600  = 8,
    UART_BAUD_76800  = 6,
    UART_BAUD_115200 = 3
} UART_BaudRate_t;
```

### Important Note
These values are **not valid** if:
- CPU frequency changes
- double-speed mode is used
- a different UART operating mode is selected

If support for other clock frequencies or double-speed mode is needed later, the UBRR values must be recalculated.

---

## UART Configuration Structure

```c
typedef struct
{
    UART_Mode_t mode;
    UART_Speed_t speed;
    UART_Parity_t parity;
    UART_StopBits_t stop_bits;
    UART_CharSize_t char_size;
    UART_BaudRate_t baud_rate;
    bool_t tx_enable;
    bool_t rx_enable;
} UART_Config_t;
```

---

# Configuration Example

`UART_Cfg.c`

```c
#include "UART_Cfg.h"

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

---

# Public APIs

## Initialization

```c
void UART_Init(void);
```

Initializes the UART peripheral using the selected configuration object.

---

## Blocking APIs

```c
void UART_Send(u8 Copy_u8Data);
u8 UART_Receive(void);
```

### `UART_Send()`
- waits until the transmit data register becomes empty
- writes one byte to `UDR`

### `UART_Receive()`
- waits until a byte is received
- reads one byte from `UDR`

---

## Non-Blocking APIs

```c
error_t UART_SendNoBlock(u8 Copy_u8Data);
error_t UART_ReceiveNoBlock(u8 *Add_pu8Data);
```

### `UART_SendNoBlock()`
- checks `UDRE`
- sends data only if the transmit data register is ready
- returns:
  - `OK`
  - `NOK`

### `UART_ReceiveNoBlock()`
- checks `RXC`
- receives data only if a new byte is available
- returns:
  - `OK`
  - `NOK`
  - `NULL_PTR`

---

## Direct APIs

```c
void UART_SendDirect(u8 Copy_u8Data);
u8 UART_ReceiveDirect(void);
```

These APIs access `UDR` directly without checking UART status flags.

### `UART_SendDirect()`
- writes directly to `UDR`
- should only be used when the transmitter is already ready
- intended mainly for controlled interrupt-driven transmission

### `UART_ReceiveDirect()`
- reads directly from `UDR`
- should only be used when data is already available
- intended mainly for controlled interrupt-driven reception

---

## Interrupt Control APIs

```c
void UART_RX_InterruptEnable(void);
void UART_RX_InterruptDisable(void);

void UART_TX_InterruptEnable(void);
void UART_TX_InterruptDisable(void);

void UART_UDRE_InterruptEnable(void);
void UART_UDRE_InterruptDisable(void);
```

---

## Callback Registration APIs

```c
error_t UART_RX_SetCallBack(UART_Callback_t Add_pfCallBack);
error_t UART_TX_SetCallBack(UART_Callback_t Add_pfCallBack);
error_t UART_UDRE_SetCallBack(UART_Callback_t Add_pfCallBack);
```

Return values:
- `OK`
- `NULL_PTR`

---

# Interrupt Behavior

## RX Interrupt

The RX ISR calls the registered RX callback.

```c
ISR(UART_RX_vect)
{
    if (UART_RX_Fptr != NULLPTR)
    {
        UART_RX_Fptr();
    }
}
```

### Important Note
The **RX callback is responsible for reading `UDR`**.

This is important because the receive condition is not considered handled until `UDR` is read.

Typical usage inside the callback:

```c
void UART_RX_Task(void)
{
    u8 Local_u8Data;
    Local_u8Data = UART_ReceiveDirect();
    /* process received data */
}
```

---

## TX Interrupt

The TX ISR calls the registered TX callback after transmission completion.

```c
ISR(UART_TX_vect)
{
    if (UART_TX_Fptr != NULLPTR)
    {
        UART_TX_Fptr();
    }
}
```

---

## UDRE Interrupt

The UDRE ISR calls the registered callback whenever `UDR` becomes empty and ready for a new byte.

```c
ISR(UART_UDRE_vect)
{
    if (UART_UDRE_Fptr != NULLPTR)
    {
        UART_UDRE_Fptr();
    }
}
```

### Important Note
Inside the UDRE callback, you should do one of the following:
- send the next byte using `UART_SendDirect()`
- disable UDRE interrupt when transmission is complete

If the callback does neither, the interrupt may continue triggering repeatedly.

Example:

```c
void UART_UDRE_Task(void)
{
    if (/* more data available */)
    {
        UART_SendDirect(NextByte);
    }
    else
    {
        UART_UDRE_InterruptDisable();
    }
}
```

---

# Internal Design Notes

## UCSRC / UBRRH Shared Address
In ATmega32, `UCSRC` and `UBRRH` share the same I/O address.

Therefore, when writing to `UCSRC`, the `URSEL` bit must be set.

The driver handles this internally using private helper macros.

---

## Character Size Handling
For character sizes from **5-bit to 8-bit**, the driver configures `UCSZ1:0` in `UCSRC` and clears `UCSZ2` in `UCSRB`.

For **9-bit mode**, the driver additionally sets `UCSZ2` in `UCSRB`.

### Current Limitation
The current public send/receive byte APIs are **8-bit oriented**:
- `UART_Send(u8)`
- `UART_Receive(void)`
- `UART_SendDirect(u8)`
- `UART_ReceiveDirect(void)`

So while initialization supports selecting `UART_9_BIT_CHAR`, a dedicated 9-bit send/receive API is still recommended for full 9-bit communication support.

---

# Example Usage

## Basic Blocking Example

```c
#include "UART_Int.h"
#include "UART_Cfg.h"

int main(void)
{
    u8 Local_u8Data;

    UART_Init();

    UART_Send('A');

    Local_u8Data = UART_Receive();

    while (1)
    {
    }
}
```

---

## RX Interrupt Example

```c
#include "UART_Int.h"

static volatile u8 Global_u8ReceivedData;

void UART_RX_Task(void)
{
    Global_u8ReceivedData = UART_ReceiveDirect();
}

int main(void)
{
    UART_Init();
    UART_RX_SetCallBack(UART_RX_Task);
    UART_RX_InterruptEnable();
    sei();

    while (1)
    {
    }
}
```

---

## UDRE Interrupt Example

```c
#include "UART_Int.h"

static const u8 Global_u8Msg[] = "HELLO";
static volatile u8 Global_u8Index = 0;

void UART_UDRE_Task(void)
{
    if (Global_u8Msg[Global_u8Index] != '\0')
    {
        UART_SendDirect(Global_u8Msg[Global_u8Index]);
        Global_u8Index++;
    }
    else
    {
        UART_UDRE_InterruptDisable();
        Global_u8Index = 0;
    }
}

int main(void)
{
    UART_Init();
    UART_UDRE_SetCallBack(UART_UDRE_Task);
    UART_UDRE_InterruptEnable();
    sei();

    while (1)
    {
    }
}
```

---

# Current Limitations / Future Improvements

- Add dedicated APIs for full **9-bit transmit/receive** support
- Add error-status handling for framing, overrun, and parity errors
- Add buffered interrupt-driven TX/RX in service layer
- Add string and buffer utilities in service layer instead of MCAL
- Add support for baud calculations for other `F_CPU` values
- Add safe double-speed mode handling with correct UBRR values

---

# Design Summary

This UART driver currently focuses on providing a clean **byte-oriented MCAL abstraction** for ATmega32.

It is suitable as a reusable base for:
- polling-based UART communication
- interrupt-driven UART communication
- future higher-level UART service modules

---

# Author

**Abdelrahman Elzayat**
