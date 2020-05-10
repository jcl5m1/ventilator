#include <Wire.h>

#define SENSIRION_ADDR 0x40

void init_flow_sensor(){
  // init continuous flow reading for Sensirion 3200
  // https://14core.com/wp-content/uploads/2017/12/Sensirion-Mass-Flow-Meters-SFM3000-I2C-Functional-Description.pdf
  Wire.beginTransmission(SENSIRION_ADDR); // transmit to device
  Wire.write(0x10);        // request serial
  Wire.write(0x00);        // request serial
  Wire.endTransmission();    // stop transmitting
}

int read_flow(){  
  // read flow  for Sensirion 3200
  // https://14core.com/wp-content/uploads/2017/12/Sensirion-Mass-Flow-Meters-SFM3000-I2C-Functional-Description.pdf
  Wire.requestFrom(SENSIRION_ADDR, 3);
  while(!Wire.available()){}  
  byte a = Wire.read();
  byte b = Wire.read();
  byte crc = Wire.read();  //TODO use CRC
  return ((a<<8) + b)-32768;
}
