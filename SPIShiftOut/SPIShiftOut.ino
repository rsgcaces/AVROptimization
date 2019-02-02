// PROJECT  :SPIShiftOut
// PURPOSE  :To highlight the overlap between SPI (hardware) and ShiftOut (software) by 
//          :shifting out a single byte to a Morland Bargraph
// DEVICE   :Arduino UNO + 595 Shift Register
// AUTHOR   :C. D'Arcy
// DATE     :2019 01 31
// uC       :328p
// COURSE   :ICS3U/ICS4U
// STATUS   :Working
// REFERENCE:http://darcy.rsgc.on.ca/ACES/TEI3M/images/SPIConcept.png
//          :http://darcy.rsgc.on.ca/ACES/TEI3M/SPICommunication/images/SPIvsShiftOut.png
// NOTES    :This (revealing) code highlights and contrasts the difference between the use
//          :of the (software-based) shiftOut library function and the (hardware-based)
//          :SPI peripheral on the UNO. They are strikingly similar but each has their own
//          :advantages and disadvantages depending on your application.
#include <SPI.h>
#define VALUE 0x03          //sample data to upload for confirmation and comparison

void setup() {
  Serial.begin(9600);       //Useful for time benchmarks to compare speeds
  //comment out one or the other...
  // softwareShiftOut();
  hardwareShiftOut();
}

void softwareShiftOut() {
  pinMode(SCK, OUTPUT);     //System Clock (pin 13) (exploit predefines:)
  pinMode(MOSI, OUTPUT);    //Master Out Slave In (pin 11). No need for MISO (12) in this example
  pinMode(SS, OUTPUT);      //Slave Select (pin 10)
  digitalWrite(SS, LOW);
  shiftOut(MOSI, SCK, LSBFIRST, VALUE); //No control over transfer parameters
  digitalWrite(SS, HIGH);
}

void hardwareShiftOut() {
  //Initializes the SPI bus setting SCK, MOSI, and SS to outputs,
  SPI.begin();              //Required. Pulls SCK and MOSI low, and SS high. Def. MSBFIRST
  //SPI.beginTransaction(SPISettings(16000000, LSBFIRST, SPI_MODE0)); //optional
  digitalWrite(SS, LOW);    //
  SPI.transfer(VALUE);
  digitalWrite(SS, HIGH);
  SPI.end();                //disables SPI Bus (releases pins, leaving pin modes unchanged)
  //SPI.endTransaction();   //optional (use with SPI.beginTransaction to release pins)
}

void loop() {
}
