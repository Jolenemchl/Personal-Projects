// 
// Jolene Michel
// 3/27/2026
//  Personal Project 1.5: distance light
//
//the goal of this code is to make a working sensor that can read distance from objects. if the sensor is close to a object then the color will change
// the rgb led will change color depending on how close it is. green if far, red if close, and yellow inbetween
// while the sensor is picking up the distance the lcd is displaying the color that is has to be at the certain moment
// to make sure that is getting configured correctly the display also includes the distance that is read to make sure it is following the if statement
//


#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  


const int trigPin = 5;
const int echoPin = 18;

int red = 25;
int blue = 23;
int green = 33;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
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
  analogWrite(red, 255);
  analogWrite(green, 0);
  analogWrite(blue, 0);
    lcd.clear();

      lcd.setCursor(0, 0);
    lcd.print("color:");

    lcd.setCursor(0, 1);

      lcd.print("RED!");
     lcd.setCursor(9, 1);
     lcd.print(distance);

  }
  else if ((distance < 1250) && (distance > 250)) {
  analogWrite(red, 255);
  analogWrite(green, 210);
  analogWrite(blue, 0);
lcd.clear();

       lcd.setCursor(0, 0);
    lcd.print("color:");

    lcd.setCursor(0, 1);

      lcd.print("YELLOW!");
      lcd.setCursor(9, 1);
     lcd.print(distance);


  }
  else {
  analogWrite(red, 51);
  analogWrite(green, 210);
  analogWrite(blue, 51);
    lcd.clear();

      lcd.setCursor(0, 0);
    lcd.print("color:");

    lcd.setCursor(0, 1);

    lcd.print("GREEN!");
      lcd.setCursor(9, 1);
      lcd.print(distance);

  }

  delay(750);
}                                                                                                                                                                                             