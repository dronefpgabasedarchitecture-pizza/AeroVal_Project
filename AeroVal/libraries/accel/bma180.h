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


/*
 TODO:
 Change scale factor to avoid dividing
 Axis & offset orientation?
 */

#ifndef BMA180_H_
#define BMA_180_

#include <Wire.h>

//Define the addresses
#define BMA180_ADDRESS 0x40
#define BMA180_ADDRESS_CHIP_ID 0x0
#define BMA180_ADDRESS_XAXIS_LSB_VAL 0x02
#define BMA180_ADDRESS_RANGE 0x35
#define BMA180_ADDRESS_EE_W 0x0D
#define BMA180_ADDRESS_BW 0x20
#define BMA180_ADDRESS_SOFT_RESET 0x10

//Define the normal values
#define BMA180_VAL_CHIP_ID 0x03
//#define BMA180_SCALE_FACTOR 204 //Need to check. 2000/9.81
#define BMA180_RESET 0xB6

//prototypes
void accel_init_EEPROM(void); //Unused yet
void accel_init(void);
void accel_calibrate(void);
void accel_readRaw(void);
void accel_readAccelAverageMeterPerSecPerSec(void);


//functions

void accel_init(void)
{
    i2c_write8(BMA180_ADDRESS, BMA180_ADDRESS_SOFT_RESET, BMA180_RESET);
    delay(10);
    
    // Let's check if it's really BMA 180 there.
    if (i2c_readWho(BMA180_ADDRESS) == BMA180_VAL_CHIP_ID){
        sensorsDetected |=0b00000010; //Adjust the variable to indicate that BMA180 is here.
        Serial.println("accel's here :)");
        
        //Let's change the range to +-4G
        byte range = i2c_read8(BMA180_ADDRESS, BMA180_ADDRESS_RANGE); //get the range (+others) bits
        range = range & 0b11110001; //mask to change only the range bits
        range = range | 0b00001000; //mask to change only the range bits
        i2c_write8(BMA180_ADDRESS, BMA180_ADDRESS_EE_W, 0x10); //enable the ee_w register to be able to update registers
        i2c_write8(BMA180_ADDRESS, BMA180_ADDRESS_RANGE, range); //upload range
        
        // Let's setup the low pass filter (1200Hz)
        byte bw = i2c_read8(BMA180_ADDRESS, BMA180_ADDRESS_BW);
        bw = bw & 0b11110000; //Mask to change the 4 LSB bits
        bw = bw | 0b00000111; //Mask to change the 4 LSB bits
        i2c_write8(BMA180_ADDRESS, BMA180_ADDRESS_BW, bw); //We write the modified byte        
    }
}

