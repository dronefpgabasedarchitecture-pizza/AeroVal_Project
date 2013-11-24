#include "Arduino.h"
#include <Wire.h>
#include <globalVariables.h>
#include <i2cFunctions.h>
#include <itg3200.h>
#include <bma180.h>
#include <kinematics.h>
#include <receiver.h>
#include <motors.h>
#define SERIAL_SPEED 115200
    
void setup()
{
    digitalWrite(STATE_LED, LOW);
    
    Wire.begin();
    TWBR = 12; // 400 KHz (maximum supported frequency) for I2C
    
    Serial.begin(SERIAL_SPEED);
    delay(1000);
    
    //call init functions
    gyro_init();
    accel_init();
    receiver_init();
    motors_init();

    
    //show results of initialisation
    
    Serial.print("Sensors detected: ");
    Serial.println(sensorsDetected, BIN);
    
    //calibrate sensors
    gyro_calibrate();
    //accel_calibrate();
    
    
    //show results of calibration
    Serial.println("Accel offset : ");
    Serial.println(accel_offset[0]);
    Serial.println(accel_offset[1]);
    Serial.println(accel_offset[2]);
    // + pente
    
    Serial.println("Gyro offset : ");
    Serial.println(gyro_offset[0]);
    Serial.println(gyro_offset[1]);
    Serial.println(gyro_offset[2]);
    // + pente
    
    
    //setup() completed
    delay(1000);
    for(int i=0; i++; i<=10)
    {
        digitalWrite(STATE_LED, blink10Hz); //ready!
        blink10Hz=!blink10Hz;
    }
    
}

void loop()
{
    now=micros();
    if(now-sensorPreviousTime>=1000) // 1 kHZ sampling raw values
    {
        accel_readRaw();
        gyro_readRaw();
        sensorPreviousTime=now;
    }
    
    
    if(now-mainLoopPreviousTime>=10000)     //100 HZ
    {
      frame_counter++;
      if (frame_counter>=200) //reset frame_counter to avoid overflow
      {
         frame_counter=0; 
      }
        
      do100HzLoop();
      
      if(frame_counter%10==0)//10 Hz
      {
          do10HzLoop();
      }
      
      if(frame_counter%100==0)//1 Hz
      {
          frame_counter=0;
          do1HzLoop();
      }
        mainLoopPreviousTime= now;
    }
}

void do100HzLoop(void)
{
    accel_readAccelAverageMeterPerSecPerSec();
    gyro_readGyroAverageRadPerSec();
    receiver_analyze();

}

void do10HzLoop(void)
{
    if (!armed)
    {
        digitalWrite(RED_LED, LOW);
    }
    else
    {
        digitalWrite(RED_LED, HIGH);
    }
}

void do1HzLoop(void)
{
    digitalWrite(STATE_LED, blink1Hz);
    blink1Hz=!blink1Hz;
}
