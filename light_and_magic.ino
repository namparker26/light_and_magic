#include "LedControl.h"

#define msg7RESET 9
#define msg7Strobe 8
#define msg7DCout 0
// #define pushButton 2

// const int LEDpins[7] = {3,5,5,6,9,9,10}; // there are 5 LEDs and 7 freq bands. So, repeat LEDs

int DIN = 12;
int CS = 11;
int CLK = 10;
LedControl lc=LedControl(DIN, CLK, CS,1);

void setup() {
   // initialize the digital pin as an output.
   // Pin 13 has an LED connected on most Arduino boards:
  //  for (int x=0; x<7; x++) {
  //     pinMode(LEDpins[x], OUTPUT);
  //  }
   pinMode(msg7RESET, OUTPUT);
   pinMode(msg7Strobe, OUTPUT);
  //  pinMode(pushButton, INPUT); // never actually used in this example.
  //  digitalWrite(pushButton, HIGH); // Enable internal pull-up
  Serial.begin(9600);

  //setup Led
  lc.shutdown(0,false);
  lc.setIntensity(0,3);
  lc.clearDisplay(0);
}
void displayLed(int PWMvalue, int x){
  for(int i = 0; i < PWMvalue; i++){
    lc.setLed(0,x,i, true);
  }
}

void undisplayLed(int PWMvalue, int x){
  for(int i = PWMvalue-1; i>=0; i--){
    lc.setLed(0,x,i, false);
  }
}


void loop() {
   digitalWrite(msg7RESET, HIGH); // reset the MSGEQ7's counter
   delay(5);
   digitalWrite(msg7RESET, LOW);

   for (int x = 0; x < 7; x++){
      digitalWrite(msg7Strobe, LOW); // output each DC value for each freq band
      delayMicroseconds(35); // to allow the output to settle
      int spectrumRead = analogRead(msg7DCout);

      int PWMvalue = map(spectrumRead, 0, 1024, 0, 255); // scale analogRead's value to Write's 8 max
      // if (PWMvalue < 50)
      //    PWMvalue = PWMvalue / 2; // bit of a noise filter, so the LEDs turn off at low levels
      displayLed(PWMvalue, x);
      // undisplayLed(PWMvalue, x);
      Serial.print("Frequenct band ");
      Serial.println(x);
      Serial.println(PWMvalue);
      digitalWrite(msg7Strobe, HIGH);
   }
   
}