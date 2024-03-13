#ifndef myTime_H
#define myTime_H

#include<Arduino.h>
#include<Wire.h>


//OLED
#include <GyverOLED.h>
GyverOLED <SSD1306_128x32, OLED_NO_BUFFER> LCD;

//RTC
#include <RTClib.h>
RTC_DS3231 rtc;
unsigned long resetSec;


void myTime () {
    DateTime now =rtc.now();
    DateTime day=rtc.now();
    //Design
    LCD.fastLineH(20,0,23);
    LCD.fastLineH(20,112,127);

    //Kalender
    LCD.setScale(1);
    LCD.setCursor(115,1);
    LCD.print(day.day(),DEC);

    LCD.setCursor(115,3);
    LCD.print(day.month(),DEC);

    //Uhrzeit
    LCD.setScale(2);
    LCD.setCursor(30,2);
    LCD.print(now.hour());
    
    LCD.setCursor(60,2);
    LCD.print(":");

    LCD.setCursor(75,2);
    LCD.print(now.minute());

    LCD.setScale(1);
    LCD.setCursor(100,2);
    LCD.print(now.second());
    if(now.second()==0 && millis()-resetSec>1000){
        resetSec=millis();
        LCD.clear();
    }

}

void myDay() {
    DateTime day=rtc.now();
     //Design
    LCD.fastLineH(20,0,23);
    LCD.fastLineH(20,112,127);
    //LCD.fastLineH(63,0,127);

    //Kalender
    LCD.setScale(1);
    LCD.setCursor(115,1);
    LCD.print(day.day(),DEC);

    LCD.setCursor(45,4);
    LCD.print("/");

    LCD.setCursor(115,3);
    LCD.print(day.month(),DEC);
    
    LCD.setCursor(70,4);
    LCD.print("/");

    LCD.setCursor(80,4);
    LCD.print(day.year(),DEC);
}

#endif