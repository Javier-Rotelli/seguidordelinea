#ifndef HMOTOR
#define HMOTOR

#include "Arduino.h"

int SetDirection(int currentStep, int stepperDirection);

int makeStep(int pins[4], int currentStep, int stepperDirection);

#endif