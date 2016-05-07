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
       for(int i=1; i<1000; i++) {
        presiuneIntermediara = getPressure_psi();     
        digitalWrite(ledGalben, HIGH);
        sum = sum + presiuneIntermediara;
     }
     lcd.clear();
     media = sum/1000;  
     digitalWrite(ledGalben, LOW);
     digitalWrite(ledVerde, HIGH);
     lcd.print(media);
     Serial.println(media);
     apasaButon = false;
    }

    delay(1000);
}
