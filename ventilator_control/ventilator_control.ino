// originally written by Johnny Lee <jcl5m1@gmail.com>

#include <Servo.h>

Servo myservo;  

//some variables to tweek
#define version "20201603.1"
#define rate 16 // PEEP breathing cycles per minute
#define enable_motor true // useful for debugging without noise

#define max_speed 180
#define min_speed 0
#define start_speed 20
#define ledPin 13
#define buttonPin A5
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

void setup() {

  Serial.begin(serial_baud);
  Serial.print("Version:\t");
  Serial.println(version);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(ledPin, HIGH);

  if(enable_motor){
    myservo.attach(servo_pin,1000,2000); // some motors need min/max setting
    
    Serial.print("Initializing ESC...");
    
    myservo.write(max_speed);
    delay(3000); // wait for esc to boot, and sample maximum
    myservo.write(min_speed);
    digitalWrite(ledPin, LOW);
    delay(2000);  // wait for esc to sample minimum

    digitalWrite(ledPin, HIGH);
    myservo.write(start_speed);
    Serial.println("Done");  
  } else {
    Serial.println("Motor disabled.  Bypassing initialization");      
  }
}

void loop() {

  buttonStatePrev = buttonState;
  buttonState = digitalRead(buttonPin);

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
      digitalWrite(ledPin, LOW);
      delay(50);  
      digitalWrite(ledPin, HIGH);

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
  
  if (speed_state == 0) {
    target_speed_high = 60;
    target_speed_low = 10;
  }
  if (speed_state == 1) {
    target_speed_high = 75;
    target_speed_low = 10;
  }
  if (speed_state == 2) {
    target_speed_high =  90;
    target_speed_low =  10;
  }
  if (speed_state == 3) {
    target_speed_high =  105;
    target_speed_low =  10;
  }
  if (speed_state == 4) {
    target_speed_high =  120;
    target_speed_low =  10;
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

  loop_count += 1;
  delay(10);  // 100 cycles per second
}