//Calibration procedure: the sensor must remain leveled in a 1G field
//We should consider to write in EEPROM instead of an offset variable array.
void accel_calibrate()
{
    float Xlow=0, X0=0, Xhigh=0, Ylow=0, Y0=0, Yhigh=0, Zlow=0, Z0=0, Zhigh=0, useless=0;
    Serial.println("Waiting 10");
    delay(5000);
    Serial.println("Waiting 5");
    delay(5000);
    
    Serial.println("level");
    delay(10000);
    for (int i=0; i<50; i++)
    {
        i2c_writeByte(BMA180_ADDRESS, BMA180_ADDRESS_XAXIS_LSB_VAL);//Send the address of the byte(s) we want
        Wire.requestFrom(BMA180_ADDRESS, 6); //request 6 bytes
        
        Y0+=0.02*((Wire.read() | (Wire.read()<<8))>>2);
        X0+=0.02*((Wire.read() | (Wire.read()<<8))>>2);// invert axis
        Zlow+=0.02*((Wire.read() | (Wire.read()<<8))>>2);
        delay(20);
    }
    
    
    Serial.println("upside down");
    delay(10000);
    for (int i=0; i<50; i++)
    {
        i2c_writeByte(BMA180_ADDRESS, BMA180_ADDRESS_XAXIS_LSB_VAL);//Send the address of the byte(s) we want
        Wire.requestFrom(BMA180_ADDRESS, 6); //request 6 bytes
        
        useless=((Wire.read() | (Wire.read()<<8))>>2);
        useless=((Wire.read() | (Wire.read()<<8))>>2);// invert axis
        Zhigh+=0.02*((Wire.read() | (Wire.read()<<8))>>2);
        delay(20);
    }
    
    Serial.println("nose down");
    delay(10000);
    for (int i=0; i<50; i++)
    {
        i2c_writeByte(BMA180_ADDRESS, BMA180_ADDRESS_XAXIS_LSB_VAL);//Send the address of the byte(s) we want
        Wire.requestFrom(BMA180_ADDRESS, 6); //request 6 bytes
        
        useless=((Wire.read() | (Wire.read()<<8))>>2);
        Xhigh+=0.02*((Wire.read() | (Wire.read()<<8))>>2);// invert axis
        Z0+=0.02*((Wire.read() | (Wire.read()<<8))>>2);
        delay(20);
    }
    
    Serial.println("nose up");
    delay(10000);
    for (int i=0; i<50; i++)
    {
        i2c_writeByte(BMA180_ADDRESS, BMA180_ADDRESS_XAXIS_LSB_VAL);//Send the address of the byte(s) we want
        Wire.requestFrom(BMA180_ADDRESS, 6); //request 6 bytes
        
        useless=((Wire.read() | (Wire.read()<<8))>>2);
        Xlow+=0.02*((Wire.read() | (Wire.read()<<8))>>2);// invert axis
        useless=((Wire.read() | (Wire.read()<<8))>>2);
        delay(20);
    }
    
    Serial.println("left up");
    delay(10000);
    for (int i=0; i<50; i++)
    {
        i2c_writeByte(BMA180_ADDRESS, BMA180_ADDRESS_XAXIS_LSB_VAL);//Send the address of the byte(s) we want
        Wire.requestFrom(BMA180_ADDRESS, 6); //request 6 bytes
        
        Ylow+=0.02*((Wire.read() | (Wire.read()<<8))>>2);
        useless=((Wire.read() | (Wire.read()<<8))>>2);
        useless=((Wire.read() | (Wire.read()<<8))>>2);
        delay(20);
    }
    
    Serial.println("left down");
    delay(10000);
    for (int i=0; i<50; i++)
    {
        i2c_writeByte(BMA180_ADDRESS, BMA180_ADDRESS_XAXIS_LSB_VAL);//Send the address of the byte(s) we want
        Wire.requestFrom(BMA180_ADDRESS, 6); //request 6 bytes
        
        Yhigh+=0.02*((Wire.read() | (Wire.read()<<8))>>2);
        useless=((Wire.read() | (Wire.read()<<8))>>2);
        useless=((Wire.read() | (Wire.read()<<8))>>2);
        delay(20);
    }
    
    Serial.println("End of measurements");
    
    accel_scaling[XAXIS]=(Xhigh-Xlow)/19.62;
    accel_scaling[YAXIS]=(Yhigh-Ylow)/19.62;
    accel_scaling[ZAXIS]=(Zhigh-Zlow)/19.62;
    
    accel_offset[XAXIS]=-X0/accel_scaling[XAXIS];
    accel_offset[YAXIS]=-Y0/accel_scaling[YAXIS];
    accel_offset[ZAXIS]=-Z0/accel_scaling[ZAXIS];
    
    Serial.print("X offset: ");
    Serial.println(accel_offset[XAXIS]);
    
    Serial.print("X scale factor : ");
    Serial.println(accel_scaling[XAXIS]);
    
    Serial.print("Y offset: ");
    Serial.println(accel_offset[YAXIS]);
    
    Serial.print("Y scale factor : ");
    Serial.println(accel_scaling[YAXIS]);
    
    Serial.print("Z offset: ");
    Serial.println(accel_offset[ZAXIS]);
    
    Serial.print("Z scale factor : ");
    Serial.println(accel_scaling[ZAXIS]);
    
    
    
}


void accel_readAccelAverageMeterPerSecPerSec(void) //Divide the raw values by samples to average, reset variables
{
    for(int axis=XAXIS; axis<=ZAXIS; axis++)
    {
        accel_meterPerSecPerSec[axis] = ((accel_rawDataSamplesSum[axis]/accel_samples)/accel_scaling[axis])+accel_offset[axis];
        accel_rawDataSamplesSum[axis]=0;
    }
    accel_samples=0;
}

void accel_readRaw(void)  //Add/imcrement raw values sum from accel for the 3 axis
{
    i2c_writeByte(BMA180_ADDRESS, BMA180_ADDRESS_XAXIS_LSB_VAL);//Send the address of the byte(s) we want
    Wire.requestFrom(BMA180_ADDRESS, 6); //request 6 bytes
    
    accel_rawDataSamplesSum[1]+=((Wire.read() | (Wire.read()<<8))>>2);
    accel_rawDataSamplesSum[0]+=((Wire.read() | (Wire.read()<<8))>>2);
    accel_rawDataSamplesSum[2]+=((Wire.read() | (Wire.read()<<8))>>2);
    accel_samples++;
}



#endif
