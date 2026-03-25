# 🔌 External Interrupt Driver (EXIU) – ATmega32

A configurable and modular External Interrupt (EXTI) driver for **ATmega32**, supporting INT0, INT1, and INT2.

---

## 📌 Features

* Support for **INT0, INT1, INT2**
* Configurable trigger modes:

  * Low Level
  * Any Logical Change
  * Falling Edge
  * Rising Edge
* Enable / Disable control per interrupt
* Callback mechanism for ISR handling
* Clean and scalable design (no magic numbers)
* Configuration separated from driver logic

---

## 🧱 Driver Structure

```text
MCAL/
└── EXIU/
    ├── EXIU_Int.h        → Public interface (APIs + types)
    ├── EXIU_Program.c    → Driver implementation
    └── EXIU_Private.h    → Internal definitions

CFG/
└── EXIU/
    ├── EXIU_Cfg.h        → Configuration header
    └── EXIU_Cfg.c        → User configuration
```

```

---

## ⚙️ Configuration

All interrupt settings are defined in:

📄 `EXIU_Cfg.c`

```c
EXTI_Config_t EXTI_ConfigArr[EXTI_CONFIG_SIZE] =
{
    {EX_INT0, FALLING_EDGE, EXTI_ENABLE},
    {EX_INT1, RISING_EDGE,  EXTI_ENABLE},
    {EX_INT2, FALLING_EDGE, EXTI_DISABLE}
};
```

---

## 🚀 Usage

### 1. Configure DIO Pins

```c
DIO_SetPinDirection(PORTD, PIN2, INPUT); // INT0
```

---

### 2. Initialize Driver

```c
EXIU_Init();
```

---

### 3. Set Callback

```c
void button_handler(void)
{
    // Your code here
}

EXIU_SetCallback(EX_INT0, button_handler);
```

---

### 4. Enable Global Interrupts

```c
GLOBAL_ENABLE();
```

---

## ⚠️ Notes

* INT2 supports only:

  * `FALLING_EDGE`
  * `RISING_EDGE`
* Callbacks are executed inside ISR context:

  * Keep them short
  * Avoid blocking code
* DIO must be configured before EXIU initialization

---

## 💡 Example

```c
void led_toggle(void)
{
    DIO_TogglePin(PORTC, PIN0);
}

int main(void)
{
    DIO_SetPinDirection(PORTC, PIN0, OUTPUT);
    DIO_SetPinDirection(PORTD, PIN2, INPUT);

    EXIU_SetCallback(EX_INT0, led_toggle);
    EXIU_Init();

    GLOBAL_ENABLE();

    while(1)
    {
    }
}
```

---

## 🧠 Design Highlights

* No magic numbers (uses enums and macros)
* Scalable callback system (array-based)
* Configurable via external file (no hardcoding)
* Clean separation between interface, config, and implementation

---

## 👨‍💻 Author

**Abdelrahman Elzayat**

---

## 📜 License

This project is open-source and available for educational and development purposes.
