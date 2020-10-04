// PROJECT  :AT28C16WriteControlEEPROM
// PURPOSE  :Writes data to the AT28C16 (2Kx8) EEPROM IC used for Control Codes
// COURSE   :ICS4U
// AUTHOR   :B. Eater. adapted for ACES CHUMP use by C. D'Arcy
// DATE     :2019 12 07. Confirmed 2020 10 04
// MCU      :Nano/328p
// STATUS   :Working
// NOTE     :Close as many other open applications as possible to
//          :provide as little backround mysteries and as much free RAM
//          :as possible. This has proven to be a 'sensitive' sketch.
//WORKSHEET :CHUMPPartsList.xlsx on my laptop
// REFERENCE:B. Eater Github...
//          :https://github.com/beneater/eeprom-programmer/blob/master/eeprom-programmer/eeprom-programmer.ino
// REFERENCE:B. Eater Videos
//          1. Using an EEPROM to replace combinational logic
//          https://www.youtube.com/watch?v=BA12Z7gQ4P0&feature=emb_logo
//          2. Build an Arduino EEPROM programmer
//          https://www.youtube.com/watch?v=K88pgWhEb1M&feature=emb_logo
// REFERENCE:C. D'Arcy video
//          https://drive.google.com/file/d/1te2hyfPv4tktT8398nUsuvJcwPtQwapH/view
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CHUMP Basic Program Example
//  0000: 10000010  READ    2   ;addr<-2
//  0001: 00010000  LOAD    IT  ;accum<-mem[addr]
//  0002: 00100001  ADD     1   ;accum++
//  0003: 01100010  STORETO 2   ;mem[2]<-accum
//  0004: 10100000  GOTO    0   ;pc<-0000
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool debug = true;
//storage for code read back from EEPROM
byte codeRead[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//based on my analysis in the Excel worksheet referenced above..
/*
  byte code [] = {//Common 2019/2020 Control Codes
  //  SSSSMCAR/W  //S3..S0-Select, M-Mode, C-Carry, A-Accum, R/W-Addr Read/~Write
  0b10101000,   //0xA9  0000 LOAD     const   ALU:B
  0b10101001,   //0xA9  0001 LOAD     IT      ALU:B
  0b10010101,   //0x95  0010 ADD      const   ALU:A plus B
  0b10010101,   //0x95  0011 ADD      IT      ALU:A plus B
  0b01100001,   //0x61  0100 SUBTRACT const   ALU:A minus B
  0b01100001,   //0x61  0101 SUBTRACT IT      ALU:A minus B
  0b00000010,   //0x02  0110 STORETO  const   ALU:ignore
  0b00000010,   //0x02  0111 STORETO  IT      ALU:ignore
  0b00000011,   //0x03  1000 READ     const   ALU:ignore
  0b00000011,   //0x03  1001 READ     IT      ALU:ignore
  0b00000000,   //0x00  1010 FILLER   const   ALU:ignore
  0b00000000,   //0x00  1011 FILLER   IT      ALU:ignore
  0b11001011,   //0xCB  1100 GOTO     const   ALU:Logic 1
  0b11001011,   //0xCB  1101 GOTO     IT      ALU:Logic 1
  0b00001011,   //0x0B  1110 IFZERO   const   ALU:Not A
  0b00001011    //0x0B  1111 IFZERO   IT      ALU:Not A
  };
*/

byte code [] = {//Feinberg Example
  0b10000010,   //0x82
  0b00010000,   //0x10
  0b00100001,   //0x21
  0b01100010,   //0x62
  0b10100000   //0xA0
//  0b10100000    //eof
  };

/*
  byte code [] = {//Josh's Code Original
  0b00000000, // Load 0
  0b01100010, // Store to 2
  0b00000101, // LOAD 5
  0b01100001, // STORETO 1
  0b10000010, // READ 2
  0b00010000, // LOAD IT
  0b10000001, // READ 1
  0b00110000, // ADD IT
  0b01100010, // STORETO 2
  0b10000001, // READ 1
  0b00010000, // LOAD IT
  0b01000001, // SUBTRACT 1
  0b11101110, // IFZERO 14
  0b11000011, // GOTO 3
  0b11001110  // GOTO 14
  };
*/
/*
  byte code [] = {//Josh's Code 2
  0b00000101,     //0x05
  0b01100001,     //0x61
  0b10000001,     //0x82
  0b00010000,     //0x10
  0b01000001,     //0x81
  0b11100111,     //0x30
  0b11000001};
*/
/*
  byte code [] = {//Fola's code
  0b00001010,     //0x0A      LOAD 10
  0b01100101,     //0x65      STORETO 5
  0b10000101,     //0x85      READ  5
  0b00010000,     //0x10      LOAD  IT
  0b11100111,     //0xE7      IFZERO 7
  0b01000001,     //0x41      SUBTRACT  1
  0b11000001,     //0xC1      GOTO  1
  0b11000111      //0xC7      GOTO 7
  }; */
/*
  byte code [] = {//Lank's Code
  0b00000110,     //0x06
  0b01100001,     //0x61
  0b10010001,     //0x91
  0b00000000,     //0x00
  0b01000010,     //0x42
  0b01100001,     //0x61
  0b10000010,     //0x82
  0b00000000,     //0x00
  0b00100001,     //0x21
  0b01100010,     //0x62
  0b10010001,     //0x91
  0b00000000,     //0x00
  0b11101110,     //0xEE
  0b11000010,     //0xC2
  0b10000010     //0x82
  };
*/
/*
  byte code [] = {
  0b10001111,   //Macdonald
  0b00010000,
  0b11000110,     //IFZERO 6
  0b01000001,
  0b11000010,     // GOTO 2
  0b00001111,
  0b01100110,
  0b11000000      //GOTO 0
  }; */
/*
  byte code [] = {
  0b00000101,   //Mazzucca
  0b01100111,
  0b00010110,
  0b10000101,
  0b00010110,
  0b01100111,
  0b11000000    //GOTO 0
  };
*/
/*
  byte code [] = {  //McCutcheon
  0b10000011, //0 READ 3
  0b00010000, //1 LOAD IT
  0b11101101, //2 IFZERO 13
  0b01000001, //3 SUBTRACT 1
  0b11101000, //4 IFZERO 8
  0b01000001, //5 SUBTRACT 1
  0b11101011, //6 IFZERO 11
  0b11000011, //7 GOTO 3
  0b00000001, //8 LOAD 1
  0b01100001, //9 STORETO 1
  0b11001101, //10 GOTO 13
  0b00000010, //11 LOAD 2
  0b01100010, //12 STORETO 2
  0b11001101, //13 GOTO 13
  };
*/
/*
  byte code [] = {
  0b00000100,   //Parker
  0b01000010,
  0b11100110,   //E6
  0b00000010,
  0b01100101,
  0b11001001,     //C9
  0b00000100,
  0b01000001,
  0b01100100,
  0b00000101,
  0b00110000,
  0b01100110,
  0b11000000      //C0    GOTO 0
  };
*/
/*

  byte code [] = { //Peterson
  0b10000011,  //READ     3   ;addr<-3  (assumes contents are 0?)
  0b00010000,  //LOAD     IT  ;accum<-mem[3]
  0b11000100,  //IFZERO   4   ;pc<-0004
  0b11000110,  //GOTO     6   ;pc<-0006
  0b00100101,  //ADD      5   ;accum=accum+5
  0b11000010,  //GOTO     2   ;pc<-0002
  0b00100001,  //ADD      1   ;accum++
  0b01100011,  //STORETO  3   ;mem[3]<-accum
  0b11001000   //GOTO     8   ;infinite loop
  };*/
/*
  byte code [] = {
  0b00000000,   //Pyper...
  0b01100001,
  0b01100010,
  0b10000001,
  0b00010000,
  0b01000101,
  0b11101110,   //IFZERO 14
  0b00100110,   // add 6
  0b01100001,
  0b10000010,
  0b00010000,
  0b00100010,
  0b01100010,
  0b11000011,    //GOTO 3
  0b11001110    // GOTO 14
  };
*/
#define PROG_SIZE sizeof(code)
#define EEPROM_D0 5
#define EEPROM_D7 12
#define EEPROM_WE 13
#define EEPROM_A0 14    //  |PC0
#define EEPROM_A1 15    //  |PC1
#define EEPROM_A2 16    //  |PC2
#define EEPROM_A3 17    //  |PC3
#define EEPROM_OE 18    //  |PC4
// Tie /CE to ground as permanently enabled
#define CLK       2
#define LATCH     3
#define DATA      4
void setup() {
  pinMode(EEPROM_A0, OUTPUT);
  pinMode(EEPROM_A1, OUTPUT);
  pinMode(EEPROM_A2, OUTPUT);
  pinMode(EEPROM_A3, OUTPUT);
  digitalWrite(EEPROM_WE, HIGH);
  pinMode(EEPROM_WE, OUTPUT);
  digitalWrite(EEPROM_OE, HIGH);
  pinMode(EEPROM_OE, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);

  Serial.begin(9600);
  while (!Serial);
  // Zero out the target for the reading of EEPROM just to be sure...
  for (int address = 0; address < PROG_SIZE; address++)
    codeRead[address] = 0;

  if (debug) {
    //Display the code to be flashed...
    Serial.println("Here's the data to be flashed to EEPROM...");
    for (int address = 0; address < PROG_SIZE; address++) {
      Serial.print(code[address], HEX);
      Serial.print("\t");
    }
    Serial.println();
    // Write the code to EEPROM...
    Serial.println("Writing " + String(PROG_SIZE) + " bytes of code to EEPROM...");
  }
  //Write code to EEPROM...
  for (int address = 0; address < PROG_SIZE; address++) {
    writeEEPROM(address, code[address]);
  }
  if (debug) {
    Serial.println("Done");

    // Confirm the write by reading and echoing the code to the Serial Monitor...
    Serial.println("Reading EEPROM");
  }
  printContents();
}

// Read the contents of the EEPROM, print them to the serial monitor
// Shift them out to a 595/bargraph combo.
void printContents() {
  //Separate the reading from the displaying...
  for (int address = 0; address < PROG_SIZE; address++) {
    codeRead[address] = readEEPROM(address);
    Serial.println(codeRead[address], HEX);
    delay(10);
  }
  Serial.println(String(PROG_SIZE)+" bytes read back:");
  for (int address = 0; address < PROG_SIZE; address++) {
    Serial.println(codeRead[address], HEX);
    digitalWrite(LATCH, LOW);
    shiftOut(DATA, CLK, LSBFIRST, codeRead[address]);
    digitalWrite(LATCH, HIGH);
    delay(1000);
  }
}

// Output the address bits and outputEnable signal using shift registers.
void setAddress(int address) {
  for (int pin = EEPROM_A3; pin >= EEPROM_A0; pin--) {
    digitalWrite(pin, address & 0x08);
    address <<= 1;    //destructive
  }
}
/*
   Read a byte from the EEPROM at the specified address.
*/
byte readEEPROM(int address) {
  digitalWrite(EEPROM_WE, HIGH);
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin--) {
    pinMode(pin, INPUT);
  }
  byte value = 0;
  setAddress(address);
  digitalWrite(EEPROM_OE, LOW);
  delayMicroseconds(1);
  digitalWrite(EEPROM_OE, HIGH);
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin--) {
    // if (debug) Serial.print(digitalRead(pin));
    value = (value << 1) + digitalRead(pin);
  }
  //  if (debug) Serial.print(" ");
  return value;
}

