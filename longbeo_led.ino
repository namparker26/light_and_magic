#include "LedControl.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// #include <iostream.h>

using namespace std;
int DIN = 12;
int CS = 11;
int CLK = 10;
//Mario's Ideas
//MAX7219 - using Led Control library to display all   leds one by one

LedControl lc=LedControl(DIN, CLK, CS,1);

// int myNumber[] = {1, 2, 3, 4, 5, 6, 7, 8};


// int myNumberSize = sizeof(myNumber) / sizeof(myNumber[0]);

void setup()   {
  Serial.begin(9600);

  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);
}


// void ledLoop (){
//   for (int j = 0; j< 8; j++){

//     for (int i = 0; i < myNumber[j]; i++){
//       lc.setLed(0,j,i, true);
//       delay(100);
//     }
//   }
//   delay(100);
//   for (int j = 7; j >= 0 ; j--){
//     for (int k = myNumber[j] -1 ; k >= 0; k--){
//       lc.setLed(0,j,k, false);
//         Serial.print("k = ");
//       Serial.println(k);
//       delay(100);
//     }

//   }
// }

void randomLoop() {
  // int r = rand() % 8 + 1; // random 0->7
  // Serial.println(r);
  int num_random_col = rand() % 8;
  // for (int i = 0; i < num_random_col; i++) {
  //     displayColumn(i);
  // }
  byte ledConfiguration[8];
  getRandomLedConfiguration(ledConfiguration);
  for (int r = 0; r < 8; r++) {
    lc.setRow(0, r, ledConfiguration[r]);
    delay(20);
  }

  for (int i = 7; i >= 0; i--) {
    undisplayColumn(i);
  }
}


// void displayColumn(int n_row) {

// }

void undisplayColumn(int col) {
  // for (int r = n_row-1; r >= 0; r--) {
    // lc.setLed(0, r, col, false);
    lc.setRow(0, col, 00000000);
    delay(20);
  // }
}

void getRandomLedConfiguration(byte *result) {
  int col_heights[] = {0, 0, 0, 0, 0, 0, 0, 0};
  // generate random column heights
  for (int i = 0; i < 8; i++) {
    col_heights[i] = rand() % 8; // random 0->7
  }
  // all string representation of rows
  String all_rows[] = {"00000000", "00000000", "00000000", "00000000", 
                       "00000000", "00000000", "00000000", "00000000"};
  // Set string representation to random heights
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (row < col_heights[col]) {
        all_rows[row][col] = '1';
      }
    }
  }

  // convert string to byte array and return it
  for (int i = 0; i < 8; i++) {
    result[i] = byteFromString(all_rows[i]);
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

void   loop() {
  randomLoop();
  delay(100);
}