# 🎙️ TME Education 10th Anniversary Voice-Interactive Chatbot

An offline voice-interactive embedded system developed to celebrate the **10th Anniversary of TME Education**.

The project combines the **TME Education Board (TME-EDU-ARD-2)** with a **VC-02 Offline Voice Recognition Module** to create an interactive chatbot capable of recognizing predefined voice commands, providing spoken responses, and triggering corresponding visual animations.

The system operates **offline** and does not require an internet connection.

---

## 📌 Project Overview

The chatbot was developed as an interactive demonstration for the **TME Education 10th Anniversary celebration**.

A user can ask predefined questions such as:

- "Who are you?"
- "What is TME Education?"
- "What are we celebrating?"
- "How old is TME Education?"
- "When is the celebration?"
- "Where is the celebration?"
- "What time does it start?"
- "What will happen today?"
- "Say happy anniversary!"
- "Show me how to celebrate!"

The **VC-02** recognizes the spoken command and provides a predefined voice response. At the same time, it sends a hexadecimal command through UART to the TME-EDU-ARD-2 board.

The board then executes the corresponding visual animation.

---

## ✨ Main Features

- Offline voice recognition
- Offline spoken responses
- UART communication between VC-02 and TME-EDU-ARD-2
- Animated chatbot expressions
- Blinking eyes
- Looking left and right
- Happy and excited facial expressions
- Anniversary animations
- Dancing celebration animation
- Confetti effects
- Automatic idle animations
- 16×2 LCD anniversary message
- Colorful NeoPixel celebration lighting
- No Wi-Fi or internet connection required
- Memory-optimized for the Arduino Uno-based platform

---

## 🧩 Hardware

### Main Platform

**TME Education Board – TME-EDU-ARD-2**

The project makes use of features available on the TME Education Board, including:

- Arduino Uno-based controller
- OLED display
- 16×2 LCD
- Addressable NeoPixel LEDs

### External Module

**VC-02 Offline Voice Recognition Module**

The VC-02 is used for:

- Offline speech recognition
- Predefined voice responses
- UART command transmission

The VC-02 is the external voice-interface module used in the project.

---

## 🏗️ System Architecture

```text
             USER
              │
              │ Voice command
              ▼
        ┌─────────────┐
        │    VC-02    │
        │   Offline   │
        │    Voice    │
        │ Recognition │
        └──────┬──────┘
               │
        Spoken │ Response
               │
               └────────────► User

               │
               │ UART command
               │ 01 ... 0D
               ▼
      ┌────────────────────┐
      │ TME-EDU-ARD-2      │
      │ Arduino Controller │
      └─────────┬──────────┘
                │
        ┌───────┼─────────┐
        ▼       ▼         ▼
      OLED    16×2 LCD  NeoPixels
        │       │         │
    Chatbot   Anniversary Colorful
   Animations   Message   Lighting
```

---

# 🗣️ VC-02 Configuration

The VC-02 is configured using the **Ai-Thinker Voice Open Platform**.

The configuration consists of three main stages:

1. UART pin configuration
2. Customized offline command configuration
3. Control-details configuration

---

## 1. UART Pin Configuration

The VC-02 UART interface is configured as follows:

| Parameter | Configuration |
|---|---|
| UART RX | UART1_RX – Pin 7 |
| UART TX | UART1_TX – Pin 8 |
| Baud Rate | 9600 |
| Data Bits | 8 |
| Stop Bits | 1 |
| Parity | None |

For this project, **UART1_TX** is particularly important because the VC-02 sends a command to the TME Education Board whenever a voice command is successfully recognized.

### UART Connection

```text
VC-02 UART1_TX (Pin 8)
          │
          ▼
TME-EDU-ARD-2 / Arduino RX

VC-02 GND
          │
          ▼
TME-EDU-ARD-2 GND
```

A common ground must be provided between both systems.

---

## 2. Customized Offline Commands

Offline commands are created under:

**Customize offline command words and response words**

Each interaction contains three important elements:

```text
Behavior
Command Word
Reply Word
```

### Behavior

The behavior acts as the internal identifier for a particular interaction.

Examples include:

```text
Hello
Who_are_you
what_is_tme
celebrating
how_old
when_is_it
where_is_it
start_time
end_time
happen
Say_happy
celebrate
goodbye
```

### Command Word

The command word defines what the user can say to activate a behavior.

Multiple alternative phrases can be configured for the same behavior.

For example:

```text
Who are you?
What are you?
Your name?
```

All these phrases can activate the same `Who_are_you` behavior.

### Reply Word

The reply word specifies what the VC-02 says after recognizing the corresponding command.

Example:

```text
User:
"Who are you?"

VC-02:
"I am the TME Education anniversary assistant."
```

Voice recognition and the spoken responses are handled locally by the VC-02.

---

## 3. Control Details Configuration

After creating an offline command, a corresponding UART control action is configured under **Control Details**.

The configuration is:

```text
Control Type : UART1_TX
Action       : Send
Parameter    : Hexadecimal command
```

For example:

```text
Behavior     : Hello
Control Type : UART1_TX
Action       : Send
Parameter    : 01
```

When the user says **"Hello"**, the VC-02 recognizes the command, speaks its configured response, and sends the hexadecimal byte `01` through UART.

The Arduino receives the byte as:

```cpp
0x01
```

