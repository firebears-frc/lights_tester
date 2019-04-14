#include <Wire.h>

/**
   Test program for Firebears lights boards.

   This program operates as an I2C master sending signals to the I2C client program
   running on our Arduino/lights board.  Every 5 seconds it switchs all the animations
   on all the strips.  When it switchs, it will blink the pin-13 LED on the master
   Arduino for half a second.

   The two Arduinos should have jumpers connecting GND, SDA, and SCL.

   On the client Arduino/lights board, you should see the lights change every 5 seconds.
   Also, the client Arduino's pin-13 LED should light up for half a second every time it
   receives an I2C signal.
*/
const int I2C_ADDRESS = 4;
const int MAX_ANIMATIONS = 9;
const int MAX_PIXELSTRIPS = 3;
const int TIMEOUT_SECS = 5;

unsigned long timeout = 0;
unsigned long wireTimeout = 0;
int currentAnimation = 0;

void setup() {
  timeout = millis() + TIMEOUT_SECS * 1000;
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  if (millis() > timeout) {
    for (int s = 0; s < MAX_PIXELSTRIPS; s++) {
      setAnimation(s, currentAnimation);
    }
    currentAnimation = (currentAnimation + 1) % MAX_ANIMATIONS;
    digitalWrite(LED_BUILTIN, HIGH);
    timeout = millis() + TIMEOUT_SECS * 1000;
  } else if (wireTimeout > 0 && millis() > wireTimeout)  {
    digitalWrite(LED_BUILTIN, LOW);
    wireTimeout = 0;
  }
  delay(5);
}

void setAnimation(int stripNum, int animationNum) {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write((byte)(stripNum + '0'));
  Wire.write((byte)(animationNum + '0'));
  Wire.endTransmission();
  Serial.print("setAnimation(");
  Serial.print(stripNum);
  Serial.print(",");
  Serial.print(animationNum);
  Serial.println(")");
  wireTimeout = millis() + 500;
}

