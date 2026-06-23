#ifndef PWM_H
#define PWM_H

#ifndef SERVO_FREQ
#define SERVO_FREQ PWM_FREQUENCY
#endif

#ifndef SERVO_BITS
#define SERVO_BITS PWM_BITS
#endif

void initPwm();
void setupPwm(int pin, int freq, int bits); // analogWriteFrequency() and analogWriteResolution()
void setPwm(int pin, int value); // analogWrite()
void setLevel(int pin, int value); // digitalWrite()
void setMicro(int pin, int value); // writeMicroseconds()

#endif
