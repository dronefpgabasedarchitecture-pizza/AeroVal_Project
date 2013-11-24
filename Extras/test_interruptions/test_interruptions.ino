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

void  init_Receiver(void);
#define NUMBER_OF_CHANELS 7
#define RISING_EDGE 1
#define FALLING_EDGE 0
#define MIN_WIDTH 900
#define MAX_WIDTH 2100

typedef struct {
   uint8_t prevEdge;
   unsigned long risingTime;
   unsigned long fallingTime;
   uint16_t width;
} ReceiverDatas;
 
volatile static ReceiverDatas Receiver[NUMBER_OF_CHANELS];
volatile static uint8_t prev_pink=0;
   
static void ISR_ReceiverPinsChange()  //this ISR is called every time that a change on a receiver pin is detected
{
   static int now=0;
   now=micros();
   uint8_t act_pink = PINK;
   uint8_t changed = prev_pink ^ act_pink;
   for (int i=0; i<NUMBER_OF_CHANELS; i++) //for all bits
   {
       if((0x1<<i) & changed) //if bit has changed
       {
          if ((0x1 << i) & act_pink) // if bit i is 1
          {
            Receiver[i].risingTime = now;
          }
          else
          {
            Receiver[i].fallingTime = now;
            int width = now - Receiver[i].risingTime;
            if ((width <= MAX_WIDTH) || (width >=MIN_WIDTH))
            {
              Receiver[i].width=width;
            }
            
          }
       }
   }
   prev_pink = act_pink;
}





void init_Receiver()
{
  DDRK = 0b00000000; // port K, input 
  PORTK = 0; //No pull up on port K
  EICRA|=0b00010000; //interrupt on changing state ISC20=1
  EICRA&=0b11011111; //interrupt on changing state ISC21=0
  PCMSK2 = 0b01111111; //Pin PK0:PK6 trigger interrupt
  PCICR |= 0b00000100; //enable interrupts 2. 
  prev_pink=0;
  for (int i=0; i<NUMBER_OF_CHANELS; i++)
  {
    Receiver[i].prevEdge = FALLING_EDGE;
    Receiver[i].width = 0;
    Receiver[i].risingTime = 0;
    Receiver[i].fallingTime=0; 
  }
}

void setup()
{       
  Serial.begin(115200);  
  init_Receiver();
}

int now2=0;
int sensorPreviousTime=0;
void loop()
{
    now2=millis();
    if(now2-sensorPreviousTime>=10) // 25Hz sampling raw values
    {
        Serial.print(Receiver[0].width);
        Serial.print("\t");
        Serial.print(Receiver[1].width);
        Serial.print("\t");
        Serial.print(Receiver[2].width);
        Serial.print("\t");
        Serial.print(Receiver[3].width);
        Serial.print("\t");
        Serial.print(Receiver[4].width);
        Serial.print("\t");
        Serial.print(Receiver[5].width);
        Serial.print("\t");
        Serial.println(Receiver[6].width);
        sensorPreviousTime=now2;
    }
}

 

SIGNAL(PCINT2_vect) //Interrupt, so we call the appropriate function.
{
  ISR_ReceiverPinsChange();
}
