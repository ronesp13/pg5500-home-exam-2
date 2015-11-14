#include <Wire.h>
#include <SPI.h>
#include <TFT.h>
#include "RTClib.h"
#include "AlarmClock.h"

AlarmClock alarmClock;

void setup() {
    Serial.begin(57600);
    alarmClock.setup();
}

void loop() {
    alarmClock.update();
}
