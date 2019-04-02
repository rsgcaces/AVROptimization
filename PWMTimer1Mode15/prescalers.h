// RSGC ACES: ICS4U
// Prescale constants for ATmega328p Timers
#define T0Stopped 0b00000000    // Timer0 stopped
#define T0psNone  0b00000001    // T0:2^24/2^8  (no prescale)> 2^? ovf/s = ? Hz
#define T0ps8     0b00000010    // T0:2^24/2^3/2^8 (prescale)> 2^? ovf/s = ? Hz
#define T0ps64    0b00000011    // T0:2^24/2^6/2^8 (prescale)> 2^? ovf/s = ? Hz
#define T0ps256   0b00000100    // T0:2^24/2^8/2^8 (prescale)> 2^? ovf/s = ? Hz
#define T0ps1024  0b00000101    // T0:2^24/2^10/2^8(prescale)> 2^? ovf/s = ? Hz
#define T1Stopped 0b00000000    // Timer1 stopped
#define T1psNone  0b00000001    // T1:2^24/2^16  (no prescale)> 2^8 ovf/s > 128Hz
#define T1ps8     0b00000010    // T1:2^24/2^3/2^16 (prescale)> 2^5 ovf/s > 16Hz
#define T1ps64    0b00000011    // T1:2^24/2^6/2^16 (prescale)> 2^2 ovf/s > 2Hz
#define T1ps256   0b00000100    // T1:2^24/2^8/2^16 (prescale)> 1 ovf/s   > 0.5Hz
#define T1ps1024  0b00000101    // T1:2^24/2^10/2^16(prescale)> 0.25 ovf/s> 0.125Hz
#define T2Stopped 0b00000000    // Timer2 stopped
#define T2psNone  0b00000001    // T2:2^24/2^8  (no prescale)> 2^? ovf/s > ? Hz
#define T2ps8     0b00000010    // T2:2^24/2^3/2^8 (prescale)> 2^? ovf/s = ? Hz
#define T2ps32    0b00000011    // T2:2^24/2^5/2^8 (prescale)> 2^? ovf/s = ? Hz
#define T2ps64    0b00000100    // T2:2^24/2^6/2^8 (prescale)> 2^? ovf/s = ? Hz
#define T2ps128   0b00000101    // T2:2^24/2^7/2^8(prescale)> 2^? ovf/s = ? Hz
#define T2ps256   0b00000110    // T2:2^24/2^8/2^8(prescale)> 2^? ovf/s = ? Hz
#define T2ps1024  0b00000111    // T2:2^24/2^10/2^8(prescale)> 2^? ovf/s = ? Hz

