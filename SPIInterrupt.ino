// PROJECT  :SPIInterrupt
// PURPOSE  :Interrupt-driven coding of SPI transmission to a shift register  
// DEVICE   :Arduino UNO + Morland Bargraph v3.
// AUTHOR   :C. D'Arcy
// DATE     :2019 02 02
// uC       :328p
// COURSE   :ICS3U/ICS4U
// STATUS   :Working
// REFERENCE:http://darcy.rsgc.on.ca/ACES/TEI3M/images/SPIConcept.png
//          :http://darcy.rsgc.on.ca/ACES/TEI3M/SPICommunication/images/SPIvsShiftOut.png
//          :https://shift-register.org/2017/spi-vs-shiftout-on-arduino-and-attiny
// NOTES    :As much as 100x faster than simple shiftout() function
#include <SPI.h>

uint8_t value = 0;            //sample data to upload for confirmation and comparison

void setup() {
  Serial.begin(9600);
  pinMode(SCK, OUTPUT);       //SCK=13 (configure SPI pins for output)
  pinMode(MOSI, OUTPUT);      //MOSI=11
  pinMode(SS, OUTPUT);        //SS=10
  cli();                      //disable GIS
  //SPI Flags: Interrupt Enable | SPI Enable | 1-MSBFIRST; default speed of f_CPU/4
  SPCR = (1 << SPIE) | (1 << SPE) | (1 << DORD) | (1 << MSTR); 
  SPSR = 1<<SPI2X;            //double speed (fast as possible)
  sei();                      //enable GIS
}

ISR(SPI_STC_vect) {           //serial transmission complete; interrupt flag (SPIF) cleared
  PORTB |= 1<<PB2;            //release the slave; '595 stores (presents) the registers as output 
}

void loop() {
  PORTB &= ~(1<<PB2);         //select the slave; '595 latch low 
  SPDR = value;               //writing to the data register starts serial transmission
  value++;                    //increment display value   
  delay(4);                   //eliminate for fastest possible                 
}
