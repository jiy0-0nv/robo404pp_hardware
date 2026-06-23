#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "config.h"
#include "syslog.h"
#include "pwm.h"

#ifdef PCA_BASE
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);
#endif

void initPwm()
{
#ifdef PCA_BASE
    pca.begin();
    pca.setPWMFreq(SERVO_FREQ);
#endif
}

#ifdef ESP32
static int pin_to_channel[60];
static int next_channel = 0;
#endif

void setupPwm(int pin, int freq, int bits)
{
    if (pin < 0) return;
#ifdef PCA_BASE
    if (pin < PCA_BASE)
#endif
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, 0);
#ifdef ESP32 // for arduino-esp32 v2 only, use ledcAttach on v3
        pin_to_channel[pin] = next_channel;
        int st = ledcSetup(next_channel, freq, bits);
        ledcAttachPin(pin, next_channel);
        setPwm(pin, 0);
        next_channel++;
#elif defined(PICO)
        analogWriteFreq(freq);
        analogWriteResolution(bits);
#elif defined(TEENSYDUINO)
        analogWriteFrequency(pin, freq);
        analogWriteResolution(bits);
#else
        analogWriteFrequency(freq);
        analogWriteResolution(bits);
#endif
    }
}

void setPwm(int pin, int value)
{
    if (pin < 0) return;
#ifdef PCA_BASE
    if (pin >= PCA_BASE)
        pca.setPin(pin - PCA_BASE, value);
    else
#endif
    {
#ifdef ESP32 // for arduino-esp32 v2 only
      int chn;
        ledcWrite(chn = pin_to_channel[pin], value); // do not run ledcSetup and ledcAttachPin
#else
        analogWrite(pin, value);
#endif
    }
}

void setLevel(int pin, int value)
{
    if (pin < 0) return;
#ifdef PCA_BASE
    if (pin >= PCA_BASE)
        pca.setPin(pin - PCA_BASE, value ? ((1 << PWM_BITS) - 1) : 0);
    else
#endif
    {
        digitalWrite(pin, value);
    }
}

// for servo, micro-second
void setMicro(int pin, int value)
{
    if (pin < 0) return;
    int pulse = value * (float) SERVO_FREQ * (1 << SERVO_BITS) / 1000000;
    setPwm(pin, pulse);
}
