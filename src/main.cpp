#include <Arduino.h>
#include <Wire.h>
#include "config.h"

void setup() {
    Serial.begin(9600);
    Wire.begin(SDA_PIN, SCL_PIN);

    lcd.init();
    lcd.backlight();

    lcd_show_message("Welcome :)", "");
    delay(1500);

    etat_courant = STAT_LOKED;
}

void loop() {
    changer_etat();
}