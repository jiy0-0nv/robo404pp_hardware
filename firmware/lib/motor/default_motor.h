// Copyright (c) 2021 Juan Miguel Jimeno
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DEFAULT_MOTOR
#define DEFAULT_MOTOR

#include <Arduino.h>
#include "config.h"
#include "pwm.h"
#include "motor_interface.h"

class Generic2: public MotorInterface
{
    private:
        int in_a_pin_;
        int in_b_pin_;
        int pwm_pin_;
        int pwm_bits_;
        float pwm_frequency_;

    protected:
        void forward(int pwm) override
        {
            if (in_a_pin_ < 0) return;
            setLevel(in_a_pin_, HIGH);
            setLevel(in_b_pin_, LOW);
            setPwm(pwm_pin_, abs(pwm));
        }

        void reverse(int pwm) override
        {
            if (in_a_pin_ < 0) return;
            setLevel(in_a_pin_, LOW);
            setLevel(in_b_pin_, HIGH);
            setPwm(pwm_pin_, abs(pwm));
        }

    public:
        Generic2(float pwm_frequency, int pwm_bits, bool invert, int pwm_pin, int in_a_pin, int in_b_pin):
            MotorInterface(invert),
            pwm_frequency_(pwm_frequency),
            pwm_bits_(pwm_bits),
            in_a_pin_(in_a_pin),
            in_b_pin_(in_b_pin),
            pwm_pin_(pwm_pin) {}

        void begin()
        {
            if (in_a_pin_ < 0) return;
#ifdef PCA_BASE
            if (in_a_pin_ < PCA_BASE)
#endif
        {
                pinMode(in_a_pin_, OUTPUT);
                pinMode(in_b_pin_, OUTPUT);
            }
            setupPwm(pwm_pin_, pwm_frequency_, pwm_bits_);
            //ensure that the motor is in neutral state during bootup
            setPwm(pwm_pin_, 0);
        }

        void brake() override
        {
            if (in_a_pin_ < 0) return;
            setPwm(pwm_pin_, 0);
#ifdef USE_SHORT_BRAKE
            setLevel(in_a_pin_, HIGH); // short brake
            setLevel(in_b_pin_, HIGH);
#endif
        }
};

class Generic1: public MotorInterface
{
    private:
        int in_pin_;
        int pwm_pin_;
        int pwm_bits_;
        float pwm_frequency_;

    protected:
        void forward(int pwm) override
        {
        if (in_pin_ < 0) return;
            setLevel(in_pin_, HIGH);
            setPwm(pwm_pin_, abs(pwm));
        }

        void reverse(int pwm) override
        {
        if (in_pin_ < 0) return;
            setLevel(in_pin_, LOW);
            setPwm(pwm_pin_, abs(pwm));
        }

    public:
        Generic1(float pwm_frequency, int pwm_bits, bool invert, int pwm_pin, int in_pin, int unused=-1):
            MotorInterface(invert),
            pwm_frequency_(pwm_frequency),
            pwm_bits_(pwm_bits),
            in_pin_(in_pin),
            pwm_pin_(pwm_pin) {}

        void begin()
        {
            if (in_pin_ < 0) return;
#ifdef PCA_BASE
            if (in_pin_ < PCA_BASE)
#endif
                pinMode(in_pin_, OUTPUT);
            setupPwm(pwm_pin_, pwm_frequency_, pwm_bits_);
            //ensure that the motor is in neutral state during bootup
            setPwm(pwm_pin_, 0);
        }

        void brake() override
        {
        if (in_pin_ < 0) return;
            setPwm(pwm_pin_, 0);
        }
};

class BTS7960: public MotorInterface
{
    private:
        int in_a_pin_;
        int in_b_pin_;
        int pwm_bits_;
        int pwm_max_;
        float pwm_frequency_;

    protected:
        void forward(int pwm) override
        {
            if (in_a_pin_ < 0) return;
#ifdef USE_SHORT_BRAKE
            setPwm(in_a_pin_, pwm_max_ - abs(pwm));
            setPwm(in_b_pin_, pwm_max_); // short brake
#else
            setPwm(in_a_pin_, 0);
            setPwm(in_b_pin_, abs(pwm));
#endif
        }

        void reverse(int pwm) override
        {
            if (in_a_pin_ < 0) return;
#ifdef USE_SHORT_BRAKE
            setPwm(in_b_pin_, pwm_max_ - abs(pwm));
            setPwm(in_a_pin_, pwm_max_); // short brake
#else
            setPwm(in_b_pin_, 0);
            setPwm(in_a_pin_, abs(pwm));
#endif
        }

    public:
        BTS7960(float pwm_frequency, int pwm_bits, bool invert, int unused, int in_a_pin, int in_b_pin):
            MotorInterface(invert),
            pwm_frequency_(pwm_frequency),
            pwm_bits_(pwm_bits),
            in_a_pin_(in_a_pin),
            in_b_pin_(in_b_pin) {}

        void begin()
        {
            if (in_a_pin_ < 0) return;
            pwm_max_ = (1 << pwm_bits_) - 1;
            setupPwm(in_a_pin_, pwm_frequency_, pwm_bits_);
            setupPwm(in_b_pin_, pwm_frequency_, pwm_bits_);
            //ensure that the motor is in neutral state during bootup
            setPwm(in_a_pin_, 0);
            setPwm(in_b_pin_, 0);
        }

        BTS7960(float pwm_frequency, int pwm_bits, bool invert, int in_a_pin, int in_b_pin):
            MotorInterface(invert),
            pwm_frequency_(pwm_frequency),
            pwm_bits_(pwm_bits),
            in_a_pin_(in_a_pin),
            in_b_pin_(in_b_pin) {}

        void brake() override
        {
        if (in_a_pin_ < 0) return;
#ifdef USE_SHORT_BRAKE
            setPwm(in_a_pin_, pwm_max_);
            setPwm(in_b_pin_, pwm_max_); // short brake
#else
            setPwm(in_b_pin_, 0);
            setPwm(in_a_pin_, 0);
#endif
        }
};

class ESC: public MotorInterface
{
    private:
        int pwm_pin_;
    protected:
        void forward(int pwm) override
        {
            if (pwm_pin_ < 0) return;
            setMicro(pwm_pin_, 1500 + pwm);
        }

        void reverse(int pwm) override
        {
        if (pwm_pin_ < 0) return;
            setMicro(pwm_pin_, 1500 + pwm);
        }

    public:
        ESC(float pwm_frequency, int pwm_bits, bool invert, int pwm_pin, int unused=-1, int unused2=-1):
            MotorInterface(invert),
            pwm_pin_(pwm_pin) {}

        void begin()
        {
            if (pwm_pin_ < 0) return;
            setupPwm(pwm_pin_, SERVO_FREQ, SERVO_BITS);
            //ensure that the motor is in neutral state during bootup
            setMicro(pwm_pin_, 1500);
        }

        void brake() override
        {
            if (pwm_pin_ < 0) return;
            setMicro(pwm_pin_, 1500);
        }
};

#endif
