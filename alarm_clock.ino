#include <Wire.h>
#include <SPI.h>
#include "RTClib.h"
#include "AlarmClock.h"
#include <Adafruit_GFX.h>
#include "Adafruit_ST7735.h"

AlarmClock alarmClock;

void setup() {
    Serial.begin(57600);
    alarmClock.setup();
}

void loop() {
    alarmClock.update();
}
