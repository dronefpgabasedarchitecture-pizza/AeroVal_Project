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

#define LOW_VOLTAGE_LIMIT 11000
#define EMERGENCY_VOLTAGE_LIMIT 9900

void batteryCtrl_init();
void batteryCtrl_update();
int voltage;
int lowVoltageState;
int emergencyVoltageState;

setup()
{
  Serial.begin(9600);
  
  batteryCtrl_init();
}

loop()
{
 batteryCtrl_update();
 Serial.print("voltage: ");
 Serial.println(voltage);
 
 Serial.print("lov voltage: ");
 
 delay(100); 
}



int 

void batteryCtrl_init()
{
  pinMode(0, INPUT);
  voltage=0;
  lowVoltageState=0;
  emergencyVoltageState=0;
  
}
void batteryCtrl_update()
{
  
}
