#include "Arduino.h"
#include "motor.h"
int steps [8][4] =
{  
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

int Directions [3] = { -1, 0, 1 };

int SetDirection(int currentStep, int stepperDirection)
{  
  currentStep += Directions[stepperDirection];
  return (currentStep + 8) % 8;
}

int makeStep(int pins[4], int currentStep, int stepperDirection)
{
  currentStep = SetDirection(currentStep, stepperDirection);
  // don't write if we are stopped
  for (int i = 0; i < 4 && Directions[stepperDirection]; i++)
  {
    digitalWrite(pins[i], steps[currentStep][i]);
  }
  return currentStep;
}