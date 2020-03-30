#include <Wire.h>

#define SENSIRION_ADDR 0x40
float volume_estimate = 0;
void setup()
{
  Wire.begin();
  //disable internal pullups to support 3.3v I2C
  pinMode(A4, INPUT);
  pinMode(A5, INPUT); 
  
  Serial.begin(9600);
  Serial.println("\nSensirion3200_i2c reader");
  init_flow_sensor();
}

void init_flow_sensor(){
  // init continuous flow reading for Sensirion 3200
  // https://14core.com/wp-content/uploads/2017/12/Sensirion-Mass-Flow-Meters-SFM3000-I2C-Functional-Description.pdf
  Wire.beginTransmission(SENSIRION_ADDR); // transmit to device
  Wire.write(0x10);        // request serial
  Wire.write(0x00);        // request serial
  Wire.endTransmission();    // stop transmitting
}

int read_flow_sensor(int addr){  
  // read flow  for Sensirion 3200
  // https://14core.com/wp-content/uploads/2017/12/Sensirion-Mass-Flow-Meters-SFM3000-I2C-Functional-Description.pdf
  Wire.requestFrom(addr, 3);
  while(!Wire.available()){}  
  byte a = Wire.read();
  byte b = Wire.read();
  byte crc = Wire.read();
  return ((a<<8) + b)-32768;
}

void loop() {
  int flow = read_flow_sensor(SENSIRION_ADDR);
  volume_estimate += flow;
  Serial.print(flow);
  Serial.print(',');
  Serial.println(volume_estimate);
  delay(10);
}
