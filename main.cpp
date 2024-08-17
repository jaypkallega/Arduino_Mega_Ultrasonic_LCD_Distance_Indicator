// This code will check distance and indicate if an obstacle/person is away for 5 seconds from 
// a fixed distance ( it is set at 50cm here). Three LEDs indicate status for Present, Away (for 5 secnods) and Absent (when away for more than 5 seconds)
// Arduino MEGA 2560, Ultrasonic sensor HC SR04

#include <Arduino.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27  // <<----- Replace your address here by scanning your i2c display using i2c scanner program to find the code
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);  

// Define pins

const int led1 = 14;
const int led2 = 15;
const int led3 = 16;
const int trigPin = 23;
const int echoPin = 22;

// Define other variables
long duration, distance;
int tCnt;

void setup() {
  // Set pins as output or input
  pinMode(led3, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // initialise trigger count
  tCnt=0;
  // Initialize serial communication for debugging (optional)
    Serial.begin(9600);
  
lcd.begin (16,2); //  <<----- My LCD was 16x2
// Switch on the backlight
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
lcd.setBacklight(HIGH);
lcd.home (); // go home
lcd.print("hi, Distance is: !");  
  
}

void loop() {
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  digitalWrite(led3, LOW);
  delayMicroseconds(2);

  // Set trigPin on for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read pulseIn on echoPin
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance
  distance = duration * 0.034 / 2;
  lcd.setCursor (0,1);        // go to start of 2nd line
  lcd.print(distance,DEC);
  // Check if something is detected
  if (distance <= 50) { // Adjust distance threshold as needed
    digitalWrite(led1, LOW); // Turn off LED1
    digitalWrite(led2, HIGH); // Turn on LED2 to indicate presence
    digitalWrite(led3, LOW); // turn off LED3
    lcd.home (); // go home, start at the first line, first character of LCD
    lcd.print("Present @ :             ");
    tCnt=0;
  } else {
    tCnt++;
    digitalWrite(led3, HIGH); // Turn on LED3to indicate away
    lcd.home (); //  go home, start at the first line, first character of LCD
    lcd.print("Away @ :                ");

    if (tCnt > 5) //this is the duration for which an object can be away from the set distance threshold. Here it is set at 5 x delay(1000) 5 seconds
    {
    digitalWrite(led3, LOW); // Turn off LED3
    digitalWrite(led1, HIGH); // Turn on LED1 to indicate absence
    digitalWrite(led2, LOW); // Turn off LED2
    lcd.home (); //  go home, start at the first line, first character of LCD
    lcd.print("Absent @ :              ");
    }
  }

  // Print distance and LED states for debugging in the serial monitor (optional)
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, LED1: ");
  Serial.print(digitalRead(led1));
  Serial.print(", LED2: ");
  Serial.println(digitalRead(led2));
  Serial.print(", trigCount: ");
  Serial.print(tCnt);
  delay(1000); // Adjust delay as needed. This will determine the distance reading sampling. 
}
