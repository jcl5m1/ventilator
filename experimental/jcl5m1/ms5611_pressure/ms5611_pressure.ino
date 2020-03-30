#include <Wire.h>
#include <MS5611.h>

MS5611 sensor(&Wire);
float sensor_zero = 0;
#define ZERO_SAMPLES 100
#define BUTTON_PIN 4

#define mbar_to_cmh2o 1.01972

void setup() {
  Serial.begin(9600);
  sensor.setI2Caddr(0x77);
  pinMode(BUTTON_PIN,INPUT_PULLUP);

  bool not_connected = true;

  while(sensor.connect()>0) {
    Serial.println("Error connecting ms5611");
    delay(500);
  }
  zero_sensor();
}

// get local ambient pressure
void zero_sensor(){
  sensor_zero = 0;
  sensor.ReadProm();

  Serial.println("zeroing....");
  for(int i = 0; i < ZERO_SAMPLES;i++){
    sensor.Readout();
    sensor_zero += sensor.GetPres();
  }
  sensor_zero /= ZERO_SAMPLES;    
}

void loop() {
  if(!digitalRead(BUTTON_PIN))
    zero_sensor();
  sensor.Readout();
//  Serial.print(sensor.GetTemp()/100.0,2);
  float relative_press = sensor.GetPres()-sensor_zero;
  Serial.println(mbar_to_cmh2o*relative_press/100.0);
}
