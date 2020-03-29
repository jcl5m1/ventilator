#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define VERSION "20200328.1"
#define BUTTON_PIN 4
#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3

// settings management
char* value_names[] = {"Rate/min", "IPAP(cm)","PEEP(cm)","TdV(mL)","Runtime", "Version", "Dbg"};
volatile int values[] = {14,10,5,25,0,0,0};
int values_max[] = {30,45,45,80,0,0,255};
int measured_values[] = {0,0,0,0,0,0,0};
#define MAX_VALUE_IDX 7
#define DISPLAY_ROWS 4
volatile int selected_idx = 0;
volatile int display_page_idx = 0;
volatile int input_mode = 0;

// input handling
volatile bool button_state_prev = 0;
volatile bool button_state = 0;
volatile int enc_state = 0;

//display variables
char msg[20];

#define BAR_CHART_LENGTH 4
//single thick middle line
const byte bar0Char[] = {0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00};
const byte bar1Char[] = {0x10,0x10,0x10,0x1F,0x10,0x10,0x10,0x00};
const byte bar2Char[] = {0x18,0x18,0x18,0x1F,0x18,0x18,0x18,0x00};
const byte bar3Char[] = {0x1C,0x1C,0x1C,0x1F,0x1C,0x1C,0x1C,0x00};
const byte bar4Char[] = {0x1E,0x1E,0x1E,0x1F,0x1E,0x1E,0x1E,0x00};
const byte bar5Char[] = {0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x00};

const byte editArrow[] = {
  0x1F,
  0x1B,
  0x1D,
  0x00,
  0x1D,
  0x1B,
  0x1F,
  0x00
};

const byte editArrow2[] = {
  0x18,
  0x0C,
  0x06,
  0x1F,
  0x1F,
  0x06,
  0x0C,
  0x18
};

int debug = 0;

void setup()
{
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight 

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);

  lcd.createChar(0, bar0Char);
  lcd.createChar(1, bar1Char);
  lcd.createChar(2, bar2Char);
  lcd.createChar(3, bar3Char);
  lcd.createChar(4, bar4Char);
  lcd.createChar(5, bar5Char);
  lcd.createChar(6, editArrow);
  lcd.createChar(7, editArrow2);
  lcd.home();
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), isrEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), isrEncoderB, CHANGE);
}

void isrEncoderA() {
  // manage quadrature state.  only index value if quadrature cycle completes
  if(bitRead(PIND,ENCODER_A_PIN)){
    if(enc_state == 1)
      enc_state = 0;
    if(enc_state == -2)
      enc_state = -1;
    if(enc_state == 2)
      enc_state = 3;
    if(enc_state == -3){
      // cycle completed, decrease value
      encoder_adjust(-1);
      enc_state = 0;
    }
  } else {
    if(enc_state == 0)
      enc_state = 1;
    if(enc_state == -1)
      enc_state = -2;
    if(enc_state == 3)
      enc_state = 2;
  }
}

void isrEncoderB() {
  // manage quadrature state.  only index value if quadrature cycle completes
  if(bitRead(PIND,ENCODER_B_PIN)){
    if(enc_state == -1)
      enc_state = 0;
    if(enc_state == 2)
      enc_state = 1;
    if(enc_state == -2)
      enc_state = -3;
    if(enc_state == 3){ 
      // cycle completed, increase value
      encoder_adjust(1);
      enc_state = 0;
    }
  } else {
    if(enc_state == 0)
      enc_state = -1;
    if(enc_state == 1)
      enc_state = 2;
    if(enc_state == -3)
      enc_state = -2;
  }
}

