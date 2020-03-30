#include <Wire.h>

#include "defines.h"
#include "lcd_menu.h"
#include "i2c_scanner.h"
#include "sensirion3200.h"
#include "ms5611_pressure.h"

float volume_est = 0;
long last_flow_sample_ts = 0;
int flow = 0;
void setup(){

  //boot up text over serial
  Serial.begin(BAUD);
//  Serial.print("Low-Cost Ventilator.  Version: ");
//  Serial.println(VERSION);

  // includes i2c wire initialization
//  Serial.println("init lcd menu...");
  init_lcd_menu();
  //jump cursor to debug position
  selected_idx = DEBUG_IDX;
  
  //Serial.println("init flow sensor...");
  init_flow_sensor();

  //Serial.println("init pressure sensor...");
  init_pressure_sensor();

  //Serial.println("init complete!");
}

void loop() 
{
  if(last_flow_sample_ts > 0){
    int dt = millis()-last_flow_sample_ts;
    flow = read_flow();
//    volume_est += dt*read_flow()/1000.0;    //timestamps are very irregular, makes integration quite bad
    volume_est += flow;
  }
  last_flow_sample_ts = millis();    
  
  double pressure = read_pressure();
  debug = pressure;
//  Serial.print(millis());
//  Serial.print(',');  
  Serial.print(volume_est/1000.0);
  Serial.print(',');
  Serial.println(pressure);
  
//  process_input();
//  update_display();
}
