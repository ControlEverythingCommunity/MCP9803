// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// MCP9803
// This code is designed to work with the MCP9803_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Temperature?sku=MCP9803_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// MCP9803 I2C address is 0x48(72)
#define Addr 0x48

double cTemp = 0.0, fTemp = 0.0;
void setup()
{
    // Set variable
    Particle.variable("i2cdevice", "MCP9803");
    Particle.variable("cTemp", cTemp);
    
    // Initialise I2C communication as MASTER
    Wire.begin();
    // Initialise Serial Communication, set baud rate = 9600
    Serial.begin(9600);
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select configuration register
    Wire.write(0x01);
    // Continuous conversion mode, Power-up default
    Wire.write(0x60);
    // Stop I2C Transmission
    Wire.endTransmission();
    delay(300);
}

void loop()
{
    unsigned int data[2];
    
    // Starts I2C communication
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write(0x00);
    // Stop I2C transmission
    Wire.endTransmission();
    
    // Request 2 bytes of data
    Wire.requestFrom(Addr, 2);
    
    // Read 2 bytes of data
    // temp msb, temp lsb
    if(Wire.available() == 2)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();
    }
    
    // Convert the data to 12-bits
    int temp = ((data[0] * 256) + data[1]) / 16.0;
    if(temp > 2047)
    {
        temp -= 4096;
    }
    cTemp = temp * 0.0625;
    fTemp = cTemp * 1.8 + 32;
  
    // Output data to serial monitor
    Particle.publish("Temperature in Celsius : ", String(cTemp));
    Particle.publish("Temperature in Fahrenheit : ", String(fTemp));
    delay(500);
}
