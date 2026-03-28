// 
// Jolene Michel
// 3/28/2026
//  Personal Project 2: button adder and subtracter
//
//the goal of this code is to a functional counter that adds and subtracts depending on what button you press
// There are two buttons one that is specifially for adding 1, and one for subtracting 1
// when a button is pressed the display will update depending on what the num varibale is at the time
// to make sure that is getting configured correctly the there is a print in function to make sure that it is being added correctly
//

#include <ezButton.h>


String commonType = "CC";  // Change to "CA" if you're using Common Anode
 
// Define ESP32-S3 GPIO pins connected to segments a-g and DP
const int a = 4;
const int b = 5;
const int c = 18;
const int d = 19;
const int e = 17;
const int f = 21;
const int g = 3;
const int dp = 1;  // Decimal point pin
 
// Segment patterns for digits 0 to 9 (Standard pattern)
byte digits[10][7] = {
  // a, b, c, d, e, f, g
  {1, 1, 1, 1, 1, 1, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1},  // 2
  {1, 1, 1, 1, 0, 0, 1},  // 3
  {0, 1, 1, 0, 0, 1, 1},  // 4
  {1, 0, 1, 1, 0, 1, 1},  // 5
  {1, 0, 1, 1, 1, 1, 1},  // 6
  {1, 1, 1, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1},  // 8
  {1, 1, 1, 1, 0, 1, 1}   // 9
};
 
// Segment pin array a–g
const int segmentPins[7] = {a, b, c, d, e, f, g};

#define BUTTON_PIN_1 33
#define BUTTON_PIN_2 32

ezButton button1(BUTTON_PIN_1);
ezButton button2(BUTTON_PIN_2);


int num = 0;

void displayDigit(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], digits[num][i]);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);

  button1.setDebounceTime(100);
  button2.setDebounceTime(100);

    for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

    displayDigit(num);

}

void loop() {

  button1.loop();
  button2.loop();


  if (button1.isPressed()) {
    Serial.println("Button 1 pressed");
        num++;
    if (num > 9) {
      num = 0;
    }
    displayDigit(num);
    Serial.println(num);

  }

  if (button1.isReleased()) {
    Serial.println("Button 1 released");
  }

  if (button2.isPressed()) {
    Serial.println("Button 2 pressed");
        num--;
       if (num < 0) 
       num = 9;

    displayDigit(num);
    Serial.println(num);

  }

  if (button2.isReleased()) {
    Serial.println("Button 2 released");
  }
}