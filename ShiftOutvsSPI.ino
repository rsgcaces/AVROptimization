// PROJECT  :ShiftOutvsSPI
// PURPOSE  :To highlight the overlap between ShiftOut (software) and SPI (hardware) by
//          :looping through a full byte range to a Morland Bargraph v3.
// DEVICE   :Arduino UNO + Morland Bargraph v3.
// AUTHOR   :C. D'Arcy
// DATE     :2019 02 02
// uC       :328p
// COURSE   :ICS3U/ICS4U
// STATUS   :Working
// REFERENCE:http://darcy.rsgc.on.ca/ACES/TEI3M/images/SPIConcept.png
//          :http://darcy.rsgc.on.ca/ACES/TEI3M/SPICommunication/images/SPIvsShiftOut.png
//          :https://shift-register.org/2017/spi-vs-shiftout-on-arduino-and-attiny
// NOTES    :This (revealing) code highlights and contrasts the difference between the use
//          :of the (software-based) shiftOut library function and the (hardware-based)
//          :SPI peripheral on the UNO. They are strikingly similar but each has their own
//          :advantages and disadvantages depending on your application.
#include <SPI.h>
//Simply activate one of the next two lines. The code will do the rest...
#define SHIFTMODE     //Start with (more common) SHIFTMODE...(notice the flicker)
//#define SPIMODE     //Now (considerably faster) SPIMODE...(flicker-free)

uint8_t value = 0;          //sample data to upload for confirmation and comparison

void setup() {
  Serial.begin(9600);       //Useful for time benchmarks to compare speeds if desired
#ifdef SHIFTMODE
  pinMode(SCK, OUTPUT);     //System Clock (pin 13) (exploit predefines:)
  pinMode(MOSI, OUTPUT);    //Master Out Slave In (pin 11). No need for MISO (12) in this example
  pinMode(SS, OUTPUT);      //Slave Select (pin 10)
#endif
#ifdef SPIMODE
  SPI.begin();
  SPI.beginTransaction(SPISettings(16000000, LSBFIRST, SPI_MODE0));
#endif
}

void loop() {
  digitalWrite(SS, LOW);      //pull down Slave Select line (aka RCLK or Latch) to start
#ifdef SHIFTMODE
  shiftOut(MOSI, SCK, LSBFIRST, value);
#endif
#ifdef SPIMODE
  SPI.transfer(value);
#endif
  digitalWrite(SS, HIGH);     //pull up Slave Select (aka RCLK or Latch) to complete transfer
  delayMicroseconds(100);    //observe flicker with SHIFTMODE but not SPIMODE (100x faster!)
  value++;
}
