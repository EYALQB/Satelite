#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10); // RX, TX (azul, naranja)
unsigned long Tr = 2000; // Cambiado a 0 para comenzar desde el principio
unsigned long Tg = 100;
unsigned long Ty = 100;
int yellow=2;
int red=3;
int green=4;


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  }

void loop() {
  if (millis()>=Tg){digitalWrite(green,LOW);}
  if (millis()>=Ty){digitalWrite(yellow,LOW);}
  if (millis()>=Tr){digitalWrite(red,HIGH);}
  if (mySerial.available()) {
    digitalWrite(green,HIGH);
    Tg=millis()+500;
    digitalWrite(red,LOW);
    Tr=millis()+2000;
    Serial.write(mySerial.read());
    char data = mySerial.read();
    if (data=='e'){
      digitalWrite(yellow,HIGH);
      Ty=millis()+100;
    }
  }

  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}