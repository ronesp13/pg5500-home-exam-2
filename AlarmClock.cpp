#include "Arduino.h"
#include "AlarmClock.h"

void AlarmClock::timer1_init() {
    TCCR1A = 0X00;
    TCCR1B = 0X05;
    TCCR1C = 0X00;
    TCNT1 = 0X00;
    TIMSK1 = 0X00;
}

char AlarmClock::logic_value() {
    TCNT1 = 0x00;
    while (!(digitalRead(IR_IN)));
    Pulse_Width = TCNT1;
    TCNT1 = 0;
    if (Pulse_Width >= 7 && Pulse_Width <= 10) {
        while (digitalRead(IR_IN));
        Pulse_Width = TCNT1;
        TCNT1 = 0;
        if (Pulse_Width >= 7 && Pulse_Width <= 10) //560us
            return 0;
        else if (Pulse_Width >= 25 && Pulse_Width <= 27) //1.7ms
            return 1;
    }
    return -1;
}

void AlarmClock::pulse_deal() {
    int i;
    int j;
    ir_code = 0x00;
    adrL_code = 0x00;
    adrH_code = 0x00;

    for (i = 0; i < 16; i++) {
        if(logic_value() == 1)
            ir_code |= (1 << i);
    }
    for (i = 0; i < 8; i++) {
        if(logic_value() == 1)
            adrL_code |= (1 << i);
    }
    for (j = 0; j < 8; j++) {
        if(logic_value() == 1)
            adrH_code |= (1 << j);
    }
}

void AlarmClock::remote_decode() {
    TCNT1 = 0x00;
    while (digitalRead(IR_IN))
    {
        if (TCNT1 >= 1563)
        {
            ir_code = 0x00ff;
            adrL_code = 0x00;
            adrH_code = 0x00;
            return;
        }
    }
    TCNT1 = 0x00;
    while (!(digitalRead(IR_IN)));
    Pulse_Width = TCNT1;
    TCNT1 = 0;
    if (Pulse_Width >= 140 && Pulse_Width <= 141) { //9ms
        while (digitalRead(IR_IN));
        Pulse_Width = TCNT1;
        TCNT1 = 0;
        if (Pulse_Width >= 68 && Pulse_Width <= 72) { //4.5ms
            pulse_deal();
            return;
        }
        else if (Pulse_Width >= 34 && Pulse_Width <= 36) { //2.25ms
            while (!(digitalRead(IR_IN)));
            Pulse_Width = TCNT1;
            TCNT1 = 0;
            if (Pulse_Width >= 7 && Pulse_Width <= 10) { //560us
                return;
            }
        }
    }
}

void AlarmClock::setupScreen() {
    screen.begin();
    screen.background(0, 0, 0);
    delay(250);
    drawAlarmTime();
}

void AlarmClock::drawAlarmTime() {
    char alarmText[12]; //
    char alarmTime[6];
    String tmp = getAlarmTime();
    tmp.toCharArray(alarmTime, 6);
    SET_ALARM.toCharArray(alarmText, 12);//
    screen.background(0, 0, 0);//
    screen.stroke(255, 255, 255);
    screen.setTextSize(2);
    screen.text(alarmText, 15, 30); //
    screen.text(alarmTime, 50, 60);
    screen.setTextSize(1);
}

void AlarmClock::setupRemote() {
    pinMode(IR_IN, INPUT);
    timer1_init();
}

void AlarmClock::setupClock() {
    if (!clock.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    if (!clock.isrunning()) {
        Serial.println("RTC is NOT running!");
        clock.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

void AlarmClock::setup() {
    setupRemote();
    setupClock();
    setupScreen();
    Serial.println("Finished alarmclock ctor");
}

String AlarmClock::getAlarmTime() {
    String alarm = String(first);
    alarm += second;
    alarm += ":";
    alarm += third;
    alarm += fourth;
    return alarm;
}

void AlarmClock::keyPressedWhenSettingAlarm(int code) {
    if (currentIndex == 0) {
        if (code == 82 || code == 22 || code == 25) {
            currentIndex++;
            first = codeToChar(code);
            drawAlarmTime();
        }
    } else if (currentIndex == 1) {
        if (first == '2' && (code == 82 || code == 22 || code == 25 || code == 13)) {
            currentIndex++;
            second = codeToChar(code);
            drawAlarmTime();
        } else if ((first == '0' || first == '1') && (code == 82 || code == 22 || code == 25 || code == 13 || code == 12 || code == 24 || code == 94 || code == 8 || code == 28 || code == 90)) {
            currentIndex++;
            second = codeToChar(code);
            drawAlarmTime();
        }
    } else if (currentIndex == 2) {
        if (code == 82 || code == 22 || code == 25 || code == 13 || code == 12 || code == 24) { // accept 0-5 as input
            currentIndex++;
            third = codeToChar(code);
            drawAlarmTime();
        }
    } else {
        if (code == 82 || code == 22 || code == 25 || code == 13 || code == 12 || code == 24 || code == 94 || code == 8 || code == 28 || code == 90) { // accept 0-9 as input
            fourth = codeToChar(code);
            currentIndex = 0;
            drawAlarmTime();
        }
    }

    if (code == 64 && (first != 'x' && second != 'x' && third != 'x' && fourth != 'x')) { // ok button pressed
        currentIndex = 0;
    }
}

char AlarmClock::codeToChar(int code) {
    if (code == 82) return '0';
    else if (code == 22) return '1';
    else if (code == 25) return '2';
    else if (code == 13) return '3';
    else if (code == 12) return '4';
    else if (code == 24) return '5';
    else if (code == 94) return '6';
    else if (code == 8) return '7';
    else if (code == 28) return '8';
    else if (code == 90) return '9';
    return '0';
}

void AlarmClock::keyPressed(int code) {
    if (state == SET_ALARM_STATE) {
        keyPressedWhenSettingAlarm(code);
    } else if (state == CLOCK_STATE) {

    } else if (state == BEEP_STATE) {

    }
}

void AlarmClock::update() {
    now = clock.now();

    remote_decode();
    if (adrL_code != lastCode && adrL_code != 0) {
        lastCode = adrL_code;
        int code = (int) lastCode;
        Serial.println(code);
        keyPressed(code);
    }
}