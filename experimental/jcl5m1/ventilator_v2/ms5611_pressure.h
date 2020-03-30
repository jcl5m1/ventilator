#include <Wire.h>
#include <MS5611.h>

MS5611 ms5611_sensor(&Wire);
float pressure_sensor_zero = 0;
#define ZERO_SAMPLES 100

#define mbar_to_cmh2o 1.01972
//
//void setup() {
//  Serial.begin(9600);
//  pressure_sensor.setI2Caddr(0x77);
//  pinMode(BUTTON_PIN,INPUT_PULLUP);
//
//  bool not_connected = true;
//
//  while(pressure_sensor.connect()>0) {
//    Serial.println("Error connecting ms5611");
//    delay(500);
//  }
//  zero_pressure_sensor();
//}


// get local ambient pressure
void zero_pressure_sensor(){
  pressure_sensor_zero = 0;
  ms5611_sensor.ReadProm();

  Serial.print("zeroing with ");
  Serial.print(ZERO_SAMPLES);
  Serial.println(" samples...");
  for(int i = 0; i < ZERO_SAMPLES;i++){
    ms5611_sensor.Readout();
    pressure_sensor_zero += ms5611_sensor.GetPres();
  }
  pressure_sensor_zero /= ZERO_SAMPLES;    
}

double read_pressure(){
  ms5611_sensor.Readout();
  float relative_press = ms5611_sensor.GetPres()-pressure_sensor_zero;
  return mbar_to_cmh2o*relative_press/100.0;
}


double read_tempurature(){
  // report temps from last readout update. assumes pressure is being requested regularly
  return ms5611_sensor.GetTemp()/100.0;
}

void init_pressure_sensor(){
  while(ms5611_sensor.connect()>0) {
    Serial.println("Error connecting ms5611");
    delay(500);
  }
  zero_pressure_sensor();
}
