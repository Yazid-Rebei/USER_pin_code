#include <Arduino.h>
#include "config.h"
#include "esp_sleep.h"

stat_machine etat_courant = STAT_LOKED;
static int nbr_block = 1;

void show_stat_menu(stat_machine etat)
{
    switch (etat)
    {
    case STAT_LOKED:
        if (user_count == 0) {
            Serial.println("NO USER");
            Serial.println("N : ADD FIRST USER");
            lcd_show_message("NO USER", "N:ADD USER");
        } else {
            Serial.println("L : LOGIN");
            Serial.println("N : NEW USER");
            Serial.println("S : SHOW USERS");
            lcd_show_message("L:LOGIN N:NEW", "S:SHOW USERS");
        }
        break;

    case STAT_UNLOCKED:
        Serial.println("1.ADD USER");
        Serial.println("2.SEE USERS");
        Serial.println("0.LOGOUT");
        lcd_show_message("1.ADD USER", "2.SEE 0.LOGOUT");
        break;

    case STAT_BLOCKED:
        lcd_show_message("SYSTEM BLOCKED", "");
        delay(1000);
        break;

    default:
        break;
    }
}

void enter_state(stat_machine new_state)
{
    etat_courant = new_state;

    switch (new_state)
    {
    case STAT_LOKED:
        lcd_show_message("CURRENT STAT :", "LOCKED");
        delay(1000);
        show_stat_menu(new_state);
        break;

    case STAT_UNLOCKED:
        lcd_show_message("CURRENT STAT :", "UNLOCKED");
        delay(1000);
        show_stat_menu(new_state);
        break;

    case STAT_BLOCKED:
        lcd_show_message("CURRENT STAT :", "BLOCKED");
        delay(1000);
        show_stat_menu(new_state);
        break;

    default:
        break;
    }
}

void changer_etat()
{
    switch (etat_courant)
    {
    case STAT_LOKED:
    {
        if (Serial.available() > 0) {
            char cmd = Serial.read();

            if (user_count == 0) {
                if (cmd == 'N' || cmd == 'n') {
                    first_user();
                    enter_state(STAT_LOKED);
                } else {
                    lcd_show_message("NO USER", "PRESS N");
                    delay(1000);
                    enter_state(STAT_LOKED);
                }
                break;
            }

            switch (cmd)
            {
            case 'L':
            case 'l':
                if (authent_login()) {
                    nbr_block = 1;
                    enter_state(STAT_UNLOCKED);
                } else {
                    enter_state(STAT_BLOCKED);
                }
                break;

            case 'N':
            case 'n':
                authent_add_user();
                enter_state(STAT_LOKED);
                break;

            case 'S':
            case 's':
                show_users();
                enter_state(STAT_LOKED);
                break;

            default:
                lcd_show_message("INVALID CMD", "L/N/S");
                delay(1000);
                enter_state(STAT_LOKED);
                break;
            }
        }
        break;
    }

    case STAT_UNLOCKED:
    {
        if (Serial.available() > 0) {
            char choix = Serial.read();

            switch (choix)
            {
            case '1':
                authent_add_user();
                enter_state(STAT_UNLOCKED);
                break;

            case '2':
                afficher_users();
                enter_state(STAT_UNLOCKED);
                break;

            case '0':
                enter_state(STAT_LOKED);
                break;

            default:
                lcd_show_message("CHOIX INVALID", "RETRY");
                delay(600);
                enter_state(STAT_UNLOCKED);
                break;
            }
        }
        break;
    }

    case STAT_BLOCKED:
        countdown_wait(nbr_block * 30);
        nbr_block++;
        enter_state(STAT_LOKED);
        break;

    default:
        lcd_show_message("CHOIX INVALIDE", "");
        break;
    }
}