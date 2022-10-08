
int ABORT_BTN = 17;
int STOP_BTN = 11;
int START_BTN = 2;
int ABORT_LED = 7;
int READY_LED = 12;

char state;
char ABORT_STATE = 'A';
char STOP_STATE = 'S';
char START_STATE = 'T';
char READY_STATE = 'R';
int state_time_begin;

void setup() {
  pinMode(ABORT_BTN, INPUT);
  pinMode(STOP_BTN, INPUT);
  pinMode(START_BTN, INPUT);
  pinMode(ABORT_LED, OUTPUT);
  pinMode(READY_LED, OUTPUT);
  
  Serial.begin(9600);
  Serial1.begin(9600);
  
  state = READY_STATE;
}

void loop() {
  //get_data();
  //Serial.print("BCD");
  update_state();
  display_state();
  delay(10);
}

void get_data(){
  // pass temperature data from radio to computer
  if(Serial1.available() > 0){
    Serial.print(Serial1.read());
  }
}

void update_state(){
  if(state == READY_STATE){
    if(!check_button(ABORT_BTN)){
      if(!check_button(STOP_BTN)){
        if(!check_button(START_BTN)){
          //check_state_timer(1000);
        }
      }
    }
  }
  
  if(state == START_STATE){
    if(!check_button(ABORT_BTN)){
      if(!check_button(STOP_BTN)){
        //check_state_timer(1000);
      }
    }
  }
  
  if(state == STOP_STATE){
    if(!check_button(ABORT_BTN)){
      //check_state_timer(3000);
    }
  }
  
  if(state == ABORT_STATE){
    //check_state_timer(5000);
  }
}

int check_button(int btn){
  if(digitalRead(btn) == HIGH){
    state = btn_to_state(btn);
    state_time_begin = millis();
    Serial1.print(state);
    return 1;
  }
  else{
    return 0;
  }
}


char btn_to_state(int btn){
  if(btn == START_BTN){
    return START_STATE; 
  }
  else if(btn == STOP_BTN){
    return STOP_STATE; 
  }
  else if(btn == ABORT_BTN){
    return ABORT_STATE; 
  }
  else{
    return (char)btn;
  }
  
}

int check_state_timer(int state_time_length){
  if(millis()-state_time_begin >= state_time_length){
    state = READY_STATE;
    state_time_begin = millis();
    return 1;
  }
  else{
    return 0;
  }
}

void display_state(){
  if(state == READY_STATE){
    digitalWrite(READY_LED, LOW);
    digitalWrite(ABORT_LED, HIGH);
  }
  else if(state == START_STATE){
    digitalWrite(READY_LED, HIGH);
    digitalWrite(ABORT_LED, HIGH);
  }
  else if(state == STOP_STATE){
    digitalWrite(READY_LED, HIGH);
    digitalWrite(ABORT_LED, HIGH);
  }
  else if(state == ABORT_STATE){
    digitalWrite(READY_LED, HIGH);
    digitalWrite(ABORT_LED, LOW);
  }
}
