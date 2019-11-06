#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <SPI.h>

#include <Servo.h>
Servo myservo; 

#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);

String CardID;

byte ActualUID[4];

int ledR = 3;
int ledV = 2;

int EchoPin = 7;
int TriggerPin = 6;
long duration, distancia;

boolean pluma = false;
void setup(){
  pinMode(EchoPin, INPUT);
  pinMode(TriggerPin, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledV, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  myservo.attach(8);
  myservo.write(90);   
}
void medicion(){
   digitalWrite(TriggerPin, LOW);
   delayMicroseconds(4);
   digitalWrite(TriggerPin, HIGH);  
   delayMicroseconds(10);
   digitalWrite(TriggerPin, LOW);
   
   duration = pulseIn(EchoPin, HIGH);
   
   distancia = duration * 10 / 292/ 2; 
   
   delay(1000);
}
void loop(){
  if( mfrc522.PICC_IsNewCardPresent()){
    if(mfrc522.PICC_ReadCardSerial()){
      CardID = String(mfrc522.uid.uidByte[0], HEX) + String(mfrc522.uid.uidByte[1], HEX) + String(mfrc522.uid.uidByte[2], HEX) + String(mfrc522.uid.uidByte[3], HEX) + String(mfrc522.uid.uidByte[4], HEX); 
      delay(1000);
    }
    Serial.println(CardID);
  }else{
    CardID = "";
  }
  if(Serial.available() > 0){
    int input = Serial.read();
    if(input == '0'){
      digitalWrite(ledR, LOW);
      digitalWrite(ledV, HIGH);
      for (int i=90; i <= 180; i++){
              myservo.write(i);              
              delay(5);  
      }
      pluma = true;
      while(pluma){
        medicion();
        if(distancia > 7){
          delay(5000);
          for (int o=180; o >= 90; --o){
                myservo.write(o);              
                delay(15);  
            }
            digitalWrite(ledR, HIGH);
            digitalWrite(ledV, LOW);
            pluma = false;
        }else{
          myservo.write(180); 
        }
      }
      pluma = false;
    }else if(input != '0'){
      myservo.write(90); 
      digitalWrite(ledR, HIGH);
      digitalWrite(ledV, LOW);
    }
  }
}
