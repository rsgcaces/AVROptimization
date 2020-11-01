// PROJECT  :AT28C16EEPROMShieldVersion
// PURPOSE  :Flashes the AT28C16 (2Kx8) EEPROM IC for Program, Control, and 7-Segment Hex Codes
// COURSE   :ICS4U
// AUTHOR   :B. Eater. adapted for ACES' CHUMP use by C. D'Arcy
// DATE     :Confirmed: 2020 11 01
// MCU      :UNO
// STATUS   :Working (on RSGC ACES EEPROM Burner Shield for 28C16A-15)
// NOTE     :Close as many other open applications as possible to
//          :provide as little background mysteries and as much free RAM
//          :as possible. This has proven to be a 'sensitive' sketch.
// OPTION   :Adjust boolean variables to perform Write and/or Read
//WORKSHEET :CHUMPPartsList.xlsx
//REFERENCE :B. Eater Github...
//          :https://github.com/beneater/eeprom-programmer/blob/master/eeprom-programmer/eeprom-programmer.ino
//REFERENCE :B. Eater Videos
//          1. Using an EEPROM to replace combinational logic
//          https://www.youtube.com/watch?v=BA12Z7gQ4P0&feature=emb_logo
//          2. Build an Arduino EEPROM programmer
//          https://www.youtube.com/watch?v=K88pgWhEb1M&feature=emb_logo
//VIDEOS    :C. D'Arcy's Breadboard Burner (BBB)...
//            https://drive.google.com/file/d/1uVkS9kr7deTCG3GVGE0JxhzmMqhn4OrR/view
//          :C. D'Arcy's EEPROM Shield...
//            https://drive.google.com/file/d/12FoAc5GdYHuSdHf3LrMWL7VSGYHGxhIQ/view
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ACES Encoding of Instructions is changed from Feinberg to 
//  facilitate simpler identification of the two branching instructions 
// CHUMPanese INSTRUCTION SET where n: 0-constant, 1-memory (RAM)
//  000n  LOAD
//  001n  ADD 
//  010n  SUBTRACT
//  011n  STORETO
//  100n  READ
//  101n  User-defined  // placed here to facilitate branch coding below
//  110n  GOTO          // Moved here for upper two bits high
//  111n  IFZERO        //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Feinberg's Basic CHUMPanese Program Example
//  0000: 10000010  READ    2   ;addr<-2
//  0001: 00010000  LOAD    IT  ;accum<-mem[addr]
//  0002: 00100001  ADD     1   ;accum++
//  0003: 01100010  STORETO 2   ;mem[2]<-accum
//  0004: 11000000  GOTO    0   ;pc<-0000
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// I employ a 16-byte buffer for consistent CHUMP writing and reading
#define DFLT  0xFF     //default byte contents for EEPROM write buffer
byte codeWrite[16] = { DFLT, DFLT, DFLT, DFLT, //storage for code written
                       DFLT, DFLT, DFLT, DFLT,
                       DFLT, DFLT, DFLT, DFLT,
                       DFLT, DFLT, DFLT, DFLT
                     };
byte codeRead[16] = { 0, 0, 0, 0, //storage for EEPROM read buffer
                      0, 0, 0, 0,
                      0, 0, 0, 0,
                      0, 0, 0, 0
                    };

