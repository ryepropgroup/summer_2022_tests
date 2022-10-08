#include <Wire.h>

//pins
int data_ready = 2;

//temporary variables for i2c connection
int sensor_addr = 0x67;
uint8_t MSB, LSB;
int rv;

//variables
uint32_t timestamp;
float ign_temp;
float ign_volts;



void setup() {
  pinMode(data_ready, INPUT);

  //set all data variables to 0
  timestamp = 0;
  ign_temp = 0;
  ign_volts = 0;
  
  // put your setup code here, to run once:
  //MCP9600 Thermiciople Snesor Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/MCP960X-Data-Sheet-20005426.pdf
  Wire.setClock(100000); //MCP9600 has max I@C serial frequency of 100kHz (page 9)
  Wire.begin();   //join i2c bus, address optional for master device (the Arduino Mega)
  Wire.beginTransmission(sensor_addr); //write to device #6
  Wire.write(0b00000110); //specify the device configuration register (page 27)
  Wire.write(0b11100000); //specify lowest resolution for fastest sampling mode (page 36)
                          //cold junction res = 0.25*C, ADC res = 12 bits, burst mode samples = 1, shutdown mode = normal operation
  Wire.endTransmission();
  
  Serial.begin(115200);
  Serial.println("igniter temp, igniter voltage");  //labels for serial plotter
}

void loop() {
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
  
  //read igniter voltage
  ign_volts = ((float)(analogRead(A7)*5))/1024.0;
  
  //send current data to laptop in CSV format
  Serial.print(timestamp);
  Serial.print(",");
  Serial.print(ign_temp);
  Serial.print(",");
  Serial.println(ign_volts);
  
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
  timestamp = micros();
  
  ign_temp = ((MSB << 8) + LSB)/16.0;
}
