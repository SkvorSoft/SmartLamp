#ifndef all_Vars_H
#define all_Vars_H


//***********Ein/Aus_Mody***********
int ModCounter=1;               //Mody Counter
int lastMode=0;
bool powerOn=0;

//***********Für_DF_Player***********
bool musicFlag=0;
#define BusyMP3 23
#define testLED 32

//***********Für_Handy_App***********
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL4sZV8unzN"
#define BLYNK_TEMPLATE_NAME "SmartLamp"
#define BLYNK_AUTH_TOKEN "UXAC5TiroA1agIgqPI8z2lP4m7m2A92h"

//char ssid[] = "Vodafone-0850";
//char pass[] = "mdqQyNUVyNBx6Lxb";

char ssid[] = "KGB Lan";
char pass[] = "kgb12345";


//***********Für_WS2812***********
#define CURRENT_LIMIT 3000  // mA Strom Limit, 0 - ausschalten
#define NUM_LEDS 13 
#define NUM_STRIPS 4        // количество отрезков ленты (в параллели)
#define LED_PIN 2

//***********Helligkeit defines***********
#define BRIGHTNESS 50    // Anfangshelligkeit
#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 250
#define EVE_BRIGHTNESS 50

int brightness = BRIGHTNESS;

#endif