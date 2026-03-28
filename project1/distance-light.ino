// 
// Jolene Michel
// 3/27/2026
//  Personal Project 1: distance light
//
//the goal of this code is to make a working sensor that can read distance from objects. if the sensor is close to a object then the color will change
// the leds work on a red, yellow, green system. the further it is then the led will be green and the rest will turn off. 
// while the sensor is picking up the distance the lcd is displaying the color that is has to be at the certain moment
// to make sure that is getting configured correctly the display also includes the distance that is read to make sure it is following the if statement
//


#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  


const int trigPin = 5;
const int echoPin = 18;

int red = 25;
int yellow = 23;
int green = 33;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  Serial.begin(115200);

    lcd.init();
  lcd.backlight();
lcd.clear();
}

void loop() {
  long distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  distance = pulseIn(echoPin, HIGH, 30000);

  Serial.println(distance);

  if (distance <= 250) {
    digitalWrite(red, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);
    lcd.clear();

      lcd.setCursor(0, 0);
    lcd.print("color:");

    lcd.setCursor(0, 1);

      lcd.print("RED!");
     lcd.setCursor(9, 1);
     lcd.print(distance);

  }
  else if ((distance < 1250) && (distance > 250)) {
    digitalWrite(red, LOW);
    digitalWrite(yellow, HIGH);
    digitalWrite(green, LOW);
lcd.clear();

       lcd.setCursor(0, 0);
    lcd.print("color:");

    lcd.setCursor(0, 1);

      lcd.print("YELLOW!");
      lcd.setCursor(9, 1);
     lcd.print(distance);


  }
  else {
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(green, HIGH);
    lcd.clear();

      lcd.setCursor(0, 0);
    lcd.print("color:");

    lcd.setCursor(0, 1);

    lcd.print("GREEN!");
      lcd.setCursor(9, 1);
      lcd.print(distance);

  }

  delay(500);
}                                                                                                                                                                                             