#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

AsyncDelay delay_10s;
int mode = 0;
const byte leftButton = 4;
volatile bool LBP = 0;

void setup() {

  CircuitPlayground.begin();
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(leftButton), check, RISING);
}

void loop() {

  if (mode == 0) {
   delay_10s.start(10000, AsyncDelay::MILLIS);
    CircuitPlayground.setPixelColor(2, 225, 0, 0);
    for (int i = 0; i <= 9; i++) {
      if (i != 2) {
        CircuitPlayground.setPixelColor(i, 0xFFFFFF);
        delay(100);
        CircuitPlayground.setPixelColor(i, 0x000000);

      } else {
        CircuitPlayground.setPixelColor(2, 255, 255, 255);
        delay(100);
        CircuitPlayground.setPixelColor(2, 255, 0, 0);
      }
      if (LBP == true) {
        if (i == 2) {
          CircuitPlayground.clearPixels();
          delay(300000);
        } else {
          Serial.print("NICE TRY");
        }
      }
    }
  }
  if (delay_10s.isExpired()){
    CircuitPlayground.clearPixels();
    delay(1000);
  }

}



void check() {
  LBP = !LBP;
}