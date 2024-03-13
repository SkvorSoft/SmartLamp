/*  
    Abschlussprojekt: SmartLamp
    Team: SkvorSoft
    Erstellt: 08.2023
    Letztes Update: 12.03.2024

    Notizen: 
*/

#include <Arduino.h>
#include<myTime.h>
#include<FastLED.h>
#include<effects.h>
#include<GyverButton.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <all_Vars.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


//***********MP3_Player***********
HardwareSerial mySerial(1); // RX:16 TX:17
DFRobotDFPlayerMini myMusic;


//***********Taster***********
GButton MainButton(4);          //HauptTaster Pin4
GButton MusicButton(5);         //MusikTaster Pin5

//***********Photoresistor*********** 
#define lightSens 15        //Photoresistor Pin
bool dynMode=0;             //dynMode ein/aus
int lightSensVal;           
unsigned long lightSensTime;

//***********ShutDown_Timer***********
int SEK;
unsigned long myTimer;
bool TymeMode=0;

void ShutDown(){

  if ( millis() - myTimer > 1000){
    myTimer=millis();
    SEK=SEK-1;
    Blynk.virtualWrite(V6,SEK);
  	    if (SEK==0){
          powerOn=0;
          LCD.clear();
          LCD.setPower(LOW);
          Serial.println("Off");
          lastMode=ModCounter;
          ModCounter=0;
          fillAll(CRGB::Black);
          FastLED.show();
          TymeMode=0;
          myMusic.stop();
          Blynk.virtualWrite(V2,LOW);
          Blynk.virtualWrite(V0,LOW);
          }
        }
        Serial.println(SEK);
        Blynk.virtualWrite(V6,SEK);
}


//***********Start Window***********
void Welcome (){  
  LCD.roundRect(20,5,100,25,OLED_STROKE);
  LCD.setCursor(40,2);
  LCD.setScale(2);
  LCD.print("S");
  LCD.setCursor(51,2);
  LCD.setScale(1);
  LCD.print("kvorSoft");
  LCD.setCursor(51,3);
  LCD.print("martLamp");
  delay(4000);
  LCD.clear();
}


//***********Helligkeit manuel einstellen***********
bool flag=0;
void setBright(){
 
    if (MainButton.isHold() && flag==0){
      Serial.println(brightness);
      FastLED.setBrightness(brightness);
      brightness=brightness+10;
      if (brightness>MAX_BRIGHTNESS){
        brightness=MAX_BRIGHTNESS;
      }
      Blynk.virtualWrite(V1,brightness);
    }

    if (MainButton.isHolded()){
      flag=!flag;
    }

    if (MainButton.isHold() && flag==1){
      Serial.println(brightness);
      FastLED.setBrightness(brightness);
      brightness=brightness-10;
      if (brightness<MIN_BRIGHTNESS){
        brightness=MIN_BRIGHTNESS;
      }
      Blynk.virtualWrite(V1,brightness);
    }
}



void setup() {


//***********MP3_Player***********
 pinMode(BusyMP3,INPUT);
 mySerial.begin(9600, SERIAL_8N1, 16, 17);
 Serial.println();
 Serial.println("DFRobotDemo");
 Serial.println("Initializing DFPlayer ...");
 
 if (!myMusic.begin(mySerial)) { //Использовать серийный порт для взаимодействия
 Serial.println("Unable to begin:");
 Serial.println("1.Please recheck the connection!");
 Serial.println("2.Please insert the SD card!");
 }
 else {
 Serial.println("DFPlayer Mini online.");
 }

myMusic.volume(20); //Установка громкости. От 0 до 30




  //***********OLED***********
  Serial.begin(115200);
  LCD.init();
  LCD.clear();
  Welcome();

  //***********RTC***********
   Wire.begin();
   rtc.begin();
   //rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));

  //***********WS2812 Konfiguration und Initialisation***********

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT / NUM_STRIPS);
  FastLED.setBrightness(brightness);
  FastLED.show(); 

  //***********Handy_App***********
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(testLED,OUTPUT);

}


