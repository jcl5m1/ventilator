#include <Wire.h>
#include <MS5611.h>

MS5611 sensor(&Wire);
MS5611 sensor2(&Wire);
float sensor1_zero = 0;
float sensor2_zero = 0;
#define ZERO_SAMPLES 30

#define BUTTON_PIN 2
#define FLOW_PIN A0
int button_state = 0;
int button_state_prev = 0;


float volume_estimate = 0;

void setup() {

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(FLOW_PIN, INPUT);
  Serial.begin(57600);
  sensor.setI2Caddr(0x76);
  sensor.setI2Caddr(0x77);

  bool not_connected = true;

  while(not_connected){
    not_connected = false;

    if(sensor.connect()>0) {
      Serial.println("Error connecting sensor 1");
      not_connected = true;
    }

    if(sensor2.connect()>0) {
      Serial.println("Error connecting sensor 2");
      not_connected = true;
    }
    if(not_connected)
      delay(500);
  }

  zero_sensors();
}


void zero_sensors(){
  sensor1_zero = 0;
  sensor2_zero = 0;
  sensor.ReadProm();
  sensor2.ReadProm();

  Serial.println("zeroing....");
  for(int i = 0; i < ZERO_SAMPLES;i++){
    sensor.Readout();
    sensor2.Readout();
    sensor1_zero += sensor.GetPres();
    sensor2_zero += sensor2.GetPres();
  }

  sensor1_zero /= ZERO_SAMPLES;
  sensor2_zero /= ZERO_SAMPLES;
    
}

void loop() {

  button_state = digitalRead(BUTTON_PIN);
  if(button_state == 0){
    if(button_state_prev == 1)
//      zero_sensors();    
      volume_estimate = 0;
  }

  button_state_prev = button_state;
//  Serial.println(analogRead(FLOW_PIN));
//  delay(10);
//  scan_all_addresses();
  sensor.Readout();
  sensor2.Readout();
////  Serial.print(sensor.GetTemp()/100.0,2);
////  Serial.print(',');
  float p1 = sensor.GetPres();//-sensor1_zero;
  float p2 = sensor2.GetPres();//-sensor2_zero;
  Serial.print(p1);
  Serial.print(',');
  Serial.println(p2);

//  volume_estimate += p1 - p2;
//  Serial.print(p1*0.0101972,1); //convert Pa to cmh2o
//  Serial.print(',');
//  Serial.println(volume_estimate/1000);
}

void test_crc() {
  sensor.ReadProm();
  sensor.Readout(); 
  Serial.print("CRC=0x");
  Serial.print(sensor.Calc_CRC4(), HEX);
  Serial.print(" (should be 0x");
  Serial.print(sensor.Read_CRC4(), HEX);
  Serial.print(")\n");
  Serial.print("Test Code CRC=0x");
  Serial.print(sensor.CRCcodeTest(), HEX);
  Serial.println(" (should be 0xB)");
}

void scan_all_addresses()
{
  byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
  delay(5000);           // wait 5 seconds for next scan
}
