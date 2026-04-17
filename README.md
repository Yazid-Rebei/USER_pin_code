# ESP32 Smart PIN Authentication System

A lightweight embedded authentication system built with **ESP32** and **I2C LCD**, designed as a prototype for a smart lock, secure access panel, or embedded login gateway.

The system supports **multi-user registration**, **PIN-based authentication**, **LCD status feedback**, **sleep mode**, and a **progressive anti-bruteforce protection mechanism**.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Objectives](#objectives)
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
- [Example LCD Messages](#example-lcd-messages)
- [Current Limitations](#current-limitations)
- [Future Improvements](#future-improvements)
- [Use Cases](#use-cases)
- [Author](#author)
- [License](#license)

---

## Project Overview

This project implements a simple but structured **PIN authentication system** on an **ESP32** microcontroller.

It is designed as a first prototype for embedded applications such as:

- smart door access systems
- secure embedded control panels
- login gateways for smart home modules
- educational embedded security projects

The user interacts with the system through:

- the **Serial Monitor** for menu navigation and PIN input
- a **16x2 I2C LCD** for status feedback and user messages

The project focuses on combining **embedded programming**, **basic security logic**, and **human-machine interaction** in a compact and understandable architecture.

---

## Objectives

The main goals of this project are:

- design a simple embedded authentication mechanism
- manage multiple users with individual PIN codes
- display system state clearly using an LCD
- introduce basic protection against repeated login failures
- create a clean foundation for future smart lock or home access projects

---

## Features

- Registration of up to **5 users**
- Unique **4-digit PIN** for each user
- Automatic username generation
- PIN format validation
- Authentication through serial input
- LCD feedback messages
- User list display from serial monitor
- Sleep mode with LCD backlight disabled
- Progressive anti-bruteforce lockout:
  - after first 3 failed attempts: **30 seconds lock**
  - after second 3 failed attempts: **60 seconds lock**
  - after repeated failure: **permanent block until restart**
- Simple menu-driven interface
- Easy to extend for keypad, EEPROM, or web-based authentication

---

## System Architecture

The project follows a modular embedded logic flow:

```text
User Input (Serial Monitor)
        |
        v
Menu Selection
        |
        v
PIN Validation
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
LCD Feedback + Serial Output
