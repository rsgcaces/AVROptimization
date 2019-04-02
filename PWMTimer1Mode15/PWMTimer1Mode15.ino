// PROJECT  :PWMTimer1Mode15
// PURPOSE  :Hobby Servo control using Register-level commands under Timer1, Mode 15
// DEVICE   :Arduino
// AUTHOR   :C. D'Arcy
// DATE     :2019 03 28
// MCU      :328
// COURSE   :ICS3U
// STATUS   :Working
// REFERENCE:Chapter 16. https://mail.rsgc.on.ca/~cdarcy/Datasheets/ATmega328P.pdf
#include "prescalers.h"
#define F_CPU 16000000L           //MCU Clock frequency
#define PRESCALE 8                //Timer 1 prescale 
#define FREQ 50                   //desired servo PWM frequency
#define TOP F_CPU/PRESCALE/FREQ   //Timer 1, Mode 15 with OCR1A as TOP
#define MIN_DUTY TOP/20           //5% duty cycle (should) place servo horn in home position
#define MAX_DUTY TOP/10           //10% duty cycle (should) place servo horn in max position
#define DEBUG                     //optional Serial Montoring...
uint8_t potPin = A1;              //use a pot to vary the PWM signa's duty cycle (horn position)
uint16_t reading;                 //variable to read the value from the analog pin

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
  while (!Serial);
#endif
  potSetup();                     //Q&D: Pot as Appliance
  PWMSetup();                     //Timer1 required to get down to 50 Hz in this Mode
}

void potSetup() {
  DDRC = 0b00000101;              //Shoulder leads (A&C) configured for output on analog port
  PORTC = 0b00000100;             //Set one shoulder HIGH, the other remains LOW
}

void PWMSetup() {
  DDRB |= 1 << PB2;                                 //must declare OC1B pin for output
  TCCR1A = 1 << COM1B1 | 1 << WGM11 | 1 << WGM10;   //contribute to Mode 15,
  TCCR1B = 1 << WGM13 | 1 << WGM12 | T1ps8;         //complete mode 15, prescale
  OCR1A = TOP;                    //TCNT1 resets when TOP (OCR1A) reached, OC1B is set
  OCR1B = MIN_DUTY;               //when TCNT1 reaches OCR1B, OC1B pin clears
  TCNT1 = 0;                      //not necessary
}

void loop() {
#ifdef DEBUG                      //optional Serial Monitoring
  Serial.println(OCR1B);
#endif
  reading = analogRead(potPin);   //read the 10-bit pot value
  delay(20);                      //let it settle, to be sure
  //scale the pot reading to duty cycle compatible value
  OCR1B = map(reading, 0, 1023, MIN_DUTY, MAX_DUTY);
  delay(1000);                    //give the servo time to reach new position
}

