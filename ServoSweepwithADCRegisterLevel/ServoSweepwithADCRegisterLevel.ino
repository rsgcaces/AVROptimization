// PROJECT  :ServoSweepwithADCRegisterLevel
// PURPOSE  :Gentle Register-Level Introduction to ADC of potentiometer for servo horn control
// DEVICE   :Arduino
// AUTHOR   :N. Gammon, modified by C. D'Arcy
// DATE     :2019 04 06
// uC       :328p
// COURSE   :ICS4U
// STATUS   :Working
// REFERENCE:https://www.gammon.com.au/adc
#include "prescalers.h"
#define F_CPU 16000000L           //Create the 50Hz Servo PWM Signal: Start with CPU frequency
#define PRESCALE 8                //Divide it by 8
#define FREQ  50                  //Divide it by desired frequency leaves us with...
#define TOP F_CPU/PRESCALE/FREQ   //Timer1 Mode 15 (OCR1A as TOP): 2^24/2^3/50Hz = 40000
#define UPPER 5292                //10% for 180 degrees: Empirical (Theoretical: TOP/10) 
#define LOWER 1200                //5% for 0 degrees: Empirical (Theoretical: TOP/20) 
#define DEBUG                     //use if serial monitoring of readings requested
uint8_t adcPin = 0;               //we'll use A0 ADMUC.MUX0
volatile uint16_t adcReading;     //obtained from within ISR
volatile boolean adcDone;         //flag indicates reading obtained
boolean adcStarted;               //flag indicates ADC underway

void setup () {
#ifdef DEBUG
  Serial.begin (115200);          //fast, since ADCs only 13 instructions
#endif
  Timer1Setup();                  //Mode 15 Fast PWM with OCR1A as TOP
  ADCSetup();                     //configure registers
  DDRB |= 1 << PB2;               //must declare OC1B pin (10) for output
} // end of setup function

// ADC complete ISR
ISR (ADC_vect)  {
  adcReading = ADC;
  // OCR1B = map(adcReading, 0, 1023, LOWER, UPPER);
  OCR1B = (adcReading << 2) + LOWER;
  adcDone = true;
}  // end of ADC_vect ISR

void Timer1Setup() {
  TCCR1A = 1 << COM1A1 | 1 << COM1B1 | 1 << WGM11 | 1 << WGM10;
  TCCR1B = 1 << WGM13 | 1 << WGM12 | T1ps8;     //2^24/2^3
  OCR1A = TOP;                //establsihed 50Hz PWM signal to servo
  OCR1B = LOWER;              //position horn at 0 degrees on startup
} // end of Timer1Setup function

void ADCSetup() {
  DIDR0 = 1 << adcPin;        //turn digital use analog off (save power)
  ADCSRA =  1 << ADEN;        //turn ADC on
  ADCSRA |= 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0; //Prescaler of 128
  ADMUX  =  1 << REFS0 | (adcPin & 0x07); //AVcc and select input port
} //end of ADCSetup function

void loop () {
  if (adcDone) {              //if last reading finished, process it
    adcStarted = false;       //enables a new ADC to begin
#ifdef DEBUG
    Serial.println (adcReading);  //optional serial monitoring of conversions
    delay (500);
#endif
    adcDone = false;          //flag indicates new reading can be undertaken 
  }

  if (!adcStarted) {                  //if we're not doing a conversion, start a new one
    adcStarted = true;                //flag indicates we're starting a new conversion
    ADCSRA |= 1 << ADSC | 1 << ADIE;  //start the next conversion
  }
}  // end of loop function
