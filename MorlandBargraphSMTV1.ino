// PROJECT  :MorlandBargraphSMTV1
// PURPOSE  :To confirm Morland Bargraph SMT version 1 with various animations
// DATE     :2019 02 28
// STATUS   :Working
#define dataPin 13
#define clockPin 12
#define latchPin 11
#define enablePin 3
#define DELAYMS 10
#define R2L 0
#define L2R 1
#define LEDs 24
#define POTT
uint32_t UPPER = 1L << LEDs;
uint32_t n = 1;
uint8_t rate;
void setup() {
  pinMode(dataPin, OUTPUT);   //
  pinMode(clockPin, OUTPUT);  //declare pins for output
  pinMode(latchPin, OUTPUT);  //
  pinMode(enablePin, OUTPUT);  //
  digitalWrite(enablePin, LOW);
#ifdef POT    //currently a one time read on startup to set the rate
  pinMode(A0, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A0, LOW);
  digitalWrite(A2, HIGH);
  rate = analogRead(A1) >> 2;
#endif
#ifndef POT
  rate = DELAYMS;
#endif
  //uncomment one of the animations below
  //shoot(L2R, rate);
  //shoot(R2L, rate);
  // displayRandom();
  // displayAll();
  breathe(5);
}

//utility function used by most (all?) animations below
void shift(uint32_t n) {
  digitalWrite(latchPin, LOW);                      //pull latch LOW
  shiftOut(dataPin, clockPin, MSBFIRST, n >> 16);   //upload byte 2 data
  shiftOut(dataPin, clockPin, MSBFIRST, n >> 8);    //upload byte 1 data
  shiftOut(dataPin, clockPin, MSBFIRST, n );        //upload byte 0  data
  digitalWrite(latchPin, HIGH);                     //present bits on output pins
}

//depending on the direction parameter, single LEDs will fly across the entire range
//from right to left (or left to right), at a rate determined by the second parameter
void shoot(uint8_t dir, uint16_t duration) {
  while (true)
    if (dir == R2L) {
      n = 1;
      while (n < UPPER) {
        shift(n);
        delay(duration);
        n <<= 1;
      }
    }
    else {
      n = UPPER >> 1;
      while (n > 0) {
        shift(n);
        delay(duration);    //admire...
        n >>= 1;            //adjust n
      }
    }
}

void displayRandom() {
  while (true) {
    n = random(UPPER);
    shift(n);
    delay(DELAYMS << 3);
  }
}


void displayAll() {
  n = UPPER - 1;                    //prepare to light 'em all!
  shift(n);                         //do it
}

void breathe(uint8_t duration) {
  displayAll();                     //light the entire range of LEDs
  uint8_t level = 0;                //start with output enable
  int8_t delta = 1;                 //increment decrement of PWM on OE' pin (3)
  while (true) {                    //repeat, indefinitely
    analogWrite(enablePin, level);  //Set the PWM duty cycle
    delay(duration);                //admire...
    level += delta;                 //adjust duty cycle (either increase or decrease)
    delta = level == 255 || level == 0 ? -delta : delta;  //change direction, if necessary
  }
}

void loop() {}