//Ensure EEPROM_PAGE set to LOW (Page 0)
//#define PAGE LOW
byte codeC [] = {//Common 2019/2020 Control Codes
// REF: http://www.righto.com/2017/03/inside-vintage-74181-alu-chip-how-it.html
//  SSSSMCAR/W  //S3..S0-Select, M-Mode, C-Carry, A-Accum, R/W-Addr Read/~Write
  0b10101000,   //0xA8  0000 LOAD     const   ALU:B
  0b10101001,   //0xA9  0001 LOAD     IT      ALU:B
  0b10010101,   //0x95  0010 ADD      const   ALU:A plus B
  0b10010101,   //0x95  0011 ADD      IT      ALU:A plus B
  0b01100001,   //0x61  0100 SUBTRACT const   ALU:A minus B
  0b01100001,   //0x61  0101 SUBTRACT IT      ALU:A minus B
  0b00000010,   //0x02  0110 STORETO  const   ALU:ignore
  0b00000010,   //0x02  0111 STORETO  IT      ALU:ignore
  0b00000011,   //0x03  1000 READ     const   ALU:ignore
  0b00000011,   //0x03  1001 READ     IT      ALU:ignore
//--------------------------------------------------------
// Place user function here to facilitate identification of branch instructions
// Generate a Logic 1 on F0..F3, disable the Accum
  0b11001010,   //0x00  1010 USER     const   ALU:?
  0b11001010,   //0x00  1011 USER     IT      ALU:?
//-------------------------------------------------------
  0b11001011,   //0xCB  1100 GOTO     const   ALU:Logic 1 (Z Flag to PCs LOAD) 
  0b11001011,   //0xCB  1101 GOTO     IT      ALU:Logic 1 (Z Flag to PCs LOAD)
  0b00001011,   //0x0B  1110 IFZERO   const   ALU:Not A
  0b00001011    //0x0B  1111 IFZERO   IT      ALU:Not A
};
//Feinberg Example
//Ensure EEPROM_PAGE set to LOW as Feinberg Example should be Page 0
//#define PAGE LOW
byte code0 [] = {
  0b10000010,   //0x82      READ    2
  0b00010000,   //0x10      LOAD    IT
  0b00100001,   //0x21      ADD     1
  0b01100010,   //0x62      STORETO 2
  0b11000000    //0xC0      GOTO    0
};
//CHUMP Workbook: Enhanced Swapping Variables code
//Ensure EEPROM_PAGE set to HIGH as USER code should be Page 1
//#define PAGE HIGH
byte codeS [] = {
  0b00000001, // LOAD 1       accum<-1,pc++       Places a 1 in the accumulator
  0b11000101, // STORETO x    [5]<-accum,pc++     Stores accum (1) in RAM Address 5
  0b00000010, // LOAD 2       accum<-2,pc++       Places a 2 in the accumulator
  0b11001111, // STORETO x    [15]<-accum,pc++    Stores accum (2) in RAM Address 15
  0b10000101, // READ x       addr<-5, pc++
  0b00010000, // LOAD IT      accum[5], pc++
  0b01101111, // STORETO temp [15]<-accum, pc++
  0b10001010, // READ y       addr<-10, pc++
  0b00010000, // LOAD IT      accum<-[10], pc++
  0b01100101, // STORETO x    [5]<-accum, pc++
  0b10001111, // READ temp    addr<-15, pc++
  0b00010000, // LOAD IT      accum<-[15], pc++
  0b01101010, // STORETO y    [10]<-accum, pc++
};
//CHUMP Workbook: while Loop code
//Ensure EEPROM_PAGE set to HIGH as USER code should be Page 1
#define PAGE HIGH
byte code [] = {
  0b00000011, // 0:0x03 LOAD 3       accum<-3,pc++         Places a 3 in the accumulator
  0b01100101, // 1:0x65 STORETO x    [5]<-accum,pc++       Stores accum (3) in RAM Address 5
  0b11100110, // 2:0xE6 IFZERO 6     accum==0?:pc<-6:pc++  
  0b01000001, // 3:0x41 SUBTRACT 1   accum--,pc++          Stores accum (2) in RAM Address 15
  0b01100101, // 4:0x65 STORETO x    [5]<-accum,pc++       Stores accum (?) in RAM Address 5
  0b11000010, // 5:0xC2 GOTO 2       pc<-2
  0b11000000, // 6:0xC0 GOTO 0       pc<-0
};

//CHUMP Test Code: LOAD sequence
//Ensure EEPROM_PAGE set to HIGH as USER code should be Page 1
//#define PAGE HIGH
byte codeL [] = {
  0b00000000, // LOAD 0       accum<-0,pc++       Places a 0 in the accumulator
  0b00000001, // LOAD 1       accum<-1,pc++       Places a 1 in the accumulator
  0b00000010, // LOAD 2       accum<-2,pc++       Places a 2 in the accumulator
  0b00000011, // LOAD 3       accum<-3,pc++       Places a 3 in the accumulator
  0b00000100, // LOAD 4       accum<-4,pc++       Places a 4 in the accumulator
  0b00000101, // LOAD 5       accum<-5,pc++       Places a 5 in the accumulator
  0b00000110, // LOAD 6       accum<-6,pc++       Places a 6 in the accumulator
  0b00000111, // LOAD 7       accum<-7,pc++       Places a 7 in the accumulator
  0b00001000, // LOAD 8       accum<-8,pc++       Places a 8 in the accumulator
  0b00001001, // LOAD 9       accum<-9,pc++       Places a 9 in the accumulator
  0b00001010, // LOAD 10      accum<-10,pc++      Places a 10 in the accumulator
  0b00001011, // LOAD 11      accum<-11,pc++      Places a 11 in the accumulator
  0b00001100, // LOAD 12      accum<-12,pc++      Places a 12 in the accumulator
  0b00001101, // LOAD 13      accum<-13,pc++      Places a 13 in the accumulator
  0b00001110, // LOAD 14      accum<-14,pc++      Places a 14 in the accumulator
  0b11000000  // GOTO 0
};
//CHUMP Test Code: ADD sequence
//Ensure EEPROM_PAGE set to HIGH as USER code should be Page 1
//#define PAGE HIGH
byte codeA [] = {
  0b00000001, // LOAD 1       accum<-1,pc++       Places a 1 in the accumulator
  0b00100001, // ADD 1        accum+=1,pc++       Adds 1 to the accumulator
  0b11000001  // GOTO 1       pc<-1
};