//***********Variablen_für_HandyApp***********
BLYNK_WRITE(V0){
  int OnOff = param.asInt();
  if (OnOff==0){
    powerOn=0;
    LCD.setPower(LOW);
    Serial.println("Off");
    lastMode=ModCounter;
    ModCounter=0;
    fillAll(CRGB::Black);
    FastLED.show();
  }
  if (OnOff==1){
    powerOn=1;
    LCD.setPower(HIGH);
    ModCounter=lastMode;
  }
  
}

BLYNK_WRITE(V1){
  int hell = param.asInt();
  FastLED.setBrightness(hell);
}

BLYNK_WRITE(V2){
  int ON = param.asInt();
  if (ON==1){
      
      TymeMode=1;
      }
  if (ON==0){
      TymeMode=0;
      LCD.clear();
      }  
  }

BLYNK_WRITE(V3){
  int mody = param.asInt();
  LCD.clear(30,1,120,20);
  ModCounter=mody;
}  

BLYNK_WRITE(V5){
  int R = param[0].asInt();
  int G = param[1].asInt();
  int B = param[2].asInt();
  Serial.println(R);
  Serial.println(G);
  Serial.println(B);

  for(int i=0;i<NUM_LEDS;i++){
    
    leds[i].setRGB(R,G,B);
     FastLED.show();
  }
} 

BLYNK_WRITE(V6){
  int sek = param[0].asInt();
  SEK=sek;
} 

void loop() {

    Blynk.run();

    MainButton.tick();

    MusicButton.tick();

    //***********Einmal betätigen***********
    // Haupttaster
    if(MainButton.isSingle() ){
      powerOn=!powerOn;
      if (powerOn==1){
      LCD.setPower(HIGH);
      Serial.println("ON");
      ModCounter=lastMode;
      Blynk.virtualWrite(V0,HIGH);
      }
      if (powerOn==0){
        LCD.setPower(LOW);
        Serial.println("Off");
        lastMode=ModCounter;
        ModCounter=0;
        fillAll(CRGB::Black);
        FastLED.show();
        Blynk.virtualWrite(V0,LOW);
      }
    }

    if(digitalRead(BusyMP3)==0 and MainButton.isSingle()){
      myMusic.stop();
      LCD.setPower(LOW);
      Serial.println("Off");
      lastMode=ModCounter;
      ModCounter=0;
      fillAll(CRGB::Black);
      FastLED.show();
    }

    //***********Zweimal betätigen***********
    // Haupttaster
    if (MainButton.isDouble()){
      Serial.println("Pressed");
      LCD.clear(30,1,120,20);
      ModCounter++;
    }

    //***********Dreimal betätigen***********
    // Haupttaster
    if (MainButton.isTriple()){
      dynMode=!dynMode;
      LCD.clear();
      }
      if (dynMode==1){
      LCD.setCursor(0,1);
      LCD.setScale(1);
      LCD.println("DM_ON");
      }

    //***********Hintergrund Funktionen***********

    setBright();
    myTime();

    if (TymeMode==1)
    {
      LCD.setCursor(0,1);
      LCD.setScale(1);
      LCD.println("TM_ON");
      ShutDown();
    }


    //*********** Modi Umschalten bei PowerOn***********
    if (powerOn==1){
    switch (ModCounter)
    {
    case 1:
      LCD.setCursor(35,0);
      LCD.print("<White>");
      fillAll(CRGB::White);
      FastLED.show();
      Blynk.virtualWrite(V3,ModCounter);
      break;
    case 2:
      LCD.setCursor(35,0);
      LCD.print("<Light Bugs>");
      lightBugs();
      FastLED.show();
      Blynk.virtualWrite(V3,ModCounter);
      break;
    case 3:
      LCD.setCursor(30,0);
      LCD.print("<Streamer>");
      colors();
      FastLED.show();
      Blynk.virtualWrite(V3,ModCounter);
      break;
    case 4:
      LCD.setCursor(45,0);
      LCD.print("<Runner>");
      runner();
      FastLED.show();
      Blynk.virtualWrite(V3,ModCounter);
      break;
    case 5:
      LCD.setCursor(45,0);
      LCD.print("<Rainbow>");
      rainbow();
      FastLED.show();
      Blynk.virtualWrite(V3,ModCounter);
      break;
    case 6:
      LCD.setCursor(45,0);
      LCD.print("<Party>");
      sparkles();
      FastLED.show();
      Blynk.virtualWrite(V3,ModCounter);
      break;
    case 7:
      LCD.setCursor(35,0);
      LCD.print("<My Color>");
      BLYNK_WRITE(lol);
      FastLED.show();
      Blynk.virtualWrite(V3,ModCounter);
      break;
    case 8:
      LCD.setCursor(35,0);
      LCD.print("<Breath>");
      radioation();
      FastLED.show();
      Blynk.virtualWrite(V3,ModCounter);
      break;
    default:
    Serial.println(ModCounter);
    ModCounter=1;
    break;
    }
  } 

//*********** Musikwiedergabe/Steuerung***********

  if(MusicButton.isHolded()){
    musicFlag=!musicFlag;
  }
  // Einmal Betätigen
  if(MusicButton.isSingle() && digitalRead(BusyMP3)==1){
    Serial.println(digitalRead(BusyMP3));
    myMusic.play();
    lastMode=ModCounter;
    ModCounter=6;
  }

  if(MusicButton.isSingle() && digitalRead(BusyMP3)==0){
    Serial.println(digitalRead(BusyMP3));
    myMusic.pause();
    ModCounter=lastMode;
  }
  //Gedrückt halten
  if(MusicButton.isHold() && digitalRead(BusyMP3)==0){
    if (musicFlag==1){
      myMusic.volumeUp();
    }
    if(musicFlag==0){
      myMusic.volumeDown();
    }
  }
  // Zwei-/Dreimal betätigen
  if(MusicButton.isDouble()){
    myMusic.next();
  }
  if(MusicButton.isTriple()){
    myMusic.previous();
  }
}


