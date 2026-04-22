#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define NBR_MAX_USER 5
#define SDA_PIN 21
#define SCL_PIN 22

enum stat_machine {
    STAT_LOKED,
    STAT_UNLOCKED,
    STAT_BLOCKED
};

struct user {
    int id;
    char username[20];
    char code_PIN[5];
};

extern user user_table[NBR_MAX_USER];
extern int user_count;
extern LiquidCrystal_I2C lcd;
extern stat_machine etat_courant;

// auth
void lcd_show_message(const char* line1, const char* line2 = "");
bool PIN_format_valid(const char* PIN);
void clear_serial_buffer();
int authent_chercher_user(const char* pin);
void serial_read_pin(char* buffer);
void countdown_wait(int seconds);
void authent_add_user();
bool attemp();
bool authent_login();
void afficher_users();
void show_users();
void first_user();

// state machine
void show_stat_menu(stat_machine etat);
void enter_state(stat_machine new_state);
void changer_etat();

#endif