//AT28C16 Codes for LB-602MK2 Dual 7-Segment Hex display
// See CHUMP7Segment.xlsx Spreadsheet
// Segment Order: FGABCDE:  I/O6 | I/O5 | I/O4 | I/O3 | I/O0 | I/O1 | I/O2
//Ensure EEPROM_PAGE set to LOW (Page 0)
//#define PAGE LOW
byte code7 [] = {
  0x5F, 0x0C, 0x3B, 0x3E, 0x6C, 0x76, 0x77, 0x1C, 0x7F, 0x7C, 0xFD, 0x67, 0x53, 0x2F, 0x73, 0x71
};

#define PROG_SIZE sizeof(code)
#define CLK       2     // Supports '595/bargraph read/echo support
#define LATCH     3     // "
#define DATA      4     // "
#define EEPROM_D0 5     // I/O 0 
#define EEPROM_D7 12    // I/O 7
#define EEPROM_PAGE 13  // Supports paging
#define EEPROM_OE 14    // AT28C16 Output Enable 
#define EEPROM_WE 15    // AT28C16 Write Enable  
// Tie /CE (Chip Enable) to ground as permanently enabled
#define EEPROM_A0 16    // Address Line 0
#define EEPROM_A1 17    // Address Line 1
#define EEPROM_A2 18    // Address Line 2
#define EEPROM_A3 19    // Address Line 3

boolean writeIt = true;
boolean readIt = true;
boolean debug = true;

void setup() {
  Serial.begin(9600);                 //Enable logging of EEPROM access
  while (!Serial);                    //Wait until ready...
  //ACES' Paging Feature: Allows 2 CHUMPANESE programs stored in Program EEPROM
  pinMode(EEPROM_PAGE, OUTPUT);       //Feinberg (Page 0) and Personal (Page 1)
  digitalWrite(EEPROM_PAGE, PAGE);     //LOW for Page 0, HIGH for Page 1

  pinMode(EEPROM_A0, OUTPUT);
  pinMode(EEPROM_A1, OUTPUT);
  pinMode(EEPROM_A2, OUTPUT);
  pinMode(EEPROM_A3, OUTPUT);
  digitalWrite(EEPROM_WE, HIGH);
  pinMode(EEPROM_WE, OUTPUT);
  digitalWrite(EEPROM_OE, HIGH);
  pinMode(EEPROM_OE, OUTPUT);

  if (writeIt) {
    for (uint8_t address = 0; address < PROG_SIZE; address++)
      codeWrite[address] = code[address];
    if (debug) {
      //Display the code to be flashed...
      Serial.println("Here's the data to be flashed to EEPROM...");
      for (int address = 0; address < 16; address++) {
        Serial.print(codeWrite[address], HEX);
        Serial.print("\t");
      }
      Serial.println();
      // while(1);
      // Write the code to EEPROM...
      Serial.println("Writing 16 bytes of code to EEPROM...");
    }
    //Write code to EEPROM...
    for (int address = 0; address < 16; address++) {
      writeEEPROM(address, codeWrite[address]);
    }
    if (debug)
      Serial.println("Done");
  }

  if (readIt) {
    // Confirm the write by reading and echoing the code to the Serial Monitor...
    Serial.println("Reading EEPROM");
    printContents();
  }
}

// Read the contents of the EEPROM, print them to the serial monitor
// Shift them out to a 595/bargraph combo...
void printContents() {
  // Zero out the target for the reading of EEPROM just to be sure...
  for (int address = 0; address < 16; address++)
    codeRead[address] = 0;

  //Separate the reading from the displaying...
  for (int address = 0; address < 16; address++) {
    codeRead[address] = readEEPROM(address);
    Serial.print(codeRead[address], HEX);
    Serial.print(' ');
    delay(10);
  }
  Serial.println("Read and Echo completed.");
  Serial.println("Pause for 2 seconds...");
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLK, LSBFIRST, 0);
  digitalWrite(LATCH, HIGH);
  delay(2000);
  Serial.println("Bargraph display...");

  for (uint8_t address = 0; address < 16; address++) {
    Serial.println(codeRead[address], HEX);
    digitalWrite(LATCH, LOW);
    shiftOut(DATA, CLK, MSBFIRST, codeRead[address]);
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
    Serial.println("[" + String(address) + "]" + String(codeWrite[address], HEX));
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
  // nothing to do but hold...
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
