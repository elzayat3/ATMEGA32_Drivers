# UART Service Layer (SUART)

A higher-level **UART service layer** built on top of the ATmega32 **UART MCAL driver**.

This module extends the low-level UART driver by providing:

- Asynchronous string transmission
- Asynchronous raw transmission
- Queued TX/RX request handling
- Terminator-based string reception
- Fixed-length raw buffer reception
- Callback notifications for TX and RX service completion

The service layer is designed to simplify application-level UART communication while keeping direct hardware access inside the MCAL layer.

---

# Layer Position

```text
Application Layer
        │
        ▼
 UART Service Layer (SUART)
        │
        ▼
     UART MCAL Driver
        │
        ▼
   Hardware Registers
```

The application interacts with **SUART** for string/buffer communication, while the service layer internally uses the **UART MCAL APIs**.

---

# Repository Location

```text
ATMEGA32_Drivers
│
├── SERVICE
│   └── UART
│       ├── SUART_Int.h
│       ├── SUART_Private.h
│       └── SUART_Prg.c
│
└── CFG
    └── UART
        ├── SUART_Cfg.h
        └── SUART_Cfg.c
```

---

# Dependencies

This module depends on the UART MCAL driver.

Required UART MCAL features:

- `UART_UDRE_SetCallBack()`
- `UART_RX_SetCallBack()`
- `UART_UDRE_InterruptEnable()` / `UART_UDRE_InterruptDisable()`
- `UART_RX_InterruptEnable()` / `UART_RX_InterruptDisable()`
- `UART_SendDirect()`
- `UART_ReceiveDirect()`

---

# Features

## TX Features

- Queue-based asynchronous transmission
- Send one string asynchronously
- Send multiple strings asynchronously
- Send raw string without appending configured terminator
- Send multiple raw strings asynchronously
- Automatic TX terminator appending in normal mode
- TX completion callback when queue becomes empty

## RX Features

- Queue-based asynchronous reception
- Receive one string asynchronously using configurable RX terminator
- Receive multiple strings asynchronously
- Receive one raw buffer with fixed length
- Receive multiple raw buffers asynchronously
- RX completion callback when one queued RX request is completed

---

# Configuration

Configuration is split into `SUART_Cfg.h` and `SUART_Cfg.c`.

## Queue Size Configuration

Defined in `SUART_Cfg.h`:

```c
#define SUART_TX_QUEUE_SIZE     5U
#define SUART_RX_QUEUE_SIZE     5U
```

These macros define how many TX and RX requests can be queued at the same time.

## Terminator Configuration

Defined in `SUART_Cfg.h`:

```c
#define SUART_TX_TERMINATOR_LEN 1U
#define SUART_RX_TERMINATOR_LEN 1U
```

Configured in `SUART_Cfg.c`:

```c
const u8 SUART_TxTerminator[SUART_TX_TERMINATOR_LEN] = {'\n'};
const u8 SUART_RxTerminator[SUART_RX_TERMINATOR_LEN] = {'\n'};
```

### Example: Single-character terminator

```c
#define SUART_TX_TERMINATOR_LEN 1U
#define SUART_RX_TERMINATOR_LEN 1U

const u8 SUART_TxTerminator[SUART_TX_TERMINATOR_LEN] = {'\n'};
const u8 SUART_RxTerminator[SUART_RX_TERMINATOR_LEN] = {'\n'};
```

### Example: Two-character terminator

```c
#define SUART_TX_TERMINATOR_LEN 2U
#define SUART_RX_TERMINATOR_LEN 2U

const u8 SUART_TxTerminator[SUART_TX_TERMINATOR_LEN] = {'\r', '\n'};
const u8 SUART_RxTerminator[SUART_RX_TERMINATOR_LEN] = {'\r', '\n'};
```

This flexible design supports one-character or multi-character terminators without changing the service logic.

---

# Public APIs

## Initialization

```c
error_t SUART_Init(void);
```

Initializes the UART service layer, resets internal runtime state, clears all queues, links internal service handlers to UART MCAL callbacks, and disables service-controlled interrupts until needed.

---

## TX APIs

### Send one string asynchronously

```c
error_t SUART_SendStringAsync(const u8 *Add_pu8Str);
```

Adds one null-terminated string to the TX queue.
The configured TX terminator is automatically appended.

### Send multiple strings asynchronously

```c
error_t SUART_SendStringsAsync(const u8 * const Add_pu8StrArr[], u8 Copy_u8Count);
```

Adds multiple strings to the TX queue.
The configured TX terminator is automatically appended after each string.

### Send one raw string asynchronously

```c
error_t SUART_SendRawStringAsync(const u8 *Add_pu8Str);
```

Adds one raw null-terminated string to the TX queue **without** appending the configured TX terminator.

### Send multiple raw strings asynchronously

```c
error_t SUART_SendRawStringsAsync(const u8 * const Add_pu8StrArr[], u8 Copy_u8Count);
```

Adds multiple raw strings to the TX queue **without** appending the configured TX terminator.

---

## RX APIs

### Receive one string asynchronously

```c
error_t SUART_ReceiveStringAsync(u8 *Add_pu8Str, u16 Copy_u16BufferSize);
```

Registers one destination buffer in the RX queue.
Reception continues until the configured RX terminator is detected.
The service then appends `\0` to complete the string.

### Receive multiple strings asynchronously

```c
error_t SUART_ReceiveStringsAsync(u8 *Add_pu8StrArr[], const u16 Add_pu16BufferSizeArr[], u8 Copy_u8Count);
```

Registers multiple destination buffers in the RX queue.
Each buffer receives one string terminated by the configured RX terminator.

### Receive one raw buffer asynchronously

```c
error_t SUART_ReceiveRawBufferAsync(u8 *Add_pu8Buffer, u16 Copy_u16Length);
```

