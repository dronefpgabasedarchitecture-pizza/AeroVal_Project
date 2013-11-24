/*
Program:    AeroVal
Author:     Valentin Py
Date:       2013

 
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Arduino.h"
void motors_init(void);
void motors_send(void); //values come from global array
void motor_commandAll(int globalCommand);

int motorCommand[8] = {500, 500, 500, 500, 500, 500, 500, 500};

int incomingByte = 0;   // for incoming serial data
int value = 0;


void setup()
{
  Serial.begin(9600);
  motors_init();
}

void loop()
{
    // send data only when you receive data:
  if (Serial.available() > 0) {
          // read the incoming byte:
          incomingByte = Serial.read();
  
          // say what you got:
          Serial.print("I received: ");
          Serial.println(incomingByte, DEC);
          if (incomingByte==112)
          {
            value++; 
          }
          else if (incomingByte==109)
          {
            value--; 
          }
          else if (incomingByte==120)
          {
            value=2000; 
          }
          else if (incomingByte==121)
          {
            value=1000; 
          }
          Serial.print("value = ");
          Serial.println(value);
          motor_commandAll(value);
  }      
  //motors_send();
  //while(1);
}

void motors_init(void)
{
   DDRB |=0b01100000;  //PB6, PB5 output
   DDRE |=0b00111000;  //PE3,4,5 output
   DDRH |=0b00111000;  //ph3,4,5 output
   
   TCCR1A = 0b10100010; //chanels A+B PWM, non inverting
   TCCR3A = 0b10101010; //chanels A+B+C PWM, non inverting
   TCCR4A = 0b10101010; //chanels A+B+C PWM, non inverting
   
   TCCR1B = 0b00011010; //mode 14 and PS=8
   TCCR3B = 0b00011010; //mode 14 and PS=8
   TCCR4B = 0b00011010; //mode 14 and PS=8
   
   ICR1 = 6666;  // (Fcpu/(PS*Fpwm)) -1
   ICR3 = 6666;  // (Fcpu/(PS*Fpwm)) -1
   ICR4 = 6666;  // (Fcpu/(PS*Fpwm)) -1
    
   //init Thigh=1000us
   OCR1A=2000;
   OCR1B=2000;
   OCR3A=2000;
   OCR3B=2000;
   OCR3C=2000;
   OCR4A=2000;
   OCR4B=2000;
   OCR4C=2000;
   Serial.println("init fini");
}
void motors_send(void)
{
  OCR3B = motorCommand[0] * 2;
  OCR3C = motorCommand[1] * 2;
  OCR3A = motorCommand[2] * 2;
  OCR4A = motorCommand[3] * 2;
  OCR4B = motorCommand[4] * 2;
  OCR4C = motorCommand[5] * 2;
  OCR1B = motorCommand[6] * 2;
  OCR1A = motorCommand[7] * 2;
}

void motor_commandAll(int globalCommand)
{
  globalCommand *=2;
  OCR3B = globalCommand;
  OCR3C = globalCommand;
  OCR3A = globalCommand;
  OCR4A = globalCommand;
  OCR4B = globalCommand;
  OCR4C = globalCommand;
  OCR1B = globalCommand;
  OCR1A = globalCommand;
}
