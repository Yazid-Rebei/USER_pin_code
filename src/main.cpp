#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <stdio.h>

#define NBR_MAX_USER 5
#define SDA_PIN 21
#define SCL_PIN 22

struct user {
    int id;
    char username[20];
    char code_PIN[5];   // 4 chiffres + '\0'
};

user user_table[NBR_MAX_USER];
int user_count = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcd_show_message(const char* line1, const char* line2 = "") {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}

bool PIN_format_valid(const char* PIN) {
    if (strlen(PIN) != 4) return false;

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
            return;
        }

        Serial.println("PIN invalide. Reessayer.");
        lcd_show_message("PIN invalide", "Reessayer");
        delay(1200);
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

void authent_add_user() {
    if (user_count >= NBR_MAX_USER) {
        Serial.println("Nombre max d'utilisateurs atteint.");
        lcd_show_message("Max users", "atteint");
        delay(1500);
        return;
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
            return false;   // false = pas d'echec global
        }

        attempts++;

        Serial.print("PIN incorrect. Tentatives restantes : ");
        Serial.println(3 - attempts);

        char line2[17];
        sprintf(line2, "Rest:%d", 3 - attempts);
        lcd_show_message("PIN incorrect", line2);
        delay(1000);
    }

    return true;   // true = 3 tentatives echouees
}

void authent_login() {
    if (!attemp()) return;

    Serial.println("LOGIN DENIED. Blocage 30s.");
    countdown_wait(30);

    if (!attemp()) return;

    Serial.println("LOGIN DENIED. Blocage 60s.");
    countdown_wait(60);

    Serial.println("YOU ARE BLOCKED.");
    lcd_show_message("YOU ARE BLOCKED", "Restart needed");
    lcd.noBacklight();

    while (true) {
    }
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

void sleep_mode() {
    lcd.clear();
    lcd.noBacklight();
    Serial.println("Sleep mode active. Tape une touche pour reveiller.");

    while (true) {
        if (Serial.available() > 0) {
            clear_serial_buffer();

            lcd.backlight();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("LCD Reveille");
            delay(1000);
            return;
        }
    }
}

void show_menu() {
    Serial.println();
    Serial.println("1: Ajouter user");
    Serial.println("2: Login");
    Serial.println("3: Afficher users");
    Serial.println("4: Sleep mode");
    Serial.println("Choisir :");
}

void setup() {
    Serial.begin(9600);
    Wire.begin(SDA_PIN, SCL_PIN);

    lcd.init();
    lcd.backlight();

    lcd_show_message("Welcome :)", "");
    delay(1000);
}

void loop() {
    show_menu();

    while (Serial.available() == 0) {
    }

    char choice = Serial.read();
    clear_serial_buffer();

    switch (choice) {
        case '1':
            authent_add_user();
            break;

        case '2':
            authent_login();
            break;

        case '3':
            afficher_users();
            break;

        case '4':
            sleep_mode();
            break;

        default:
            Serial.println("Choix invalide.");
            lcd_show_message("Choix invalide", "");
            delay(1000);
            break;
    }
}