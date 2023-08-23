
// THIS CODE ATTEMPT TO COMBINE THE longbeo_led and light_and_magic scripts (from the bald engineer) 
// PURPOSE: TO CONTROL THE LED MATRIX, USING MSGEQ7
#define msg7RESET 11
#define msg7Strobe 12
#define msg7DCout 0
const int LEDpins[7] = {3,4,5,6,7,9,10}; // there are 5 LEDs and 7 freq bands. So, repeat LEDs

#include "LedControl.h"
int DIN = 13;
int CS = 8;
int CLK = 2;
LedControl lc=LedControl(DIN, CLK, CS,1); //assign pins for led control


void setup() {
  Serial.begin(9600);
  //  initialize the digital pin as an output.
  //  Pin 13 has an LED connected on most Arduino boards:
   for (int x=0; x<7; x++) {
      pinMode(LEDpins[x], OUTPUT); //defining the LEDpin of [x] to be OUTPUT, using in the loop below
   }
   pinMode(msg7RESET, OUTPUT);
   pinMode(msg7Strobe, OUTPUT);

  //led matrix set up
  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);

}



void loop() {
  // create an array for the led matrix's column values
  int col_heights[] = {0, 0, 0, 0, 0, 0, 0, 0};

  digitalWrite(msg7RESET, HIGH); // reset the MSGEQ7's counter
  delay(5);
  digitalWrite(msg7RESET, LOW);

  for (int x = 0; x < 7; x++){
    digitalWrite(msg7Strobe, LOW); // output each DC value for each freq band
    delayMicroseconds(35); // to allow the output to settle
    int spectrumRead = analogRead(msg7DCout);

    int PWMvalue = map(spectrumRead, 0, 1024, 0, 150); // scale analogRead's value to Write's 255 max
    if (PWMvalue < 50)
        PWMvalue = PWMvalue / 2; // bit of a noise filter, so the LEDs turn off at low levels

    analogWrite(LEDpins[x], PWMvalue);
    digitalWrite(msg7Strobe, HIGH);

    // assign the Led value to the matrix array
    int LEDvalue = map(PWMvalue, 0, 150, 0, 8); // scale analogRead's value to Write's led matrix
    col_heights[x] = LEDvalue;

    // print the led into an array-like string
    if (x<6)
    {
      Serial.print(LEDvalue);
      Serial.print(", ");
    }else {
      Serial.print(LEDvalue);
      Serial.println("");}
    
  };

  // Using th Led value to display ledMatrix
  for (int x = 0; x <=7; x++){
    for (int i = 0; i <= col_heights[x]; i++){
      lc.setLed(0, x, i,true);
    }
    
  };
  delay(50); //delay 50 for the led to settle
  lc.clearDisplay(0);   
}