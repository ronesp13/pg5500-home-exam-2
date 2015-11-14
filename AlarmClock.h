#ifndef ALARM_CLOCK_H
#define ALARM_CLOCK_H

#include "Arduino.h"
#include "RTClib.h"
#include <TFT.h>

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
    // State 0
    const String SET_ALARM = "STILL ALARM";
    byte state = SET_ALARM_STATE;
    char first = 'x';
    char second = 'x';
    char third = 'x';
    char fourth = 'x';
    byte currentIndex = 0;

    // State 1


    // State 2

    int Pulse_Width = 0;
    int ir_code = 0x00;
    char adrL_code = 0x00;
    char adrH_code = 0x00;
    char lastCode;
    DateTime now;
    TFT screen = TFT(CS, DC, RST);
    RTC_DS1307 clock;

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

    // view functions
    void drawAlarmTime();

public:
    void setup();
    void update();
};

#endif