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

#ifndef GLOBALVARIABLES_H_
#define GLOBALVARIABLES_H_

#define XAXIS 0
#define YAXIS 1
#define ZAXIS 2

#define RED_LED 4
#define STATE_LED 13
#define MODE_LED 31

//State Variables
uint8_t armed=0;
uint8_t blink1HZ=0;
uint8_t blink10HZ=0;


//sensorsDetected:
//xxxxxxx1 = Gyro
//xxxxxx1x = Accel
//xxxxx1xx = Mag
//...
unsigned char sensorsDetected=0;

//Accel variables
float accel_meterPerSecPerSec[3];
float accel_rawDataSamplesSum[3];
int accel_samples=0;
float accel_offset[3]={-0.34, 0.42, -0.42};
float accel_scaling[3]={-209.9,211.28,-209.41};



//Gyro Variables
float gyro_radPerSec[3];
float gyro_rawDataSamplesSum[3];
int gyro_samples=0;
float gyro_offset[3]={0, 0, 0};



//Receiver Variables
typedef struct {
    uint8_t prevEdge;
    unsigned long risingTime;
    unsigned long fallingTime;
    uint16_t width;
} ReceiverDatas;

volatile static ReceiverDatas Receiver[7]; //7 = number of channels



//Motors Variables
int motorCommand[8] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};



//Loops related
void do100HzLoop(void);
void do10HzLoop(void);
void do1HzLoop(void);

unsigned int now, sensorPreviousTime, mainLoopPreviousTime;
unsigned char frame_counter=0;



// Visualization related variables
int16_t gyro_scale = 65535.0 / 20.0;
int16_t accel_scale = 65535.0 / 3.0;
int16_t kinematics_scale = 65535.0 / TWO_PI;
byte vBuffer[22];

#endif