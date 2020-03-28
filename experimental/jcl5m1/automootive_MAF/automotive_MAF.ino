// originally written by Johnny Lee <jcl5m1+ventilator@gmail.com>

//some variables to tweek
#define version "20200322.0"
#define led_pin 13
#define flow_pin A0
#define button_pin 2
#define serial_baud 9600

#define zero_flow_samples 100

int buttonState = 1;
int buttonStatePrev = 1;
int raw_flow = 0;
int loop_count = 0;
float zero_flow = 0;
float flow_estimate = 0;
float volume_estimate = 0;
int loop_counter_start = 0;
void setup() {

  Serial.begin(serial_baud);
  Serial.print("Version:\t");
  Serial.println(version);

  pinMode(led_pin, OUTPUT);
  pinMode(flow_pin, INPUT);
  pinMode(button_pin, INPUT_PULLUP);
  digitalWrite(led_pin, HIGH);

  Serial.print("Sampling zero flow...");

  for(int i =0 ; i < zero_flow_samples; i++){
    zero_flow += analogRead(flow_pin);
  }
  zero_flow /= zero_flow_samples;
  Serial.println("Done");
  
}

void loop() {

  buttonStatePrev = buttonState;
  buttonState = digitalRead(button_pin);
  
  if(buttonState == 0){
    // due to drift, and 1 way flow sensing.
    // on expiration - zero volume estimate, and re-estimate zero_flow
    volume_estimate = 0;
    if(buttonStatePrev == 1){
      Serial.print("exp start");
      loop_counter_start = loop_count;
      zero_flow = 0;
    }
    zero_flow += analogRead(flow_pin);
  } else {
    // on inspiration start, finalize zero_flow estimate
    if(buttonStatePrev == 0){
      Serial.print("insp start");
      zero_flow /= loop_count - loop_counter_start;
    }

    loop_counter_start = 0;
    // for debugging breathing back pressure sensing
    flow_estimate = analogRead(flow_pin) - zero_flow;
    volume_estimate += flow_estimate;
  }

  Serial.print(flow_estimate*20);
  Serial.print(",");
  Serial.println(volume_estimate);
  
  loop_count += 1;
  delay(10);  // approximately 100 cycles per second
}
