#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define BUTTON_PIN 2
#define ENCODER_A_PIN 3
#define ENCODER_B_PIN 4

char* value_names[] = {"Rate/min", "IPAP(cm)","PEEP(cm)","TdV(mL)"};
volatile int values[] = {16,10,5,30};
int measured_values[] = {0,0,0,0};
int values_max[] = {30,45,30,80};
volatile int value_idx = 0;
volatile int button_count = 0;
volatile unsigned long last_buttonup_isr_ms = 0;
volatile unsigned long last_encoder_isr_ms = 0;
volatile unsigned long now = 0;

#define MAX_VALUE_IDX 4

char msg[16];
byte customChar[] = {
  0x00,
  0x04,
  0x02,
  0x1F,
  0x1F,
  0x02,
  0x04,
  0x00
};

void setup()
{
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight 

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);

  lcd.createChar(0, customChar);
  lcd.home();
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), isrEncoderA, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), isrButton, CHANGE);
}

void isrEncoderA() {
  now = millis();
  if (last_encoder_isr_ms > now - 20)
    return;

  if(bitRead(PIND,ENCODER_B_PIN)) {
    values[value_idx]--;
  } else {
    values[value_idx]++;
  }
  last_encoder_isr_ms = now;
}

void isrButton() {  
  //dleay in ISR is bad, trying to debounce, TODO do this in HW.
  delay(2);
  if(bitRead(PIND,BUTTON_PIN)){
    last_buttonup_isr_ms = millis();
    return;
  }
  if (last_encoder_isr_ms > millis() - 100)
    return;
 
  value_idx = (value_idx + 1) % MAX_VALUE_IDX;
  button_count += 1;
}

void check_limits(){
  for(int i = 0; i < MAX_VALUE_IDX; i++){
    if(values[i] < 0){
      values[i] = 0;
    }
    if(values[i] > values_max[i]){
      values[i] = values_max[i];
    }
  }

  // Limit PEEP to IPAP
  if(values[2] > values[1])
    values[2] = values[1];
}

void update_display(){
  for(int i = 0; i < MAX_VALUE_IDX; i++){
    lcd.setCursor ( 0, i );            // go to the top left corner  
    if(i == value_idx){
      lcd.write(0);
    } else {
      lcd.print(' ');
    }

    lcd.setCursor ( 1, i );
    if(i == 3) //tidal volume is 3 digits incrementing in units of 10mL
      sprintf(msg, "%s:%3d %3d", value_names[i], 10*values[i], 10*measured_values[i]);
    else
      sprintf(msg, "%s:%2d  %2d", value_names[i], values[i], measured_values[i]);
    lcd.print(msg);
  }

  // time running
//  lcd.setCursor ( 12, 0 );
//  long t = millis()/1000;
//  int s = t%60;
//  int m = (s/60)%60;
//  int h = (m/60);
//  sprintf(msg, "%02d:%02d:%02d", h, m, s);
//  lcd.print(msg);
}

void loop() 
{
  check_limits();
  update_display();
}
