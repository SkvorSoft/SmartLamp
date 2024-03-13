#ifndef effects_H
#define effects_H
#include<FastLED.h>
#include<Arduino.h>
#include<all_Vars.h>


CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;

unsigned long runningTime;


//Constante Farben 

//***********Alles mit Weiß füllen***********
void fillAll(CRGB newcolor) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = newcolor;
  }
}


//***********Bugs***********
#define MAX_SPEED 30
#define BUGS_AMOUNT 2
int16_t speed[BUGS_AMOUNT];
int16_t pos[BUGS_AMOUNT];
CRGB bugColors[BUGS_AMOUNT];


bool loadingFlag = true;
void lightBugs() {
  if (loadingFlag) {
    loadingFlag = false;
    for (int i = 0; i < BUGS_AMOUNT; i++) {
      bugColors[i] = CHSV(random(0, 9) * 28, 255, 255);
      pos[i] = random(0, NUM_LEDS);
      speed[i] += random(-5, 6);
    }
  }
  FastLED.clear();
  for (int i = 0; i < BUGS_AMOUNT; i++) {
    speed[i] += random(-5, 6);
    if (speed[i] == 0) speed[i] += (-5, 6);

    if (abs(speed[i]) > MAX_SPEED) speed[i] = 0;
    pos[i] += speed[i] / 10;
    if (pos[i] < 0) {
      pos[i] = 0;
      speed[i] = -speed[i];
    }
    if (pos[i] > NUM_LEDS - 1) {
      pos[i] = NUM_LEDS - 1;
      speed[i] = -speed[i];
    }
    leds[pos[i]] = bugColors[i];
  }
}

//***********Rainbow***********
byte hue;
void colors() {
  if(millis()-runningTime>250){
    runningTime=millis();
  hue += 1;
  CRGB thisColor = CHSV(hue, 255, 255);
  fillAll(CHSV(hue, 255, 255));
  }
}

//***********Laufpin***********
int position;
bool direction;

void runner() {
  if (millis()-runningTime>100){
    runningTime=millis();
  FastLED.clear();
  if (direction) {
    position++;
    if (position > NUM_LEDS - 2) {
      direction = false;
    }
  } else {
    position--;
    if (position < 1) {
      direction = true;
    }
  }
  leds[position] = CRGB::Yellow;
  }
}


//***********Rainbow2***********
void rainbow() {
  hue += 2;
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV((byte)(hue + i * float(255 / NUM_LEDS)), 255, 255);
  
}



//***********Konfetty***********
#define TRACK_STEP 50

int getPixColor(int thisPixel) {
  return (((int)leds[thisPixel].r << 16) | ((long)leds[thisPixel].g << 8 ) | (long)leds[thisPixel].b);
}
void fade() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((uint32_t)getPixColor(i) == 0) continue;
    leds[i].fadeToBlackBy(TRACK_STEP);
  }
}
void sparkles() {
  byte thisNum = random(0, NUM_LEDS);
  if (getPixColor(thisNum) == 0)
    leds[thisNum] = CHSV(random(0, 255), 255, 255);
  fade();
}






//***********NEW***********
int thissat = 255;
int idex=0;
int ihue=0;

void random_RGB (){
 idex = random(0, NUM_LEDS);
  ihue = random(0, 255);
  leds[idex] = CHSV(ihue, thissat, 255);
}

//***********NEW_2 (KUCKEN BEI LANGEM LED)***********
int thishue=random(0,250);
int bouncedirection = 0;
int ibright = 0;
float tcount = 0.0; 

void radioation(){
  int N3  = int(NUM_LEDS / 3);
  int N6  = int(NUM_LEDS / 6);
  int N12 = int(NUM_LEDS / 12);
  for (int i = 0; i < N6; i++ ) {   
    tcount = tcount + .02;
    if (tcount > 3.14) {
      tcount = 0.0;
    }
    ibright = int(sin(tcount) * 255);
    int j0 = (i + NUM_LEDS - N12) % NUM_LEDS;
    int j1 = (j0 + N3) % NUM_LEDS;
    int j2 = (j1 + N3) % NUM_LEDS;
    leds[j0] = CHSV(thishue, thissat, ibright);
    leds[j1] = CHSV(thishue, thissat, ibright);
    leds[j2] = CHSV(thishue, thissat, ibright);
  }

}

//***********NEW_3***********

void lala(){

    idex++;
  int ghue = (thishue + 80) % 255;
  int bhue = (thishue + 160) % 255;
  int N3  = int(NUM_LEDS / 3);
  int N6  = int(NUM_LEDS / 6);
  int N12 = int(NUM_LEDS / 12);
  
  for (int i = 0; i < N3; i++ ) {
    int j0 = (idex + i + NUM_LEDS - N12) % NUM_LEDS;
    int j1 = (j0 + N3) % NUM_LEDS;
    int j2 = (j1 + N3) % NUM_LEDS;
    leds[j0] = CHSV(thishue, thissat,random(50,255));
    leds[j1] = CHSV(ghue, thissat, random(50,255));
    leds[j2] = CHSV(bhue, thissat, random(50,255));
  }  
}

#endif