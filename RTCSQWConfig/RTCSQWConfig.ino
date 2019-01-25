// PROJECT  :RTCSQWConfig
// PURPOSE  :To set the frequency of the DS1307 Square Wave by configuring the bits in the Control Register
// DEVICE   :Adafruit's DS1307 RTC Breakout Board
// AUTHOR   :C. D'Arcy
// DATE     :2019 01 25 (prepared for 2018/2019 ICS4U ACES' exercise on External Interrupts)
// uC       :328p
// COURSE   :ICS4U
// STATUS   :Working
// REFERENCE:https://mail.rsgc.on.ca/~cdarcy/Datasheets/DS1307.pdf
//          :https://learn.adafruit.com/ds1307-real-time-clock-breakout-board-kit
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
uint8_t interruptPin = 2;       //SQW: RTC
// RTC based on the DS1307 chip connected via I2C and the Wire library
enum Ds1307SqwPinMode { OFF = 0x00, ON = 0x80, SquareWave1HZ = 0x10, SquareWave4kHz = 0x11, SquareWave8kHz = 0x12, SquareWave32kHz = 0x13 };
#define DS1307_ADDRESS  0x68
#define DS1307_CONTROL 0x07
tmElements_t tm;
boolean triggered = false;

void setup () {
  Serial.begin(9600);
  while (!Serial);
  pinMode(A2, OUTPUT);          //Source and sink 5V supply
  pinMode(A3, OUTPUT);          //from the Arduino pins
  digitalWrite(A2, LOW);        //...
  digitalWrite(A3, HIGH);       //...
  //configure the Square Wave frequency for 1Hz
  writeSQWPinMode(SquareWave1HZ);
  //RTC SQW Pin is open drain so we need external pullup      
  pinMode(interruptPin, INPUT_PULLUP);  
  attachInterrupt(digitalPinToInterrupt(interruptPin), beat, FALLING);
}

// derived from Adafruit's RTCLib.cpp source code
// void RTC_DS1307::writeSqwPinMode(Ds1307SqwPinMode mode) {
void writeSQWPinMode(Ds1307SqwPinMode mode) {
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(DS1307_CONTROL);
  Wire.write(mode);
  Wire.endTransmission();
}

//External Interrupt triggered by falling edge of SQW pin
void beat() {
  triggered = true;
}

void loop() {
  if (triggered) {
    confirmTime();      //should update the time every second
    triggered = false;  //prepare for the next interrupt
  }
}

void confirmTime() {
  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  }
}
void print2digits(int number) {
  if (number >= 0 && number < 10)
    Serial.write('0');
  Serial.print(number);
}

