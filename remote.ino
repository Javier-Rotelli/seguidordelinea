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
  // int leftVal = digitalRead(leftIr);
  // int rightVal = digitalRead(rightIr);
  // lStep = makeStep(LStepperPins, lStep, leftVal - 1);
  // rStep = makeStep(RStepperPins, rStep , rightVal + 1);
  // send data only when you receive data:
  if (Serial.available() > 0)
  {
    // read the incoming byte:
    byte byteRead = Serial.read();
    Serial.println(byteRead);
    switch (byteRead)
    {
      // D => 68 => frenar
      case 68:
        break;
      // U => 85 => avanzar
      case 85:
        // check array directions for number
        lStep = makeStep(LStepperPins, lStep, 0);
        rStep = makeStep(RStepperPins, rStep , 2);
        break;
      // L => 75 => girar a la izquierda
      case 75:
        // check array directions for number
        rStep = makeStep(RStepperPins, rStep , 2);
        persist("Z");
        break;
      // R => 82 => girar a la derecha
      case 82:
        // check array directions for number
        lStep = makeStep(LStepperPins, lStep, 0);
        persist("A");
        break;
      // M 77 => => verifyMotor1 (girar a la izq)
      case 77:
        for (int i = 0; i < 2000; i++)
        {
          rStep = makeStep(RStepperPins, rStep , 2);
          delay(1);
        }
        break;
      // m => 109 => verifyMotor2 (girar a la derecha)
      case 109:
        for (int i = 0; i < 2000; i++)
        {
          lStep = makeStep(LStepperPins, lStep, 0);
          delay(1);
        }
        break;
    }
  }
}

void persist(String character)
{
  // TODO show in android
  Serial.println(character);
}

