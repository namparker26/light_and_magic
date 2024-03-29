
// THIS CODE ATTEMPT TO COMBINE THE longbeo_led and light_and_magic scripts (from the bald engineer) 
// PURPOSE: TO CONTROL THE LED MATRIX, USING MSGEQ7
#define msg7RESET 11
#define msg7Strobe 12
#define msg7DCout 0
const int LEDpins[7] = {3,4,5,6,9,9,10}; // there are 5 LEDs and 7 freq bands. So, repeat LEDs

#include "LedControl.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// #include <iostream.h>
int DIN = 13;
int CS = 8;
int CLK = 2;
LedControl lc=LedControl(DIN, CLK, CS,1); //assign pins for led control

//Mario's Ideas
//MAX7219 - using Led Control library to display all   leds one by one
//define pins for led matrix control
#define pushButton 2


void setup() {
  Serial.begin(9600);
  //  initialize the digital pin as an output.
  //  Pin 13 has an LED connected on most Arduino boards:
   for (int x=0; x<7; x++) {
      pinMode(LEDpins[x], OUTPUT); //defining the LEDpin of [x] to be OUTPUT, using in the loop below
   }
   pinMode(msg7RESET, OUTPUT);
   pinMode(msg7Strobe, OUTPUT);

  //  pinMode(pushButton, INPUT); // never actually used in this example.
  //  digitalWrite(pushButton, HIGH); // Enable internal pull-up

  //led matrix set up
  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);

}


void getRandomLedConfiguration(byte *result, int LEDvalue) {
  int col_heights[] = {0, 0, 0, 0, 0, 0, 0, 0};
  // generate random column heights
  for (int i = 0; i < 8; i++) {
    col_heights[i] =  LEDvalue; // random 0->7
    // Serial.println(col_heights[i]);
  }
  
  // all string representation of rows
  String all_rows[] = {"00000000", "00000000", "00000000", "00000000", 
                       "00000000", "00000000", "00000000", "00000000"};
  // Set string representation to random heights
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (row < col_heights[col]) {
        all_rows[row][col] = '1';
        // Serial.println("all_rows[row][col]: " + all_rows[row][col]);
      }
    }
  }

  // convert string to byte array and return it
  for (int i = 0; i < 8; i++) {
    result[i] = byteFromString(all_rows[i]);
    Serial.println("result: "+result[i]);
  }
}

byte byteFromString(String s) {
    byte result = B00000000;
    for (int i = 0; i < 8; i++) {
      if (s[i] == '1') {
        bitSet(result, i);
      }
    }
    return result;
}

void undisplayColumn(int col) {
  // for (int r = n_row-1; r >= 0; r--) {
    // lc.setLed(0, r, col, false);
    lc.setRow(0, col, 00000000);
    delay(20);
  // }
}

void displayLed(int ledMatrixValue, int x){
  for(int i = 0; i < ledMatrixValue; i++){
    lc.setLed(0,x,i, true);
  }
}

void undisplayLed(int ledMatrixValue, int x){
  for(int i = ledMatrixValue-1; i>=0; i--){
    lc.setLed(0,x,i, false);
  }
}


void loop() {
  int col_heights[] = {0, 0, 0, 0, 0, 0, 0, 0};


  digitalWrite(msg7RESET, HIGH); // reset the MSGEQ7's counter
  delay(5);
  digitalWrite(msg7RESET, LOW);

  for (int x = 0; x < 7; x++){
    digitalWrite(msg7Strobe, LOW); // output each DC value for each freq band
    delayMicroseconds(35); // to allow the output to settle
    int spectrumRead = analogRead(msg7DCout);

    int PWMvalue = map(spectrumRead, 0, 1024, 0, 170); // scale analogRead's value to Write's 255 max
    if (PWMvalue < 50)
        PWMvalue = PWMvalue / 2; // bit of a noise filter, so the LEDs turn off at low levels

    int LEDvalue = map(PWMvalue, 0, 170, 0, 8); // scale analogRead's value to Write's led matrix

    analogWrite(LEDpins[x], PWMvalue);
    col_heights[x] = LEDvalue;
    digitalWrite(msg7Strobe, HIGH);
  
    Serial.print(LEDvalue);
    Serial.print(", ");
  };
  for (int x = 0; x <8; x++){
    for (int i = 0; i < col_heights[x]; i++){
      lc.setLed(0, x, i,true);
    }
    
  };
  delay(50);
  lc.clearDisplay(0);
}