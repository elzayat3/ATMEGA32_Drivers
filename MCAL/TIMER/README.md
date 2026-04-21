# Timer Driver (ATmega32)

Timer driver for the **ATmega32 AVR microcontroller** implemented as part of the **MCAL (Microcontroller Abstraction Layer)**.

The driver provides a unified interface for **Timer0**, **Timer1**, and **Timer2**, with support for initialization, start/stop control, register access, interrupts, callbacks, and **Timer1 Input Capture Unit (ICU)**.

---

# Driver Location

    MCAL/TIMER

Configuration files are located in:

    CFG/TIMER

---

# Features

- Support for **Timer0, Timer1, and Timer2**
- Support for **Normal**, **CTC**, and **PWM** modes
- Support for **Fast PWM** and **Phase Correct PWM**
- Start and stop control for each timer
- Set/Get APIs for **counter** and **compare** registers
- Interrupt enable/disable APIs
- Callback registration for timer interrupts
- **Timer1 ICU** support:
  - Edge selection
  - Capture value reading
  - ICU flag clearing
  - Noise canceler enable/disable

---

# Supported Timers

## Timer0
- Normal mode
- CTC mode
- Fast PWM
- Phase Correct PWM

## Timer1
- Normal mode
- CTC with **OCR1A** as TOP
- CTC with **ICR1** as TOP
- Fast PWM with **ICR1** as TOP
- Fast PWM with **OCR1A** as TOP
- Phase Correct PWM with **ICR1** as TOP

## Timer2
- Normal mode
- CTC mode
- Fast PWM
- Phase Correct PWM

---

# Main APIs

- `TIMER_Init()`
- `TIMER0_Init()`
- `TIMER1_Init()`
- `TIMER2_Init()`
- `TIMER_Start()`
- `TIMER_Stop()`
- `TIMER_InterruptEnable()`
- `TIMER_InterruptDisable()`
- `TIMER_SetCallback()`

## Register Access APIs

- `TIMER0_SetCounter()` / `TIMER0_GetCounter()`
- `TIMER0_SetCompare()` / `TIMER0_GetCompare()`
- `TIMER1_SetCounter()` / `TIMER1_GetCounter()`
- `TIMER1_SetCompareA()` / `TIMER1_GetCompareA()`
- `TIMER1_SetCompareB()` / `TIMER1_GetCompareB()`
- `TIMER1_SetTop()` / `TIMER1_GetTop()`
- `TIMER2_SetCounter()` / `TIMER2_GetCounter()`
- `TIMER2_SetCompare()` / `TIMER2_GetCompare()`

---

# Timer1 ICU APIs

- `TIMER1_ICU_SetEdge()`
- `TIMER1_ICU_GetCaptureValue()`
- `TIMER1_ICU_ClearFlag()`
- `TIMER1_ICU_EnableNoiseCanceler()`
- `TIMER1_ICU_DisableNoiseCanceler()`

---

# Configuration

The driver uses a **pre-compile configuration** approach.  
All timer settings are defined statically in `TIMER_Cfg.c` through:

- `TIMER0_CFG`
- `TIMER1_CFG`
- `TIMER2_CFG`

---

# Notes

- Timers are configured during initialization but remain stopped until `TIMER_Start()` is called.
- For PWM output, the related **OC pin** must be configured as output in the GPIO/DIO driver.
- Callback registration does not enable interrupts by itself; interrupts must be enabled separately, and global interrupts must also be enabled by the application.

---

# Author

**Abdelrahman Elzayat**