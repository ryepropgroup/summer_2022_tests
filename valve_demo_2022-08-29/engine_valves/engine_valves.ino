#include <Wire.h>

char ABORT_STATE = 'A';
char STOP_STATE = 'S';
char START_STATE = 'T';
char READY_STATE = 'R';

int SV1 = 9;
int SV2 = 10;
int SV3 = 11;
int SV4 = 12;
int SV5 = 13;

//pins
int data_ready = 8;

//temporary variables for i2c connection
int sensor_addr = 0x67;
uint8_t MSB, LSB;
int rv;

//variables
float ign_temp;


void setup() {
  setup_temp_sensor();

  pinMode(SV1, OUTPUT);
  pinMode(SV2, OUTPUT);
  pinMode(SV3, OUTPUT);
  pinMode(SV4, OUTPUT);
  pinMode(SV5, OUTPUT);

  digitalWrite(SV1, LOW);
  digitalWrite(SV2, LOW);
  digitalWrite(SV3, LOW);
  digitalWrite(SV4, LOW);
  digitalWrite(SV5, LOW);
  
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  //get_temp();
  
  control_valves();
}

void setup_temp_sensor(){
  pinMode(data_ready, INPUT);

  //set all data variables to 0
  ign_temp = 0;
  
  // put your setup code here, to run once:
  //MCP9600 Thermiciople Snesor Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/MCP960X-Data-Sheet-20005426.pdf
  Wire.setClock(100000); //MCP9600 has max I@C serial frequency of 100kHz (page 9)
  Wire.begin();   //join i2c bus, address optional for master device (the Arduino Mega)
  Wire.beginTransmission(sensor_addr); //write to device #6
  Wire.write(0b00000110); //specify the device configuration register (page 27)
  Wire.write(0b11100000); //specify lowest resolution for fastest sampling mode (page 36)
                          //cold junction res = 0.25*C, ADC res = 12 bits, burst mode samples = 1, shutdown mode = normal operation
  Wire.endTransmission();
}

void get_temp(){
    //110 0XXX = 0x66 or 0x67, add a bit on the right to specify read or write command (page 22)
  //read = 1, write = 0
  Wire.beginTransmission(sensor_addr); //write to device #6
  Wire.write(0b00000000); //specify the thermocoule hot-junction register (page 27)
  /*Serial.println("calling endTransmission");*/
  rv = Wire.endTransmission(); //0 if device on i2c acknowledged the request
  if(rv != 0){
    /*Serial.print("Error! Request not acknowledged! return value not 0, is ");
    Serial.println(rv);*/
  }
  
  //send current data to laptop in CSV format
  Serial.println(ign_temp);
  
  //wait until temperature data ready
  while(digitalRead(data_ready) != 1){};
  /*
  delay(80);  //typical conversion time for temp reading of the delta T thermocouple signal in ADC is 5ms for 12 bit resolution (page 6)
              //bear in mind that the cold junction temp has conversion time of 63ms at resolution of 0.25*C (page 31)
              //bear in mind that typical temp calculation time (Th = Tc + delta T) is 12 ms (page 6)
  */
  
  
  //ask for temperature from sensor
  Wire.requestFrom(sensor_addr, 2, false); //read from device #6, release i2c line afterwards
  
  //do nothing until 2 bytes have been received
  while(Wire.available() < 2){};
  
  MSB = Wire.read();  //read the MSB of the temperature
  LSB = Wire.read();  //read the LSB of the temperature
  
  ign_temp = ((MSB << 8) + LSB)/16.0;
}

void control_valves(){
  char state;
  if(Serial1.available() > 0){
    state = Serial1.read();
    Serial.print(state);
    if(state == START_STATE){
      digitalWrite(SV1, HIGH);
      delay(300);
      digitalWrite(SV2, HIGH);
      delay(300);
      digitalWrite(SV3, HIGH);
      delay(300);
      digitalWrite(SV4, HIGH);
      delay(300);
      digitalWrite(SV5, HIGH);
    }
    else if(state == STOP_STATE){
      digitalWrite(SV1, LOW);
      delay(300);
      digitalWrite(SV2, LOW);
      delay(300);
      digitalWrite(SV3, LOW);
      delay(300);
      digitalWrite(SV4, LOW);
      delay(300);
      digitalWrite(SV5, LOW);
    }
    else if(state == ABORT_STATE){
      digitalWrite(SV1, LOW);
      digitalWrite(SV2, LOW);
      digitalWrite(SV3, LOW);
      digitalWrite(SV4, LOW);
      digitalWrite(SV5, LOW);
    }
  }
}