and executes the corresponding animation.

---

## 🔢 UART Command Mapping

| Hex Command | Behavior | Function |
|---|---|---|
| `0x01` | Hello | Greeting animation |
| `0x02` | Who_are_you | Friendly chatbot animation |
| `0x03` | what_is_tme | TME Education information |
| `0x04` | celebrating | Anniversary animation |
| `0x05` | how_old | 10-year celebration |
| `0x06` | when_is_it | Event date |
| `0x07` | where_is_it | Event venue |
| `0x08` | start_time | Starting time |
| `0x09` | end_time | Ending time |
| `0x0A` | happen | Event/activity animation |
| `0x0B` | Say_happy | Happy anniversary animation |
| `0x0C` | celebrate | Full celebration animation |
| `0x0D` | goodbye | Goodbye animation |

---

## 💻 Arduino Command Processing

The Arduino receives the command from the VC-02 and uses a `switch` statement to determine which animation should run.

Example:

```cpp
switch (command) {

  case 0x01:
    happyFace();
    break;

  case 0x02:
    friendlyFace();
    break;

  case 0x04:
    anniversaryAnimation();
    break;

  case 0x0C:
    celebrationAnimation();
    break;

  case 0x0D:
    goodbyeAnimation();
    break;
}
```

This creates a direct relationship between the recognized voice command and the visual behavior of the chatbot.

---

# 🤖 OLED Chatbot Animations

The OLED provides the visual personality of the chatbot.

The implemented animations include:

- Normal eyes
- Blinking
- Double blinking
- Looking left
- Looking right
- Happy expression
- Excited expression
- Anniversary animation
- Confetti effects
- Dancing characters
- Full celebration sequence
- Goodbye animation

When no voice command is being received, the chatbot performs random idle animations to make it appear active and responsive.

---

## 🖥️ LCD Display

The integrated 16×2 LCD displays a permanent anniversary message:

```text
   HAPPY 10TH
 TME EDUCATION
```

A fixed message was selected instead of scrolling text to provide a clean and easily readable display during the demonstration.

---

## 🌈 NeoPixel Lighting

The addressable NeoPixel LEDs on the TME Education Board provide colorful celebration lighting.

The LEDs remain illuminated and automatically cycle through different colors while the chatbot is operating.

This adds a visual celebration effect to complement the OLED animations and LCD message.

---

# 💾 Memory Optimization

The project runs on an Arduino Uno-based platform with limited SRAM.

To reduce memory consumption, the SH1106 OLED uses the **U8g2 page-buffer mode**:

```cpp
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(
    U8G2_R0,
    U8X8_PIN_NONE
);
```

The `_1_` page-buffer configuration consumes significantly less SRAM than using a full display framebuffer.

This optimization allows the chatbot animations, UART communication, LCD, and lighting functions to operate together on the embedded platform.

---

# 📚 Required Arduino Libraries

Install the required libraries through the Arduino IDE Library Manager before compiling the project.

```text
U8g2
hd44780
Adafruit NeoPixel
SoftwareSerial
Wire
```

`Wire` and `SoftwareSerial` are included with the Arduino environment.

---

# 🚀 Operating Sequence

```text
1. Power ON the system
            ↓
2. Initialize TME-EDU-ARD-2
            ↓
3. Initialize OLED
            ↓
4. Initialize LCD
            ↓
5. Initialize NeoPixels
            ↓
6. Initialize VC-02 UART communication
            ↓
7. Display chatbot idle animation
            ↓
8. Wait for a voice command
            ↓
9. VC-02 recognizes the command
            ↓
10. VC-02 speaks the configured response
            ↓
11. VC-02 sends hexadecimal UART command
            ↓
12. TME-EDU-ARD-2 receives command
            ↓
13. Corresponding OLED animation executes
            ↓
14. Return to idle chatbot mode
```

---

# 🎯 Educational Objectives

This project demonstrates practical concepts in:

- Embedded systems development
- Offline voice recognition
- Human-machine interaction
- UART serial communication
- I²C communication
- OLED graphics and animation
- LCD interfacing
- Addressable LED control
- Arduino programming
- Memory optimization
- Event-driven embedded system design

It demonstrates how multiple embedded technologies can be integrated into a single interactive application.

---

# 🎉 TME Education 10th Anniversary

The project was developed to celebrate **10 years of TME Education** and demonstrate the possibilities of hands-on electronics and embedded systems education.

It provides an interactive way for visitors to learn about TME Education while experiencing voice recognition, embedded programming, electronics, and visual animation in action.

**10 Years of Learning, Technology, Innovation and Shared Passion.**

**Together we'll go further!**

---

## 📂 Repository Structure

```text
Voice-Interactive-Chatbot/
│
├── final_code/
│   └── TME Education Anniversary Chatbot source code
│
└── README.md
```

---

## 👨‍💻 Author

**Mpuuga Abdu Nasser**  
TME Education Tutor – Uganda

Embedded Systems | Electronics | IoT | Robotics | Embedded AI

---

## 📜 Project Status

**Completed – TME Education 10th Anniversary Demonstration Project**

---

## ⭐ Support

If you find this project useful or educational, consider giving the repository a **star ⭐**.

Feel free to explore the source code, learn from the implementation, and use the project as inspiration for your own embedded systems and offline voice-interaction projects.
