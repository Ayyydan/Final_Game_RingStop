#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>
#include <Wire.h>
#include <SPI.h>
const uint8_t spTOO[] PROGMEM = { 0x01, 0xD8, 0x41, 0x33, 0x00, 0x33, 0x66, 0x29, 0xA0, 0x58, 0xA3, 0x92, 0x04, 0x63, 0xEE, 0xE5, 0x93, 0x46, 0x5A, 0x98, 0xA5, 0x14, 0xB7, 0x6D, 0x59, 0x61, 0x16, 0x56, 0xD0, 0x35, 0x65, 0x85, 0x6A, 0x5A, 0x43, 0xB6, 0x54, 0x64, 0xC3, 0x95, 0x01, 0xF1, 0x5A, 0xD5, 0x85, 0x78, 0x14, 0xB4, 0x6B, 0x75, 0x27, 0x52, 0x9E, 0xD0, 0x6D, 0xD5, 0x83, 0x6A, 0x7B, 0x60, 0xB7, 0x53, 0x77, 0xE6, 0x6D, 0x8E, 0xFD, 0x4E, 0xDB, 0x68, 0x8C, 0x07 };
const uint8_t spSLOW[] PROGMEM = { 0x04, 0xF8, 0xCB, 0x44, 0x01, 0xBF, 0x86, 0x5B, 0xC0, 0x02, 0x1C, 0x28, 0xD3, 0xC6, 0x1C, 0x55, 0xA2, 0xAD, 0x0F, 0xB3, 0x3D, 0xC5, 0xA4, 0x16, 0x95, 0xE4, 0xF5, 0x64, 0x95, 0x7B, 0x8E, 0x53, 0xDF, 0x9B, 0xAD, 0x22, 0xF1, 0xEA, 0x61, 0xCE, 0xBB, 0x9B, 0xD9, 0xCF, 0xB9, 0x2F, 0x7D, 0x0D, 0x9B, 0xD7, 0x5D, 0x0B, 0x92, 0x27, 0x1E, 0xEE, 0xD4, 0xA5, 0x32, 0x50, 0xDB, 0xD8, 0xD3, 0x5E, 0xEE, 0xF6, 0xB1, 0xDD, 0x55, 0xBB, 0xFC, 0x3F };
const uint8_t spLEVEL[] PROGMEM = { 0x69, 0xAB, 0xC4, 0xB3, 0xD8, 0x92, 0x86, 0x2D, 0x83, 0xEE, 0x60, 0xCD, 0x12, 0xD6, 0x0C, 0x66, 0x45, 0x2C, 0x73, 0x58, 0x0B, 0xA8, 0x53, 0xD6, 0xAC, 0x6D, 0xE9, 0xC0, 0x57, 0xC5, 0xB2, 0xAE, 0xA1, 0xB0, 0x49, 0x0D, 0x7B, 0xBD, 0x86, 0xA2, 0x47, 0x35, 0xE3, 0xF5, 0xEA, 0xB2, 0x4B, 0x4B, 0xCB, 0xC7, 0xA3, 0xCD, 0xDE, 0x23, 0x59, 0x1A, 0x9A, 0x31, 0x8B, 0xB0, 0x54, 0x76, 0xE3, 0xC6, 0x26, 0x5C, 0x2C, 0xCC, 0x76, 0x6B, 0x92, 0xBC, 0x34, 0x95, 0xC6, 0xA3, 0xCE, 0x74, 0xDB, 0x85, 0x3B, 0x8F, 0xBA, 0x90, 0x9C, 0x51, 0xCC, 0xD6, 0xEA, 0x4C, 0x63, 0x56, 0x30, 0x6D, 0xA9, 0x23, 0xCF, 0x59, 0xD0, 0x34, 0xB5, 0xF9, 0x7F };
AsyncDelay delay_10s;
int mode = 0;
const byte leftButton = 4;
const byte rightButton = 5;
const byte slideSwtich = 7;
volatile bool LBP = 0;
volatile bool ss = 0;
volatile bool RBP = 0;


void setup() {

  CircuitPlayground.begin();
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(leftButton), check, RISING);              // set interrupt isr and pins
  attachInterrupt(digitalPinToInterrupt(slideSwtich), slide, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightButton), check2, RISING);            
}