Registers one destination buffer in the RX queue.
Reception continues until the specified number of bytes is received.
No null terminator is appended.

### Receive multiple raw buffers asynchronously

```c
error_t SUART_ReceiveRawBuffersAsync(u8 *Add_pu8BufferArr[], const u16 Add_pu16LengthArr[], u8 Copy_u8Count);
```

Registers multiple raw buffer requests in the RX queue.
Each buffer receives the exact number of bytes specified in its corresponding length entry.

---

## Service Callbacks

### TX callback

```c
error_t SUART_TX_SetCallBack(SUART_Callback_t Add_pfCallBack);
```

Registers a callback function that is invoked when the TX service queue becomes empty.

### RX callback

```c
error_t SUART_RX_SetCallBack(SUART_Callback_t Add_pfCallBack);
```

Registers a callback function that is invoked whenever one RX request is completed.

---

# Internal Operation

## TX Flow

1. Application queues one or more TX requests using SUART APIs.
2. If TX service is idle, `UART_UDRE_InterruptEnable()` is called.
3. UART MCAL triggers `UART_UDRE_vect`.
4. UART MCAL callback calls `SUART_TxHandler()`.
5. `SUART_TxHandler()`:
   - Loads the next queued TX request if needed
   - Sends characters one by one using `UART_SendDirect()`
   - Appends configured TX terminator in normal mode
   - Skips terminator in raw mode
6. When the TX queue becomes empty:
   - `UART_UDRE_InterruptDisable()` is called
   - TX service state becomes `SUART_IDLE`
   - User TX callback is invoked if registered

## RX Flow

1. Application queues one or more RX requests using SUART APIs.
2. If RX service is idle, `UART_RX_InterruptEnable()` is called.
3. UART MCAL triggers `UART_RX_vect`.
4. UART MCAL callback calls `SUART_RxHandler()`.
5. `SUART_RxHandler()`:
   - Loads the next queued RX request if needed
   - Reads one byte using `UART_ReceiveDirect()`
   - Stores data in current destination buffer
   - Detects RX terminator in string mode
   - Tracks exact byte count in raw mode
6. When one RX request completes:
   - User RX callback is invoked if registered
7. When no more RX requests remain:
   - `UART_RX_InterruptDisable()` is called
   - RX service state becomes `SUART_IDLE`

---

# Important Notes

## 1) TX queue stores pointers, not copies

The service layer stores only **string pointers** inside the TX queue.
It does **not** copy the string contents.

### Valid usage

```c
static const u8 Msg1[] = "HELLO";
SUART_SendStringAsync(Msg1);
```

### Risky usage

```c
void App(void)
{
    u8 LocalMsg[] = "HELLO";
    SUART_SendStringAsync(LocalMsg);
}
```

In the risky case, `LocalMsg` may become invalid before transmission is completed.

## 2) RX buffers must remain valid

Any RX buffer passed to SUART must remain valid until the corresponding receive request is completed.

## 3) String mode vs raw mode

- **String mode** uses configured RX terminator detection and appends `\0`
- **Raw mode** ignores terminators and receives an exact byte count

## 4) Buffer size in string receive

`Copy_u16BufferSize` represents the total destination buffer size.
It should be large enough to hold:

- received string characters
- configured RX terminator (during internal detection)
- final null terminator `\0`

## 5) Fixed length in raw receive

`Copy_u16Length` represents the exact number of bytes to receive.
No null terminator is added automatically.

---

# Example Usage

## Example 1: Send one string with automatic terminator

```c
#include "UART_Int.h"
#include "SUART_Int.h"

int main(void)
{
    UART_Init();
    SUART_Init();

    SUART_SendStringAsync((const u8 *)"HELLO");

    while (1)
    {
    }
}
```

If TX terminator is configured as `{'\n'}`, the transmitted data becomes:

```text
HELLO\n
```

## Example 2: Send one raw string without terminator

```c
SUART_SendRawStringAsync((const u8 *)"HELLO");
```

Transmitted data becomes exactly:

```text
HELLO
```

## Example 3: Queue multiple strings

```c
static const u8 *Msgs[] =
{
    (const u8 *)"TEMP=25",
    (const u8 *)"ADC=512",
    (const u8 *)"DONE"
};

SUART_SendStringsAsync(Msgs, 3U);
```

## Example 4: Receive one string until terminator

```c
u8 RxBuffer[32];

SUART_ReceiveStringAsync(RxBuffer, 32U);
```

If RX terminator is configured as `{'\n'}` and UART receives:

```text
HELLO\n
```

Then `RxBuffer` becomes:

```text
HELLO
```

## Example 5: Receive fixed raw buffer

```c
u8 RawBuffer[8];

SUART_ReceiveRawBufferAsync(RawBuffer, 8U);
```

Exactly 8 bytes are received, regardless of terminator characters.

## Example 6: Register callbacks

```c
void TxDoneNotification(void)
{
    /* TX queue became empty */
}

void RxDoneNotification(void)
{
    /* One RX request completed */
}

int main(void)
{
    UART_Init();
    SUART_Init();

    SUART_TX_SetCallBack(TxDoneNotification);
    SUART_RX_SetCallBack(RxDoneNotification);

    while (1)
    {
    }
}
```

---

# Design Summary

This UART service layer provides:

- Clean abstraction above the UART MCAL driver
- Reusable queue-based asynchronous communication
- Separate normal/raw handling for TX and RX
- Configurable TX and RX terminators
- Support for single and multiple queued requests
- Interrupt-driven service execution

It is intended to simplify application-level UART communication while preserving the layered embedded software architecture used across the project.

---

# Author

**Abdelrahman Elzayat**  
Embedded Systems Developer