/*  FUNKTIONIERT!
BLYNK_WRITE(V5){
  int R = param[0].asInt();
  int G = param[1].asInt();
  int B = param[2].asInt();
  Serial.println(R);
  Serial.println(G);
  Serial.println(B);

  for(int i=0;i<NUM_LEDS;i++){
    leds[i].setRGB(R,G,B);
     FastLED.show();
  }
} 

Diese Funktion funktioniert nicht mehr, weil ADC, an dem PR hängt, während WLAN aktiv ist nicht benutzbar

//***********Helligkeit automatisch***********
int control=0;
void dynaMode () {

    if (millis()-lightSensTime>1000 && dynMode==1){
      lightSensTime=millis();
    // !!!!!! NACH MONTAGE NOCHMAL JUSTIEREN!!!!!!
    lightSensVal=analogRead(lightSens);
    lightSensVal=map(lightSensVal,0,4095,100,600);
    lightSensVal=constrain(lightSensVal,100,600);
    Serial.println(lightSensVal);
    
    if (lightSensVal<289){
      //FastLED.setBrightness(MIN_BRIGHTNESS);
      control=1;
      //Serial.println(brightness);
    }
    else if ((lightSensVal>290) && (lightSensVal <420) ){
      //FastLED.setBrightness(EVE_BRIGHTNESS);
     control=2;
     //Serial.println(brightness);
    }
    else if (lightSensVal>420){
      //FastLED.setBrightness(MAX_BRIGHTNESS);
      control=3;
      //Serial.println(brightness);
    }
    
    switch (control)
    {
    case 1:
      FastLED.setBrightness(MIN_BRIGHTNESS);
      //brightness=10;
      //FastLED.show();
      Serial.println(brightness);
      break;
    case 2:
      FastLED.setBrightness(EVE_BRIGHTNESS);
      //brightness=50;
      //FastLED.show();
      Serial.println(brightness);
      break;
    case 3:
      FastLED.setBrightness(MAX_BRIGHTNESS);
      //brightness=100;
      //FastLED.show();
      Serial.println(brightness);
      break;
    default:
      break;
    }
    
   }
  }



*/