void loop() {
 if (ss == true){                       // indicates wether hard mode is on or not
  CircuitPlayground.redLED(true); 
 } else if (ss == false){
  CircuitPlayground.redLED(false);
 }
  if (RBP == true) {                // right button used as a reset universly
    reset();
    mode = 0;
    RBP = 0;
  } else {
    if (mode == 0) {            // Checking which level the game is set too

      CircuitPlayground.setPixelColor(2, 225, 0, 0);    // sets the pin for the target led to stop on
      for (int i = 0; i <= 9; i++) {                    // for loop starts the leds to spin
        if (i != 2) {                               
          CircuitPlayground.setPixelColor(i, 0xFFFFFF);       
          delay(200);
          CircuitPlayground.setPixelColor(i, 0x000000);

        } else {
          CircuitPlayground.setPixelColor(2, 255, 255, 255);    // keeps the red led red after being passed through
          delay(200);
          CircuitPlayground.setPixelColor(2, 255, 0, 0);
        }
        if (LBP == true) {                        // checks if left button is pressed 
          if (i == 2) {
            CircuitPlayground.clearPixels();        // if stoped on the right pin mode will increase by 1 
            Serial.println("NICE");
            Serial.println(mode);
            CircuitPlayground.playTone(600, 100);
            LBP = 0;
            mode = mode + 1;
          } else if (i != 2) {                        // checks if button is pressed at the wrong time
            if (ss == true) {                         // if hard mode is on, game will reset or mode reset to 0
             CircuitPlayground.clearPixels();
              delay(1000);
              reset();
              mode = 0;
              
            } else if (ss == false) {                 // if hard mode is not on, it will indicate you missing and resume
              Serial.println("NICE TRY");
              CircuitPlayground.speaker.say(spTOO);          // speech demos used from CPX board Speech_demos
              CircuitPlayground.speaker.say(spSLOW);
              for (int j = 0; j > 6; j++) {
                CircuitPlayground.setPixelColor(2, 0xFFFFFF);
                delay(100);
                CircuitPlayground.clearPixels();
              }
            }
          }
          LBP = 0;                                // sets button state back to false
        }
      }
    }
    if (mode == 1) {                  // checks level


      CircuitPlayground.setPixelColor(6, 225, 0, 0);              // different pin used for different levels 
      for (int i = 0; i <= 9; i++) {
        if (i != 6) {
          CircuitPlayground.setPixelColor(i, 0xFFFFFF);
          delay(150);                                               //   delay is decreased each level to create a faster spinning light
          CircuitPlayground.setPixelColor(i, 0x000000);

        } else {
          CircuitPlayground.setPixelColor(6, 255, 255, 255);
          delay(150);
          CircuitPlayground.setPixelColor(6, 255, 0, 0);
        }
        if (LBP == true) {
          if (i == 6) {
            CircuitPlayground.clearPixels();
            Serial.println("NICE");
            Serial.println(mode);
            CircuitPlayground.playTone(600, 100);
            mode = mode + 1;
          } else if (i != 6) {
            if (ss == true) {
             CircuitPlayground.clearPixels();
              delay(1000);
              reset();
              mode = 0;
             
            } else if (ss == false) {
              Serial.println("NICE TRY");
              CircuitPlayground.speaker.say(spTOO);
              CircuitPlayground.speaker.say(spSLOW);
              for (int j = 0; j > 6; j++) {
                CircuitPlayground.setPixelColor(2, 0xFFFFFF);
                delay(100);
                CircuitPlayground.clearPixels();
              }
            }
          }
          LBP = 0;
        }
      }
    }

      if (mode == 2) {                // new level


      CircuitPlayground.setPixelColor(9, 225, 0, 0);
      for (int i = 0; i <= 9; i++) {
        if (i != 9) {
          CircuitPlayground.setPixelColor(i, 0xFFFFFF);
          delay(100);
          CircuitPlayground.setPixelColor(i, 0x000000);

        } else {
          CircuitPlayground.setPixelColor(9, 255, 255, 255);
          delay(100);
          CircuitPlayground.setPixelColor(9, 255, 0, 0);
        }
        if (LBP == true) {
          if (i == 9) {
            CircuitPlayground.clearPixels();
            Serial.println("NICE");
            Serial.println(mode);
            CircuitPlayground.playTone(600, 100);
            mode = mode + 1;
          } else if (i != 9) {
            if (ss == true) {
             CircuitPlayground.clearPixels();
              delay(1000);
              reset();
              mode = 0;
             CircuitPlayground.clearPixels();
              delay(100);
            } else if (ss == false) {
              Serial.println("NICE TRY");
              CircuitPlayground.speaker.say(spTOO);
              CircuitPlayground.speaker.say(spSLOW);
              for (int j = 0; j > 6; j++) {
                CircuitPlayground.setPixelColor(2, 0xFFFFFF);
                delay(100);
                CircuitPlayground.clearPixels();
              }
            }
          }
          LBP = 0;
        }
      }
    }
      if (mode == 3) {                // new level


      CircuitPlayground.setPixelColor(0, 225, 0, 0);
      for (int i = 0; i <= 9; i++) {
        if (i != 0) {
          CircuitPlayground.setPixelColor(i, 0xFFFFFF);
          delay(75);
          CircuitPlayground.setPixelColor(i, 0x000000);

        } else {
          CircuitPlayground.setPixelColor(0, 255, 255, 255);
          delay(75);
          CircuitPlayground.setPixelColor(0, 255, 0, 0);
        }
        if (LBP == true) {
          if (i == 0) {
            CircuitPlayground.clearPixels();
            Serial.println("NICE");
            Serial.println(mode);
            CircuitPlayground.playTone(600, 100);
            mode = mode + 1;
          } else if (i != 0) {
            if (ss == true) {
             CircuitPlayground.clearPixels();
              delay(1000);
              reset();
              mode = 0;
             CircuitPlayground.clearPixels();
              delay(100);
            } else if (ss == false) {
              Serial.println("NICE TRY");
              CircuitPlayground.speaker.say(spTOO);
              CircuitPlayground.speaker.say(spSLOW);
              for (int j = 0; j > 6; j++) {
                CircuitPlayground.setPixelColor(2, 0xFFFFFF);
                delay(100);
                CircuitPlayground.clearPixels();
              }
            }
          }
          LBP = 0;
        }
      }
    }
      if (mode == 4) {                // new level


      CircuitPlayground.setPixelColor(3, 225, 0, 0);
      for (int i = 0; i <= 9; i++) {
        if (i != 3) {
          CircuitPlayground.setPixelColor(i, 0xFFFFFF);
          delay(60);
          CircuitPlayground.setPixelColor(i, 0x000000);

        } else {
          CircuitPlayground.setPixelColor(3, 255, 255, 255);
          delay(60);
          CircuitPlayground.setPixelColor(3, 255, 0, 0);
        }
        if (LBP == true) {
          if (i == 3) {
            CircuitPlayground.clearPixels();
            Serial.println("NICE");
            Serial.println(mode);
            CircuitPlayground.playTone(600, 100);
            mode = mode + 1;
          } else if (i != 3) {
            if (ss == true) {
             CircuitPlayground.clearPixels();
              delay(1000);
              reset();
              mode = 0;
             CircuitPlayground.clearPixels();
              delay(100);
            } else if (ss == false) {
              Serial.println("NICE TRY");
              CircuitPlayground.speaker.say(spTOO);
              CircuitPlayground.speaker.say(spSLOW);
              for (int j = 0; j > 6; j++) {
                CircuitPlayground.setPixelColor(2, 0xFFFFFF);
                delay(100);
                CircuitPlayground.clearPixels();
              }
            }
          }
          LBP = 0;
        }
      }
    }
      if (mode == 5) {            // final level


      CircuitPlayground.setPixelColor(4, 225, 0, 0);
      for (int i = 0; i <= 9; i++) {
        if (i != 4) {
          CircuitPlayground.setPixelColor(i, 0xFFFFFF);
          delay(50);
          CircuitPlayground.setPixelColor(i, 0x000000);

        } else {
          CircuitPlayground.setPixelColor(4, 255, 255, 255);
          delay(50);
          CircuitPlayground.setPixelColor(4, 255, 0, 0);
        }
        if (LBP == true) {
          if (i == 4) {
            CircuitPlayground.clearPixels();
            Serial.println("NICE");
            Serial.println(mode);
            CircuitPlayground.playTone(600, 100);
            mode = mode + 1;
          } else if (i != 4) {
            if (ss == true) {
             CircuitPlayground.clearPixels();
              delay(1000);
              reset();
              mode = 0;
             CircuitPlayground.clearPixels();
              delay(100);
            } else if (ss == false) {
              Serial.println("NICE TRY");
              CircuitPlayground.speaker.say(spTOO);
              CircuitPlayground.speaker.say(spSLOW);
              for (int j = 0; j > 6; j++) {
                CircuitPlayground.setPixelColor(2, 0xFFFFFF);
                delay(100);
                CircuitPlayground.clearPixels();
              }
            }
          }
          LBP = 0;
        }
      }
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    if (mode == 6) {                          // after beating level 5 
      CircuitPlayground.setPixelColor(9, 0xFFFFFF);
      CircuitPlayground.setPixelColor(0, 0xFFFFFF);
      delay(50);
      CircuitPlayground.setPixelColor(8, 0xFFFFFF);
      CircuitPlayground.setPixelColor(1, 0xFFFFFF);
      delay(50);
      CircuitPlayground.setPixelColor(7, 0xFFFFFF);
      CircuitPlayground.setPixelColor(2, 0xFFFFFF);
      delay(50);
      CircuitPlayground.setPixelColor(6, 0xFFFFFF);
      CircuitPlayground.setPixelColor(3, 0xFFFFFF);
      delay(50);
      CircuitPlayground.setPixelColor(5, 0xFFFFFF);
      CircuitPlayground.setPixelColor(4, 0xFFFFFF);
      delay(50);

      CircuitPlayground.setPixelColor(5, 0x000000);
      CircuitPlayground.setPixelColor(4, 0x000000);
      delay(50);
      CircuitPlayground.setPixelColor(6, 0x000000);
      CircuitPlayground.setPixelColor(3, 0x000000);
      delay(50);
      CircuitPlayground.setPixelColor(7, 0x000000);
      CircuitPlayground.setPixelColor(2, 0x000000);
      delay(50);
      CircuitPlayground.setPixelColor(8, 0x000000);
      CircuitPlayground.setPixelColor(1, 0x000000);
      delay(50);
      CircuitPlayground.setPixelColor(9, 0x000000);
      CircuitPlayground.setPixelColor(0, 0x000000);
      delay(50);
    }
  }
}



void slide() {              // interrupt for slide switch
  ss = !ss;
  delay(100);
}


void check() {                // interrupt for left button
  LBP = !LBP;

}

void check2() {               // interrupt for right button
  RBP = !RBP;               
}

void reset() {                                  // color pattern used for reseting the game and failing in hard mode
     CircuitPlayground.clearPixels();                         
    delay(100);
    CircuitPlayground.setPixelColor(0, 0xFFFFFF);
    delay(25);
    CircuitPlayground.setPixelColor(1, 0xFFFFFF);
    delay(25);
    CircuitPlayground.setPixelColor(2, 0xFFFFFF);
    delay(25);
    CircuitPlayground.setPixelColor(3, 0xFFFFFF);
    delay(25);
    CircuitPlayground.setPixelColor(4, 0xFFFFFF);
    delay(25);
    CircuitPlayground.setPixelColor(5, 0xFFFFFF);
    delay(25);
    CircuitPlayground.setPixelColor(6, 0xFFFFFF);
    delay(25);
    CircuitPlayground.setPixelColor(7, 0xFFFFFF);
    delay(25);
    CircuitPlayground.setPixelColor(8, 0xFFFFFF);
    delay(25);
    CircuitPlayground.setPixelColor(9, 0xFFFFFF);
    delay(25);

    CircuitPlayground.setPixelColor(0, 0x000000);
    delay(25);
    CircuitPlayground.setPixelColor(1, 0x000000);
    delay(25);
    CircuitPlayground.setPixelColor(2, 0x000000);
    delay(25);
    CircuitPlayground.setPixelColor(3, 0x000000);
    delay(25);
    CircuitPlayground.setPixelColor(4, 0x000000);
    delay(25);
    CircuitPlayground.setPixelColor(5, 0x000000);
    delay(25);
    CircuitPlayground.setPixelColor(6, 0x000000);
    delay(25);
    CircuitPlayground.setPixelColor(7, 0x000000);
    delay(25);
    CircuitPlayground.setPixelColor(8, 0x000000);
    delay(25);
    CircuitPlayground.setPixelColor(9, 0x000000);
    delay(25);
}
