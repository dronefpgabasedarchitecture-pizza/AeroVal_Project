#ifndef I2C_FUNCTIONS_H_
#define I2C_FUNCTIONS_H_

#include <Wire.h>

int i2c_writeByte(byte deviceAddress, byte registerAddress)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.endTransmission();
}


int i2c_write8(byte deviceAddress, byte registerAddress, byte data)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.write(data);
    Wire.endTransmission();
}

byte i2c_read8(byte deviceAddress, byte registerAddress)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.endTransmission();
    Wire.requestFrom((int)deviceAddress,(int) 1);
    byte data = Wire.read();
    return data;
}

byte i2c_read16(byte deviceAddress, byte registerAddress)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(registerAddress);
    Wire.endTransmission();
    Wire.requestFrom((int)deviceAddress,(int) 2);
    byte data = Wire.read()<<8 | Wire.read();
    return data;
}

/*byte i2c_readWho(byte deviceAddress)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write((byte) 0x00);
    Wire.endTransmission();
    Wire.requestFrom((int8_t)deviceAddress, (int8_t) 1);
    return  Wire.read();
}
*/

byte i2c_readWho(int16_t deviceAddress)
{
    Wire.beginTransmission(deviceAddress);
    Wire.write(0x0);
    Wire.endTransmission();
    Wire.requestFrom(deviceAddress, 1);
    return  Wire.read();
}


#endif
