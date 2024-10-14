#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX (azul, naranja)
unsigned long nextMillis = 0; // Cambiado a 0 para comenzar desde el principio
int red=3;
int green=4;
int yellow=2;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
}

void loop() {
  // Enviar mensaje cada 100 ms
  digitalWrite(red,LOW);
  digitalWrite(yellow,LOW);
  digitalWrite(green,LOW);
  // Si hay datos disponibles en mySerial, léelos y envíalos a Serial
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  // Si hay datos disponibles en Serial, léelos y envíalos a mySerial
  if (Serial.available()) {
    String data=Serial.readString();
    if (data=="EL"){
      digitalWrite(yellow,HIGH);
      delay(1000);
    }
  }
}