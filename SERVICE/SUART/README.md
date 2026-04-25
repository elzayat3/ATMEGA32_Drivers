# UART Service Layer (SUART)

A lightweight **UART service layer** built on top of the **ATmega32 UART MCAL driver**.

This module simplifies application-level UART usage by providing:

- Asynchronous **string transmission**
- **Queued TX requests** using FIFO order
- Asynchronous reception into a **user-provided buffer**
- **TX/RX completion callbacks**

The application works with the service layer APIs, while direct hardware access remains inside the UART MCAL driver.

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

---

# Repository Location

```text
ATMEGA32_Drivers
│
├── SERVICE
│   └── SUART
│       ├── SUART_Int.h
│       ├── SUART_Private.h
│       └── SUART_Prg.c
│
└── CFG
    └── SUART
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
- Send **null-terminated strings** only
- Multiple send requests can be queued
- FIFO transmission order
- TX completion callback when one queued string finishes

## RX Features

- Asynchronous reception into a **user buffer**
- One RX request at a time
- The last byte of the buffer is reserved for `\0`
- Received characters count = **Buffer Size - 1**
- RX completion callback when reception is completed

---

# Configuration

Configuration is done in `SUART_Cfg.h`.

## TX Queue Size

```c
#define SUART_TX_QUEUE_SIZE    5U
```

This macro defines the maximum number of pending TX string requests that can be stored in the queue.

> The current implementation does **not** use an RX queue.

---

# Public APIs

## Initialization

```c
error_t SUART_Init(void);
```

Initializes the SUART service, connects internal handlers to UART MCAL callbacks, and disables service-controlled interrupts until needed.

---

## Asynchronous TX

```c
error_t SUART_SendAsync(const c8 *Data);
```

Queues one null-terminated string for asynchronous transmission.

### Behavior

- If the TX service is idle, transmission starts automatically.
- If the TX service is busy, the string is stored in the TX queue.
- Strings are transmitted in FIFO order.

### Returns

- `OK` if the string is accepted successfully
- `NULL_PTR` if `Data` is `NULLPTR`
- `NOK` if the string is empty
- `FULL` if the TX queue is full

### Important Note

The driver stores only the **string pointer** inside the queue.
It does **not** copy the string contents.

So the application must make sure the string remains valid until transmission is completed.

#### Valid usage

```c
static const c8 Msg1[] = "HELLO";
SUART_SendAsync(Msg1);
```

#### Risky usage

```c
void App(void)
{
    c8 LocalMsg[] = "HELLO";
    SUART_SendAsync(LocalMsg);
}
```

In the risky case, `LocalMsg` may become invalid before transmission finishes.

---

## Asynchronous RX

```c
error_t SUART_ReceiveAsync(c8 *str, u8 size);
```

Starts asynchronous reception into a user-provided buffer.

### RX Buffer Design

In this API, `size` represents the **total buffer size**, not the number of received characters.

The service reserves the last byte of the buffer for the null terminator.

So:

```text
Received Characters = size - 1
```

### Example

If:

```c
c8 RxBuffer[10];
SUART_ReceiveAsync(RxBuffer, 10U);
```

Then:

- 9 characters will be received
- `RxBuffer[9]` is reserved for `\0`

### Minimum Valid Size

The minimum valid size is:

```text
2
```

Because:

- one byte is needed for received data
- one byte is needed for `\0`

### Returns

- `OK` if RX starts successfully
- `NULL_PTR` if `str` is `NULLPTR`
- `OUT_OF_RANGE` if `size <= 1`
- `IN_PROGRESS` if another RX request is already active

---

## TX Callback Registration

```c
error_t SUART_TX_SetCallBack(SUART_Callback_t Add_pfCallBack);
```

Registers a callback function that is called when one queued TX string finishes.

### Returns

- `OK` if callback registration succeeds
- `NULL_PTR` if `Add_pfCallBack` is `NULLPTR`

---

## RX Callback Registration

```c
error_t SUART_RX_SetCallBack(SUART_Callback_t Add_pfCallBack);
```

Registers a callback function that is called when the active RX request is completed.

### Returns

- `OK` if callback registration succeeds
- `NULL_PTR` if `Add_pfCallBack` is `NULLPTR`

---

# Internal Operation

## TX Flow

1. The application calls `SUART_SendAsync()`.
2. The string pointer is added to the TX queue.
3. `UART_UDRE_InterruptEnable()` starts or continues service execution.
4. The UART MCAL ISR triggers the registered callback.
5. `SUART_TxISR()`:
   - loads the next queued string when needed
   - sends characters one by one using `UART_SendDirect()`
   - moves to the next queued string automatically
6. When no more strings remain:
   - `UART_UDRE_InterruptDisable()` is called
   - TX callback is invoked for each completed string if registered

## RX Flow

1. The application calls `SUART_ReceiveAsync()` with a buffer and its total size.
2. The service stores the buffer pointer and prepares the receive state.
3. `UART_RX_InterruptEnable()` starts reception.
4. The UART MCAL ISR triggers the registered callback.
5. `SUART_RxISR()`:
   - reads one byte using `UART_ReceiveDirect()`
   - stores it into the destination buffer
   - continues until `(size - 1)` characters are received
6. When reception completes:
   - `UART_RX_InterruptDisable()` is called
   - RX state becomes idle
   - RX callback is invoked if registered

---

# Important Notes

## 1) TX queue stores pointers, not copies

The TX queue stores only pointers to strings.
It does not allocate or copy string data internally.

## 2) RX buffer must remain valid

The buffer passed to `SUART_ReceiveAsync()` must remain valid until reception is completed.

## 3) RX size means total buffer capacity

The `size` parameter is the **total buffer size**.
The actual number of received characters is always `size - 1`.

## 4) One RX request at a time

The current design supports only **one active RX request**.
If a receive operation is already in progress, `SUART_ReceiveAsync()` returns `IN_PROGRESS`.

## 5) Callback timing

- TX callback is called when one queued string finishes service transmission.
- RX callback is called when the configured receive buffer is filled.

---

# Example Usage

## Example 1: Initialize SUART

```c
int main(void)
{
    UART_Init();
    SUART_Init();

    while (1)
    {
    }
}
```

---

## Example 2: Send one string asynchronously

```c
static const c8 Msg1[] = "HELLO";

SUART_SendAsync(Msg1);
```

---

## Example 3: Queue multiple strings

```c
static const c8 Msg1[] = "TEMP=25";
static const c8 Msg2[] = "ADC=512";
static const c8 Msg3[] = "DONE";

SUART_SendAsync(Msg1);
SUART_SendAsync(Msg2);
SUART_SendAsync(Msg3);
```

The strings are transmitted in FIFO order.

---

## Example 4: Receive into a user buffer

```c
c8 RxBuffer[10];

SUART_ReceiveAsync(RxBuffer, 10U);
```

This receives 9 characters and keeps the last byte for `\0`.

---

## Example 5: Register callbacks

```c
void TxDoneNotification(void)
{
    /* One queued TX string finished */
}

void RxDoneNotification(void)
{
    /* RX request completed */
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

This SUART service layer provides:

- A clean abstraction above the UART MCAL driver
- Queue-based asynchronous string transmission
- Fixed-buffer asynchronous reception
- Interrupt-driven execution
- Simple callback-based completion notification

It is intended to keep the application layer simple while preserving a clean embedded software layering approach.

---

# Author

**Abdelrahman Elzayat**  
Embedded Systems Developer
