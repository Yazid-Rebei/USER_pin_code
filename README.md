# ESP32 Smart PIN Authentication System

A lightweight embedded authentication system built with **ESP32** and **I2C LCD**, designed as a prototype for a smart lock or access control module.  
The system supports user registration, PIN-based login, display feedback, sleep mode, and progressive lockout after repeated failed authentication attempts.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Pin Configuration](#pin-configuration)
- [Project Workflow](#project-workflow)
- [Code Structure](#code-structure)
- [Data Model](#data-model)
- [Security Logic](#security-logic)
- [How to Build and Run](#how-to-build-and-run)
- [Example Serial Menu](#example-serial-menu)
- [Current Limitations](#current-limitations)
- [Future Improvements](#future-improvements)
- [Use Cases](#use-cases)
- [Author](#author)
- [License](#license)

---

## Project Overview

This project implements a simple **PIN-based authentication system** on ESP32.  
It is intended as a first prototype for embedded applications such as:

- smart door access systems
- secure embedded control panels
- login gateways for smart home modules
- educational embedded security projects

The user interacts with the system through:

- the **Serial Monitor** for input
- a **16x2 I2C LCD** for feedback and status display

---

## Features

- Registration of up to **5 users**
- Unique **4-digit PIN** per user
- Automatic username generation
- PIN format validation
- Authentication via serial input
- LCD status messages
- User list display
- Sleep mode with LCD backlight off
- Progressive anti-bruteforce lockout:
  - after first 3 failed attempts: **30 seconds**
  - after second 3 failed attempts: **60 seconds**
  - after repeated failure: **permanent block until restart**

---

## System Architecture

The project follows a simple procedural embedded design:

```text id="e2n6hu"
User Input (Serial Monitor)
        |
        v
PIN Validation / Menu Selection
        |
        v
Authentication Logic
        |
        +--> User Registration
        +--> Login Verification
        +--> User Listing
        +--> Sleep Mode
        |
        v
LCD Feedback + Serial Debug Output
```
###Mohamed yazid rebei
