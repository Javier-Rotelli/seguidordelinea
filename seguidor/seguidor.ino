#include <SoftwareSerial.h>
#include "motor.h"

// Infrarrojos
#define leftIr 12
#define rightIr 13

// Left Stepper
#define IN11  9
#define IN12  8
#define IN13  7
#define IN14  6

// Right Stepper
#define IN21  5
#define IN22  4
#define IN23  3
#define IN24  2

// Bluetooth
SoftwareSerial bt(10, 11);

// Led RGB
#define led_R A1 // R
#define led_G A0 // G
#define led_B A2 // B

// Ultrasónico
#define trigger 48
#define echo 49

int leftVal;
int rightVal;

char rxChar;
#define cantidadMovimiento  2000

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

/* 
 *  Modo en que se encuentra el robor
 *  T: Test
 *  V: Remoto
 *  A: Automático
*/
char modo;

void setup() 
{ 
  // Led RGB
  pinMode(led_R, OUTPUT);
  pinMode(led_G, OUTPUT);
  pinMode(led_B, OUTPUT);

  // Motores
  for (int i = 0; i < 4; i++) 
  {
    pinMode(LStepperPins[i], OUTPUT);
    pinMode(RStepperPins[i], OUTPUT);
  }

  // Infrarrojos
  pinMode(leftIr, INPUT);
  pinMode(rightIr, INPUT);

  // Ultrasónico
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  // Velocidad serial
  Serial.begin(9600);
  bt.begin(9600);
  
  // Inicia en modo TEST
  modo = 'T';
}

void loop()
{ 
  // Pregunto si hay data recibida por el BT 
  if(bt.available())
    rxChar = bt.read();  

  if(rxChar == 'A')
    modo = 'A';

  //Serial.println(rxChar);
  
  switch (modo)
  {
    case 'T':
      testControl(rxChar);
      break;
    case 'A':
      autoControl();
      break;  
    case 'V':
      remoteControl(rxChar);
      break;  
  }
}

void autoControl()
{
  int distance;

  // Necesario para poder leer de forma correcta la data del sensor ultrasónico
    digitalWrite(trigger, LOW);
    delayMicroseconds(5);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);

    //distance = (0.017 * pulseIn(echo, HIGH));  
    distance = 1000;
    
  // Controlo la distancia al obstáculo.
  
  if(distance < 25 && distance > 8)
  {
    int comando;
    
    if(distance > 17)  
    {
      // Informa que tiene un obstáculo cercano
      bt.print((String)9);
      
     analogWrite(led_R, 150);
     analogWrite(led_G, 150);
     analogWrite(led_B, 0);
    }
    else
    {
      // Se frena e informa que debe ser controlado de forma remota
      bt.print((String)8);
      modo = 'V';

     analogWrite(led_R, 255);
     analogWrite(led_G, 0);
     analogWrite(led_B, 0);

     delay(1000);
    }
  }
  else
  {
     analogWrite(led_R, 0);
     analogWrite(led_G, 255);
     analogWrite(led_B, 0);  
  }
  
  int leftVal = digitalRead(leftIr);
  int rightVal = digitalRead(rightIr);

  // Avanzar
  lStep = makeStep(LStepperPins, lStep, leftVal - 1);
  rStep = makeStep(RStepperPins, rStep , rightVal + 1);      
  delay(1);

  rxChar = '0';
}

void testControl(char rx)
{
  analogWrite(led_R, 255);
  analogWrite(led_G, 255);
  analogWrite(led_B, 255);
 
  switch(rx)
  {
    case 'K':
            {
               analogWrite(led_R, 255);
               analogWrite(led_G, 0);
               analogWrite(led_B, 0);
               delay(1000);
               analogWrite(led_R, 0);
               analogWrite(led_G, 255);
               analogWrite(led_B, 0);
               delay(1000);
               analogWrite(led_R, 0);
               analogWrite(led_G, 0);
               analogWrite(led_B, 255);
               delay(1000);
            }
      break;
    case 'M':
           {
             analogWrite(led_R, 0);
             analogWrite(led_G, 0);
             analogWrite(led_B, 255);
             delay(1);
             for(int i = 0; i < cantidadMovimiento; i++)
             {
                lStep = makeStep(LStepperPins, lStep, 0);
                delay(1);
             }
           }  
      break;
    case 'm':
           {
             analogWrite(led_R, 0);
             analogWrite(led_G, 255);
             analogWrite(led_B, 0);
             delay(1);
             for(int i = 0; i < cantidadMovimiento; i++)
             {
                rStep = makeStep(RStepperPins, rStep, 2);    
                delay(1);
             }
           }  
      break;
    case 'I':       
          {  
             analogWrite(led_R, 255);
             analogWrite(led_G, 0);
             analogWrite(led_B, 0);
             delay(1); 
             int infraI = digitalRead(leftIr) + 48;
             bt.print((char)infraI);
          }
      break;    
    case 'i':
          {
             analogWrite(led_R, 150);
             analogWrite(led_G, 150);
             analogWrite(led_B, 0);             
             delay(1);
             int infraR = digitalRead(rightIr) + 48;
             bt.print((char)infraR);
          }
      break;
    case 'S':
           {
             analogWrite(led_R, 0);
             analogWrite(led_G, 150);
             analogWrite(led_B, 150);                         
             delay(1);
              // Necesario para poder leer de forma correcta la data del sensor ultrasónico
              digitalWrite(trigger, LOW);
              delayMicroseconds(5);
              digitalWrite(trigger, HIGH);
              delayMicroseconds(10);
              
              int distance = (0.017 * pulseIn(echo, HIGH));
              bt.print((String)distance);
           }
      break;
  }

  rxChar = '0';    
}

void remoteControl(char rx)
{
  analogWrite(led_R, 150);
  analogWrite(led_G, 0);
  analogWrite(led_B, 150);
  
  switch(rx)
  {
    case 'U':
            for(int i = 0; i < cantidadMovimiento; i++)
            {
              lStep = makeStep(LStepperPins, lStep, 0);
              rStep = makeStep(RStepperPins, rStep , 2);      
              delay(1);
            }
      break;
    case 'L':
            for(int i = 0; i < cantidadMovimiento; i++)
            {
              rStep = makeStep(RStepperPins, rStep , 2);
              delay(1);
            }
      break;
    case 'R':
            for(int i = 0; i < cantidadMovimiento; i++)
            {
              lStep = makeStep(LStepperPins, lStep, 0);
              delay(1);
            }      
      break;
  }

  rxChar = '0';
}
