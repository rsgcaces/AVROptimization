
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
// Last Update: August 26, 2018
#include <Wire.h>
#include <RTClib.h>
#include <RTC_DS1307.h>
RTC_DS1307 RTC;

void setup () {
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A2, LOW);
  digitalWrite(A3, HIGH);
  Serial.begin(9600);
  // Setting Baud rate to 9600
  Serial.println("Baud rate set to 9600...");
  Serial.println("Initializing the Wire Library...");
  Wire.begin();
  Serial.println("Initializing the RTC...");
  RTC.begin();

  Serial.println("Flashing the current Date and Time...");
  // RTC.adjust(DateTime(2016,04,7,11,46,0));
  // RTC.adjust(DateTime(2016,11,1,10,11,0));
  // RTC.adjust(DateTime(2017,8,7,9,11,50));
  // 2018
  // RTC.adjust(DateTime(2018,8,25,17,43,00));
  //Use this one (set for an hour ahead) if flashing EST
  //  RTC.adjust(DateTime(2019,0,22,9,44,0));
  RTC.adjust(DateTime(__DATE__, __TIME__));
  //  if (! RTC.isrunning()) {
  //   Serial.println("RTC is NOT running!");
  // following line sets the RTC to the date & time this sketch was compiled
  //   RTC.adjust(DateTime(__DATE__, __TIME__));
  //  }
  Serial.println("Ending Setup()");
}

void loop () {
  DateTime now = RTC.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(now.unixtime());
  Serial.print("s = ");
  Serial.print(now.unixtime() / 86400L);
  Serial.println("d");

  // calculate a date which is 7 days and 30 seconds into the future
  DateTime future (now.unixtime() + 7 * 86400L + 30);

  Serial.print(" now + 7d + 30s: ");
  Serial.print(future.year(), DEC);
  Serial.print('/');
  Serial.print(future.month(), DEC);
  Serial.print('/');
  Serial.print(future.day(), DEC);
  Serial.print(' ');
  Serial.print(future.hour(), DEC);
  Serial.print(':');
  Serial.print(future.minute(), DEC);
  Serial.print(':');
  Serial.print(future.second(), DEC);
  Serial.println();

  Serial.println();
  delay(3000);
}
// vim:ci:sw=4 sts=4 ft=cpp
