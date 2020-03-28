// originally written by Johnny Lee <jcl5m1+ventilator@gmail.com>

#include <Servo.h>

Servo myservo;  

//some variables to tweek
#define version "20200316.2"
#define rate 16 // breathing cycles per minute
#define enable_motor true // useful for debugging without noise

#define max_speed 180
#define min_speed 0
#define PEEP_speed 40 //approx 5cm/H2O
#define led_pin 13
#define button_pin A5
#define current_pin A0
#define servo_pin 3
#define serial_baud 9600

int buttonState = 1;
int buttonStatePrev = 1;
int speed_state = 0;
int loop_count = 0;
int click_loop_count = 0;
int click_count = 0;

int target_speed_high = 0;
int target_speed_low = 0;

int cycle_counter = 0;
int cycle_phase = 0;
int mode = 0;
int current = 0;


void setup() {

  Serial.begin(serial_baud);
  Serial.print("Version:\t");
  Serial.println(version);

  pinMode(led_pin, OUTPUT);
  pinMode(current_pin, INPUT);
  analogReference(INTERNAL); // sets reference to 1.1v
  pinMode(button_pin, INPUT_PULLUP);
  digitalWrite(led_pin, HIGH);

  if(enable_motor){
    myservo.attach(servo_pin,1000,2000); // some motors need min/max setting
    
    Serial.print("Initializing ESC...");
    
    myservo.write(max_speed);
    delay(3000); // wait for esc to boot, and sample maximum
    myservo.write(min_speed);
    digitalWrite(led_pin, LOW);
    delay(2000);  // wait for esc to sample minimum

    digitalWrite(led_pin, HIGH);
    myservo.write(PEEP_speed);
    Serial.println("Done");  
  } else {
    Serial.println("Motor disabled.  Bypassing initialization");      
  }
}

void loop() {

  buttonStatePrev = buttonState;
  buttonState = digitalRead(button_pin);

  // button down event, accumulate number of clicks and time of first click
  if(buttonState == 0 and buttonStatePrev == 1){
    click_loop_count = loop_count;
    click_count += 1;
  }

  if(click_count > 0){
    // past click count timeout
    if(click_loop_count + 30 < loop_count){
      if(click_count == 1){ // single click
        speed_state = (speed_state + 1)%5;
        if(mode == 0){
          cycle_counter = 10000; // if in CPAP mode update immediately
        }
      }
      if(click_count == 2){ // double click
        mode = (mode +1)%2;
        // if entering PEEP mode, update immediately in exhale cycle
        // otherwise, maintain phase
        cycle_phase = 0;
        cycle_counter = 10000;
      }
      // blink LED
      digitalWrite(led_pin, LOW);
      delay(50);  
      digitalWrite(led_pin, HIGH);

      // print state
      Serial.print(loop_count/100.0);
      if(mode == 0){
        Serial.print("\tset to CPAP level: ");
        Serial.println(speed_state);
      }
      if(mode == 1){
        Serial.print("\tset to PEEP level: ");
        Serial.print(speed_state);
        Serial.print("\tforced rate: ");
        Serial.print(rate);
        Serial.println("/min");
      }
      click_count = 0;
    }
  }

  // approximate pressure mapping fro original blower. Other blowers will vary
  // 40 ~= 5cm/H2O
  // 60 ~= 10cm/H2O
  // 90 ~= 18cm/H2O
  // 120 ~= 32cm/H2O
  
  if (speed_state == 0) {
    target_speed_high = 60;
    target_speed_low = PEEP_speed;
  }
  if (speed_state == 1) {
    target_speed_high = 70;
    target_speed_low = PEEP_speed;
  }
  if (speed_state == 2) {
    target_speed_high =  80;
    target_speed_low =  PEEP_speed;
  }
  if (speed_state == 3) {
    target_speed_high =  90;
    target_speed_low =  PEEP_speed;
  }
  if (speed_state == 4) {
    target_speed_high =  100;
    target_speed_low =  PEEP_speed;
  }
  
  //CPAP mode, both speed are the same
  if(mode == 0)
    target_speed_low = target_speed_high;
    
  // handle breath in/out cycle at target rate/min
  cycle_counter += 1;
  if( (30*100)/rate < cycle_counter){
    cycle_phase = (cycle_phase+1)%2;
    cycle_counter = 0;
    Serial.print(loop_count/100.0);
    Serial.print("\tphase speed:");
    if(cycle_phase == 0){
     if(enable_motor)
        myservo.write(target_speed_high);
      Serial.println(target_speed_high);
    } else {
     if(enable_motor)
        myservo.write(target_speed_low);
      Serial.println(target_speed_low);
    }
  }

  // for debugging breathing back pressure sensing
//  current = analogRead(current_pin);
//  Serial.print(current);
//  Serial.print(",");
//  Serial.println(digitalRead(button_pin));
  
  loop_count += 1;
  delay(10);  // approximately 100 cycles per second
}
