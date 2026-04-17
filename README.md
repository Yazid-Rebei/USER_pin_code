# ESP32 PIN Lock System with LCD I2C

## Overview

This project is a simple authentication system built with **ESP32** and an **I2C LCD display**.  
It allows the user to:

- add users with a **4-digit PIN**
- log in using an existing PIN
- display the list of registered users
- put the system into **sleep mode**
- block access after repeated failed login attempts

The project is designed as a basic prototype for a **smart lock / access control system**.

---

## Features

- User registration with unique 4-digit PIN
- PIN format validation
- Login verification
- LCD display feedback
- Serial monitor interaction
- Sleep mode
- Temporary lock after failed attempts:
  - first block: **30 seconds**
  - second block: **60 seconds**
  - then permanent block until restart

---

## Hardware Requirements

- ESP32
- LCD I2C 16x2
- Jumper wires
- USB cable
- Breadboard (optional)

---

## Pin Configuration

### I2C LCD connection
- **SDA** → GPIO **21**
- **SCL** → GPIO **22**

LCD address used in code:

```cpp
0x27
