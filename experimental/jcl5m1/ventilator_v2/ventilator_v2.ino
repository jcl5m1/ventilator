#include <Wire.h>

#include "defines.h"
#include "lcd_menu.h"
#include "i2c_scanner.h"
#include "sensirion3200.h"
#include "ms5611_pressure.h"

long volume_est = 0;
int flow = 0;
float pressure = 0;
void setup(){

  //boot up text over serial
  Serial.begin(BAUD);
//  Serial.print("Low-Cost Ventilator.  Version: ");
//  Serial.println(VERSION);

  // includes i2c wire initialization
//  Serial.println("init lcd menu...");
//  init_lcd_menu();
  //jump cursor to debug position
//  selected_idx = DEBUG_IDX;
  
  
  Wire.begin();

  //disable internal pullups to support 3.3v I2C
  pinMode(A4, INPUT);
  pinMode(A5, INPUT); 
  
  Serial.println("init flow sensor...");
  init_flow_sensor();

  Serial.println("init pressure sensor...");
  init_pressure_sensor();

  Serial.println("init complete!");
}

void loop() 
{
  flow = read_flow();
  volume_est += flow;
  pressure = read_pressure();
  Serial.print(millis()/1000.0);
  Serial.print(',');
  Serial.print(volume_est);
  Serial.print(',');
  Serial.println(pressure);
  
//  process_input();
//  update_display();
}
