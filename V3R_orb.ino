#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10); // RX, TX (azul, naranja)
unsigned long Tr = 2000;
unsigned long Tg = 100;
unsigned long Ty = 100;
int yellow = 2;
int red = 3;
int green = 4;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {
  // Control de los tiempos de encendido y apagado de los LEDs
  if (millis() >= Tg) { digitalWrite(green, LOW); }
  if (millis() >= Ty) { digitalWrite(yellow, LOW); }
  if (millis() >= Tr) { digitalWrite(red, HIGH); }

  // Lectura de datos desde el puerto serial
  if (mySerial.available()) {
    String data = mySerial.readString();
    String tipo = data.substring(0, 3);
    if (tipo == "Amb") {
      String control1 = data.substring(4, 7);
      String control2 = data.substring(8, 11);
      if (control1 == "nan" || control2 == "nan") {
        Serial.println("Error Amb");
        digitalWrite(yellow, HIGH);
        Ty = millis() + 100;
      } else {
        digitalWrite(green, HIGH);
        Tg = millis() + 500;
        Serial.println(data);  // Para depuración en el Monitor Serial
      }
    }
    if (tipo == "Dis") {
      String control1 = data.substring(4, 7);
      String control2 = data.substring(8, 11);
      if (control1 == "0.00" || control2 == "0.00") {
        Serial.println("Err Dist");
        digitalWrite(yellow, HIGH);
        Ty = millis() + 100;
      } else {
        digitalWrite(green, HIGH);
        Tg = millis() + 500;
        Serial.println(data);  // Para depuración en el Monitor Serial
      }
    }
    if (tipo == "Orb") {
      int idxX = data.indexOf("X:");
      int idxY = data.indexOf("Y:");
      int idxZ = data.indexOf("Z:");
      if (idxX == -1 || idxY == -1 || idxZ == -1) {
        Serial.println("Error Orb");
        digitalWrite(yellow, HIGH);
        Ty = millis() + 100;
      } else {
        digitalWrite(green, HIGH);
        Tg = millis() + 500;
        Serial.println(data);  // Para depuración en el Monitor Serial
      }
    }
    digitalWrite(red, LOW);
    Tr = millis() + 2000;
  }
}
