#include "motor.h"
int leftIr = 8;
int rightIr = 13;

// left Stepper
#define IN11  12
#define IN12  11
#define IN13  10
#define IN14  9

// Right Stepper
#define IN21  6
#define IN22  5
#define IN23  4
#define IN24  3

int LStepperPins [4] =
{
  IN11, IN12, IN13, IN14
};

int RStepperPins [4] =
{
  IN21, IN22, IN23, IN24
};

// Define el paso actual de la secuencia
int lStep = 0;
int rStep = 0;

void setup() 
{ 
  for (int i = 0; i < 4; i++) 
  {
    pinMode(LStepperPins[i], OUTPUT);
    pinMode(RStepperPins[i], OUTPUT);
  }
  pinMode(leftIr, INPUT);
  pinMode(rightIr, INPUT);
  Serial.begin(9600);
}

void loop()
{  
  int leftVal = digitalRead(leftIr);
  int rightVal = digitalRead(rightIr);
  lStep = makeStep(LStepperPins, lStep, leftVal - 1);
  rStep = makeStep(RStepperPins, rStep , rightVal + 1);
  delay(1);
}