//  Write a byte to the EEPROM at the specified address.
void writeEEPROM(int address, byte data) {
  digitalWrite(EEPROM_OE, LOW);
  digitalWrite(EEPROM_WE, HIGH);
  if (debug) {
    Serial.println("[" + String(address) + "]" + String(code[address], HEX));
    delay(10);
  }
  //Set the address
  setAddress(address);
  digitalWrite(EEPROM_OE, HIGH);
  //prepare to write the data...
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {
    pinMode(pin, OUTPUT);
  }
  delay(10);
  //write the data...
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin--) {
    digitalWrite(pin, data & 0x80);
    //    if (pin == EEPROM_D7 - 4) Serial.print(' ');
    //    Serial.print(data & 0x80 ? HIGH : LOW);
    data <<= 1;     //destructive....
  }
  digitalWrite(EEPROM_WE, LOW);
  delayMicroseconds(1);
  digitalWrite(EEPROM_WE, HIGH);
  delay(10);
}

void loop() {
  // nothing left to do...
}

/*  What follows below is Eater's original code beofre I modified it
    for a maximum of 16 addresses for CHUMP programs...

  //   Output the address bits and outputEnable signal using shift registers.
  void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
  }
  // Read a byte from the EEPROM at the specified address.
  byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {
    pinMode(pin, INPUT);
  }
  setAddress(address, true);//outputEnable = true

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin--) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
  }

  // Write a byte to the EEPROM at the specified address.
  void writeEEPROM(int address, byte data) {
  setAddress(address,  false);//outputEnable=false
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin--) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
  }

  // Read the contents of the EEPROM and print them to the serial monitor.
  void printContents() {
  for (int base = 0; base <= 255; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset++) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
  }
*/
