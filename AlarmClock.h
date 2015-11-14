#ifndef ALARM_CLOCK_H
#define ALARM_CLOCK_H

#include "Arduino.h"
#include "RTClib.h"
//#include <TFT.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include "Adafruit_ST7735.h"

#define CS 10
#define DC 9
#define RST 8

#define IR_IN A0

#define SPEAKER 7

#define SET_ALARM_STATE 0
#define CLOCK_STATE 1
#define BEEP_STATE 2

class AlarmClock {

private:

    const String SET_ALARM = "STILL ALARM";
    byte state = SET_ALARM_STATE;
    char first = 'x';
    char second = 'x';
    char third = 'x';
    char fourth = 'x';
    byte currentIndex = 0;

    int Pulse_Width = 0;
    int ir_code = 0x00;
    char adrL_code = 0x00;
    char adrH_code = 0x00;
    char lastCode;
    DateTime now;
    Adafruit_ST7735 screen = Adafruit_ST7735(CS, DC, RST);
    RTC_DS1307 clock;
    String lastTime;

    void timer1_init();
    char logic_value();
    void pulse_deal();
    void remote_decode();
    void setupScreen();
    void setupRemote();
    void setupClock();
    String getAlarmTime();
    void keyPressed(int code);
    void keyPressedWhenSettingAlarm(int code);
    char codeToChar(int code);
    String getTime(DateTime dt);
    String getHourMinute(DateTime dt);
    void resetAlarm();

    void drawAlarmTime();
    void drawClock(String currentTime);
    void drawAlarm();
    void drawBeep();

public:
    void setup();
    void update();
};

#endif