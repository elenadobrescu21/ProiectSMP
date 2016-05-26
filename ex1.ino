
#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, NULL, 11, 9,8,7,6);

const int sensorAddress = 0x28;
const int pMax = 100;
const int outMin = 1638;
const int outMax = 14745;
boolean apasaButon = false;
const int ledGalben = 3;
const int ledVerde = 4;
float rho = 1.225; //densitatea aerului
float pi = 3.14;
float diametruMare = 2.7;
float diametruMic = 1.1;
float dt = 0.001; 
const float volumEchilibru = 10.44;

void setup() {
  attachInterrupt(0,ISR_b1,FALLING);
  Wire.begin();

  pinMode(ledGalben, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  
  //configurare LCD
  lcd.begin(16, 2);
  lcd.noCursor();
  Serial.begin(9600);  
}

void ISR_b1() {
  apasaButon = true;
  //debouncing software
  delay(500);
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
    float pressure_pa;
    float arieMare;
    float arieMica;
    float massFlow;
    float volFlow;
    float volume;

    arieMare = pi*diametruMare*diametruMare/4;
    arieMica = pi*diametruMic*diametruMic/4;
    
    lcd.setCursor(0, 0);
    if(apasaButon == false) {
    digitalWrite(ledGalben, LOW);
   // digitalWrite(ledVerde, LOW);
    lcd.print("Apasati buton de");
    lcd.setCursor(0,1);
    lcd.print("pornire");
    }
    //Serial.println("Apasati butonul de pornire pentru a citi o valoare");
    if(apasaButon == true) {
      lcd.clear();
      lcd.print("Suflati");
      digitalWrite(ledVerde, LOW);
       for(int i=1; i<600; i++) {
        presiuneIntermediara = getPressure_psi();     
        digitalWrite(ledGalben, HIGH);
        sum = sum + presiuneIntermediara;
     }
     lcd.clear();
     media = sum/600;  
     digitalWrite(ledGalben, LOW);
     digitalWrite(ledVerde, HIGH);
     pressure_pa = media*6894.75729;
     massFlow = 1000*sqrt((abs(pressure_pa)*2*rho)/((1/(pow(arieMica,2)))-(1/(pow(arieMare,2)))));
     volFlow = massFlow/rho; 
     volume = volFlow*dt;
     Serial.println(volume);
     apasaButon = false;
    }

    delay(1000);
}