void encoder_adjust(int inc){

  if(input_mode == 0){
    //select menu item
    selected_idx += inc;
    if(selected_idx < 0)
      selected_idx = 0;
    if(selected_idx >= MAX_VALUE_IDX)
      selected_idx = MAX_VALUE_IDX-1;
  } else {
    //edit value
    if(inc > 0){
      //change value
      if(values[selected_idx] < values_max[selected_idx]){
        values[selected_idx] += 1;
      }
      // if PEEP is greater than IPAP, raise IPAP
      if(values[1] < values[2]){
        values[1] = values[2];
      }
    }else {
          if(values[selected_idx] > 0){
            values[selected_idx] -= 1;
          }
          // if IPAP is lower than PEEP, lower PEEP
          if(values[1] < values[2]){
            values[2] = values[1];
          }
    }  
  }  
}

void process_input(){

  //check if display should be cleared
  int prev = display_page_idx;  
  display_page_idx = int(selected_idx/DISPLAY_ROWS);
  if(display_page_idx != prev){
    lcd.clear(); // clearing every frame causes dim text and flickering
  }
  
  //process button press
  button_state = bitRead(PIND,BUTTON_PIN);  
  if(button_state_prev && !button_state) {
    if(input_mode == 0)
      input_mode = 1;
    else
      input_mode = 0;
  }
  button_state_prev = button_state;
}

void bar_chart(int c, int r, int seg, int v, int max_v){ 
  if(max_v == 0)
    return;
  if(v < 0)
    v = 0;
  if(v > max_v)
    v = max_v;
    
  int segments = (seg*v)/max_v;
  for(int i = c; i < c+seg; i++){
    lcd.setCursor (i, r);
    if(i < c+segments)
      lcd.write(5);
    else if(i == c+segments)
      lcd.write((5*seg*v/max_v)%5);
    else
      lcd.write(0);
  }
}

void display_runtime(int c, int r){
  // running time
  lcd.setCursor ( c, r );
  unsigned long seconds = millis()/1000;
  unsigned long minutes = seconds/60;
  unsigned long hours = minutes/60;
  unsigned long days = hours/24;
  sprintf(msg, "%02lud%02luh%02lum%02lu", days, hours%24, minutes%60, seconds%60);
  lcd.print(msg);  
}

void update_display(){
  for(int i = 0; i < DISPLAY_ROWS; i++){
    int idx = display_page_idx*DISPLAY_ROWS + i;
    lcd.setCursor ( 0, i );

    if(idx >= MAX_VALUE_IDX) {
      break;
    }
    if(input_mode == 0){
      if(idx == selected_idx){
        if(input_mode == 0)
          lcd.write(0x7E);  //right arrow
      } else {
        lcd.print(' ');
      }
    } else {
      if(idx == selected_idx){
        if((millis()/500) % 2)
          lcd.write(6); //editArrow
        else
          lcd.write(0x7E); //right arrow
      } else {
        // blank all other lines when editting
//        lcd.print("                    ");
//        continue;
      }
    }
  
    lcd.setCursor ( 1, i );
    if(idx == 3) { //tidal volume is 3 digits incrementing in units of 10mL
      sprintf(msg, "%s:%3d%c%3d", value_names[idx], 10*values[idx], ' ', 10*measured_values[idx]);
      lcd.print(msg);
      bar_chart(16,i,BAR_CHART_LENGTH, values[idx], values_max[idx]);
    } else if(idx == 4) { // runtime
      sprintf(msg, "%s:", value_names[idx]);    
      lcd.print(msg);
      display_runtime(9, i);
    } else if(idx == 5) { // version
      sprintf(msg, "%s: %s", value_names[idx],VERSION);
      lcd.print(msg);
    } else if(idx == 6) { // debug *************************************************************************
      int v = values[idx];
      sprintf(msg, "%s:%03d", value_names[idx], v);
      lcd.print(msg);
      bar_chart(8,i,12, v, 255);
    } else {
      sprintf(msg, "%s:%2d%c %2d", value_names[idx], values[idx], ' ', measured_values[idx]);
      lcd.print(msg);
      bar_chart(16,i,BAR_CHART_LENGTH, values[idx], values_max[idx]);
    }

  }    
}

void loop() 
{
  process_input();
  update_display();
}
