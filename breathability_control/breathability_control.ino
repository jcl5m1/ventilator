#include <Servo.h>
#include <Wire.h>
#include <MS5611.h>
#include "sensirion3200.h"

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

char msg[20];
char str_tmp[20];

#define BUTTON_PIN 4
#define BLOWER_PIN 5

int target_speed = 0;
int target_flow = 0;
int state = 0;
int prev_button = 1;
int flow = 0;
int flow_smooth = 0;
int flow_smooth_prev = 0;
int delta_smooth = 0;
Servo myservo;  
#define max_speed 18000
#define min_speed 0
#define led_pin 13
#define FLOW_CONVERSION 115

MS5611 pressure_sensor(&Wire);
float pressure_sensor_zero = 0;
float pressure_smooth = 0;
#define PRESSURE_ZERO_SAMPLES 100
#define mbar_to_cmh2o 1.01972

// get local ambient pressure
void zero_sensor(){
  pressure_sensor_zero = 0;
  delay(50);
  pressure_sensor.ReadProm();
  for(int i = 0; i < PRESSURE_ZERO_SAMPLES;i++){
    pressure_sensor.Readout();
    pressure_sensor_zero += pressure_sensor.GetPres();
  }
  pressure_sensor_zero /= (PRESSURE_ZERO_SAMPLES);    
}

// flow table - open air, aprox conversion  115
// 0 - 0 l/m
// 3771 - 34 l/m  * extrapolated
// 7100 - 64 l/m
// 10600 - 90 l/m
// 13800 - 116 l/m
// 16800 - 141 l/m
// 19600 - 165 l/m

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Blower Control v0.2");
  
  sprintf(msg, "Init LCD:");
  Serial.print(msg);
  
  
  lcd.init();  //initialize the lcd

//  Wire.begin();
  //disable internal pullups to support 3.3v I2C
  pinMode(A4, INPUT);
  pinMode(A5, INPUT); 
  lcd.backlight();
  lcd.clear();

  Serial.println("OK");
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);



  lcd.setCursor ( 0, 0 );
  sprintf(msg, "Init flow sensor:");
  lcd.print(msg);
  Serial.print(msg);
  init_flow_sensor();
  sprintf(msg, "OK");
  lcd.print(msg);
  Serial.println(msg);


  lcd.setCursor ( 0, 1 );
  sprintf(msg, "Init press sensor:");
  lcd.print(msg);
  Serial.print(msg);
  pressure_sensor.setI2Caddr(0x77);
  while(pressure_sensor.connect()>0) {
    sprintf(msg, "ERR");
    lcd.print(msg);
    Serial.println(msg);
    delay(500);
  }
  
  zero_sensor();
  sprintf(msg, "OK");
  lcd.print(msg);
  Serial.println(msg);    

  myservo.attach(BLOWER_PIN,1000,2000); // some motors need min/max setting
  lcd.setCursor ( 0, 2);
  sprintf(msg, "Init blower:");
  lcd.print(msg);
  Serial.print(msg);
  
  myservo.write(max_speed);
  delay(3000); // wait for esc to boot, and sample maximum
  myservo.write(min_speed);
  digitalWrite(led_pin, LOW);
  delay(2000);  // wait for esc to sample minimum

  digitalWrite(led_pin, HIGH);
  myservo.write(min_speed);
  sprintf(msg, "OK");
  lcd.print(msg);
  Serial.println(msg);
  
  delay(1000);
  lcd.clear();

}

void loop() {
 
  flow = read_flow();
  flow_smooth = 0.9*flow_smooth + 0.1*flow;

  pressure_sensor.Readout();
  float relative_pressure = mbar_to_cmh2o*(pressure_sensor.GetPres()-pressure_sensor_zero);
  pressure_smooth = 0.9*pressure_smooth + 0.1*relative_pressure;
  // put your main code here, to run repeatedly:
  int button = digitalRead(BUTTON_PIN);
    
  if(!button && prev_button){
    state += 1;
    if(state == 3){
      state = 0;
    }    
  }
  prev_button = button;

  if(state==0){
    target_flow = 0;
    target_speed = 0;
  }
  else if(state == 1)
    target_flow = 34*FLOW_CONVERSION;  // 34 L/m
  else if(state == 2)
    target_flow = 90*FLOW_CONVERSION; //90 L/m

  int p = target_flow - flow_smooth;
  int d = flow_smooth - flow_smooth_prev;
  delta_smooth = 0.9*delta_smooth + 0.1*d;
  target_speed += p/10-d;

  flow_smooth_prev = flow_smooth;
 
  if(target_speed > max_speed)
    target_speed = max_speed;
  if(target_speed < min_speed)
    target_speed = min_speed;
  myservo.write(target_speed/100);

  lcd.setCursor ( 0, 0 );
  sprintf(msg, "Target Flow: %3d L/m", target_flow/FLOW_CONVERSION);  
  lcd.print(msg);
  lcd.setCursor ( 0, 1 );
  sprintf(msg, "Meas.  Flow: %3d L/m", flow_smooth/FLOW_CONVERSION);  
  lcd.print(msg);
  lcd.setCursor ( 0, 2 );
  dtostrf(pressure_smooth/100, 4, 1, str_tmp);
  sprintf(msg, "Press: %s cmH2O ", str_tmp);
//  sprintf(msg, "Blower Speed: %3d%%", target_speed*100/max_speed);
  lcd.print(msg);
  lcd.setCursor ( 0, 3 );
  if(target_speed == max_speed) {
    sprintf(msg, "Status: Max Speed  ");      
  }else {
    if((abs(p) > 2*FLOW_CONVERSION) || abs(delta_smooth) > 10)
      sprintf(msg, "Status: Adjusting ");  
    else
      sprintf(msg, "Status: Ready     ");  
  }
  lcd.print(msg);

 
  Serial.print(target_flow);
  Serial.print(',');
  Serial.print(flow_smooth);
  Serial.print(',');
  Serial.print(relative_pressure/100);
  Serial.print(',');
  Serial.println(target_speed*100/max_speed);
  delay(0);
}
