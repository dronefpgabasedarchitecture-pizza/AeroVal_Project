/*
 TODO:
 init functions should return a value if success
 */

#ifndef ITG3200_H_
#define ITG3200_H_

#define ITG3200_ADDRESS 0x68
#define ITG3200_ADDRESS_FS_FILTER 0x16
#define ITG3200_ADDRESS_SRD 0x15
#define ITG3200_ADDRESS_POWER_MANAGEMENT 0x3E
#define ITG3200_ADDRESS_XAXIS_MSB_VAL 0x1D
#define ITG3200_RESET 0b10000000


//Since the internal frequency is 8kHz we set the SRD to 7, to have 8k/(7+1) = 1k samples per second.
//Should maybe be adjusted later
#define ITG3200_VALUE_SRD 0x7

// float gyro_scaleFactor = radians(4559.0/65536.0); //Value according to the scale factor of the datasheet
float gyro_scaleFactor = radians(1/14.375); //Value according to the datasheet instead of full range (4000dps) and 16bits

//prototypes
void gyro_init_EEPROM(void); //not used yet
void gyro_init(void);
void gyro_calibrate(void);
void gyr_readRaw(void);
void gyro_readAveragedGyroRadPerSec(void);


//functions

void gyro_init(void)
{
    //reset the sensor
    i2c_write8(ITG3200_ADDRESS, ITG3200_ADDRESS_POWER_MANAGEMENT, ITG3200_RESET);
    delay(100); //100ms ??!
    
    // Let's check if the sensor is connected
    if((i2c_readWho(ITG3200_ADDRESS) & 0b01111110)==0b01101000)
    {
        sensorsDetected |= 0b00000001; //sensor detected :)
        Serial.println("gyro's here :)");
        
         // Let's update low pass to 256Hz and full scall
        byte data = i2c_read8(ITG3200_ADDRESS, ITG3200_ADDRESS_FS_FILTER);
        data = data & 0b11100000;
        data = data | 0b00011000;
        i2c_write8(ITG3200_ADDRESS, ITG3200_ADDRESS_FS_FILTER, data);
        
        //Sample rate divider
        i2c_write8(ITG3200_ADDRESS, ITG3200_ADDRESS_SRD, ITG3200_VALUE_SRD);
        
        //Set the "power management" byte to the right value
        delay(500); //Delay to let the sensor power on and become stable.
        i2c_write8(ITG3200_ADDRESS, ITG3200_ADDRESS_POWER_MANAGEMENT, 0x01);//Set the oscillator with X axis reference (1) to improve stability.
    }
}

void gyro_calibrate(void)
{
    for (int i=0; i<50; i++) //average on 50 samples
    {
        i2c_writeByte(ITG3200_ADDRESS, ITG3200_ADDRESS_XAXIS_MSB_VAL);
        Wire.requestFrom(ITG3200_ADDRESS, 6);
        
        //Note that {1; 0; 2} => We invert X and Y axis, according to the datasheet, to have new X-axis = roll
        gyro_offset[1]-= 0.02*((Wire.read() <<8) | Wire.read()); // *0.02 is like divide by 50.
        gyro_offset[0]-= 0.02*((Wire.read() <<8) | Wire.read()); // *0.02 is like divide by 50.
        gyro_offset[2]-= 0.02*((Wire.read() <<8) | Wire.read()); // *0.02 is like divide by 50.
        delay(10);
        
    }
}



void gyro_readGyroAverageRadPerSec(void)
{
    for(int axis=XAXIS; axis <=ZAXIS; axis++)
    {
        gyro_radPerSec[axis]=((gyro_rawDataSamplesSum[axis]/gyro_samples)+gyro_offset[axis])*gyro_scaleFactor;
        gyro_rawDataSamplesSum[axis]=0;
    }
    gyro_samples=0;
    
}

void gyro_readRaw(void)
{
    i2c_writeByte(ITG3200_ADDRESS, ITG3200_ADDRESS_XAXIS_MSB_VAL);
    
    Wire.requestFrom(ITG3200_ADDRESS, 6);
    
    //Note that {1; 0; 2} => We invert X and Y axis, according to the datasheet, to have new X-axis = roll
    gyro_rawDataSamplesSum[0] -= ((Wire.read() << 8) | Wire.read());
    gyro_rawDataSamplesSum[1] -= ((Wire.read() << 8) | Wire.read()); //- invert axis
    gyro_rawDataSamplesSum[2] += ((Wire.read() << 8) | Wire.read());
    gyro_samples++;

}


#endif
