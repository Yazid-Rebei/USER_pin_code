#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <stdio.h>
#include "config.h"

user user_table[NBR_MAX_USER];
int user_count = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcd_show_message(const char* line1, const char* line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}

bool PIN_format_valid(const char* PIN) {
    if (strlen(PIN) != 4) {
        return false;
    }

    for (int i = 0; i < 4; i++) {
        if (PIN[i] < '0' || PIN[i] > '9') {
            return false;
        }
    }

    return true;
}

void clear_serial_buffer() {
    while (Serial.available() > 0) {
        Serial.read();
    }
}

int authent_chercher_user(const char* pin) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(pin, user_table[i].code_PIN) == 0) {
            return i;
        }
    }
    return -1;
}

void serial_read_pin(char* buffer) {
    while (true) {
        Serial.println("Entrer un PIN de 4 chiffres :");
        lcd_show_message("Entrer PIN:", "");

        while (Serial.available() == 0) {
        }

        String input = Serial.readStringUntil('\n');
        input.trim();

        if (PIN_format_valid(input.c_str())) {
            input.toCharArray(buffer, 5);

            Serial.print("PIN saisi: ");
            Serial.println(buffer);

            lcd_show_message("PIN recu", "OK");
            delay(1000);
            clear_serial_buffer();
            return;
        }

        Serial.println("PIN invalide. Reessayer.");
        lcd_show_message("PIN invalide", "Reessayer");
        delay(1200);
        clear_serial_buffer();
    }
}

void countdown_wait(int seconds) {
    for (int i = seconds; i > 0; i--) {
        char line2[17];
        sprintf(line2, "Wait %ds", i);

        lcd_show_message("LOGIN DENIED", line2);
        delay(1000);
    }
}

void show_users() {
    if (user_count == 0) {
        Serial.println("Aucun utilisateur.");
        lcd_show_message("NO USER", "");
        delay(1200);
        return;
    }

    Serial.println("=== Existing users ===");

    for (int i = 0; i < user_count; i++) {
        Serial.print("User: ");
        Serial.println(user_table[i].username);

        if (i + 1 < user_count) {
            lcd_show_message(user_table[i].username, user_table[i + 1].username);
            i++;
        } else {
            lcd_show_message(user_table[i].username, "");
        }

        delay(1500);
    }
}

void authent_add_user() {
    if (user_count >= NBR_MAX_USER) {
        Serial.println("Nombre max d'utilisateurs atteint.");
        lcd_show_message("Max users", "atteint");
        delay(1500);
        return;
    }

    if (user_count > 0) {
        show_users();
        delay(800);
    }

    char PIN[5];

    while (true) {
        serial_read_pin(PIN);

        if (authent_chercher_user(PIN) >= 0) {
            Serial.println("PIN deja existant !");
            lcd_show_message("PIN EXIST!", "REESSAYER");
            delay(1500);
        } else {
            break;
        }
    }

    user_table[user_count].id = user_count;
    sprintf(user_table[user_count].username, "user_%d", user_count);
    strcpy(user_table[user_count].code_PIN, PIN);

    Serial.print("Utilisateur ajoute : ");
    Serial.println(user_table[user_count].username);

    lcd_show_message("User ajoute", user_table[user_count].username);
    delay(1500);

    user_count++;
}

void first_user() {
    Serial.println("Creation du premier user");
    lcd_show_message("FIRST USER", "CREATE");
    delay(1000);
    authent_add_user();
}

bool attemp() {
    char pin[5];
    int attempts = 0;

    while (attempts < 3) {
        serial_read_pin(pin);

        int idx = authent_chercher_user(pin);
        if (idx >= 0) {
            Serial.print("LOGIN OK : ");
            Serial.println(user_table[idx].username);

            lcd_show_message("LOGIN OK", user_table[idx].username);
            delay(1500);
            return true;
        }

        attempts++;

        Serial.print("PIN incorrect. Tentatives restantes : ");
        Serial.println(3 - attempts);

        char line2[17];
        sprintf(line2, "Rest:%d", 3 - attempts);
        lcd_show_message("PIN incorrect", line2);
        delay(1000);
    }

    return false;
}

bool authent_login() {
    return attemp();
}

void afficher_users() {
    Serial.println("=== Liste des utilisateurs ===");

    if (user_count == 0) {
        Serial.println("Aucun utilisateur.");
        lcd_show_message("Aucun user", "");
        delay(1500);
        return;
    }

    for (int i = 0; i < user_count; i++) {
        Serial.print("ID: ");
        Serial.print(user_table[i].id);
        Serial.print(" | Username: ");
        Serial.print(user_table[i].username);
        Serial.print(" | PIN: ");
        Serial.println(user_table[i].code_PIN);

        lcd_show_message("USER:", user_table[i].username);
        delay(1500);
    }
}