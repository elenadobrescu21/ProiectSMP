#include <Wire.h>

const int sensorAddress = 0x28;
const int pMax = 100;
const int outMin = 1638;
const int outMax = 14745;
boolean apasaButon = false;

void setup() {
  attachInterrupt(0,ISR_b1,FALLING);
  Wire.begin();
  Serial.begin(9600);  
}

void ISR_b1() {
  apasaButon = true;
  //debouncing software
  delay(400);
}
float getPressure_psi() {
  byte byterequest=2;
  byte a;
  byte b;
  uint16_t P_bin;
  float Pressure_psi;
  
  Wire.requestFrom(sensorAddress, byterequest);    

  if(Wire.available())    // slave may send less than requested
  {
    a = Wire.read();    // receive a byte as character
    delay(4);
    b = Wire.read();
    delay(4);
    Wire.endTransmission();
  }
  P_bin = ((a & 0x3f) << 8) | b;
    float ab = P_bin - outMin;
    float numitor = outMax - outMin;
    float intermediate = ab*200/numitor;
    Pressure_psi = intermediate - pMax;

    return Pressure_psi;
  
}

void loop() {

    float presiuneIntermediara;
    float sum = 0;
    float media;
    Serial.println("Apasati butonul de pornire pentru a citi o valoare");
    if(apasaButon == true) {
       for(int i=1; i<500; i++) {
        presiuneIntermediara = getPressure_psi();
        sum = sum + presiuneIntermediara;
     }
     media = sum/500;  
     Serial.print("Presiunea citita este: "); 
     Serial.println(media);
     apasaButon = false;
    }

    delay(1000);
}
