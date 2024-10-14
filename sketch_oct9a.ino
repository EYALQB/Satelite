#include <SoftwareSerial.h>
#include <DHT.h> // Ensure you include necessary libraries
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp; // I2C


SoftwareSerial mySerial(10, 11); // RX, TX (azul, naranja)
unsigned long nextMillis = 0; // Cambiado a 0 para comenzar desde el principio
DHT dht(2, DHT22); // Initialize your DHT object with pin and type
int i = 0; // Declare i globally
float T[10]; // Declare the array globally
float H[10];

// Definir pines del LED RGB
const int ledRojo = 5;
const int ledVerde = 6;
const int ledAzul = 7;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  dht.begin(); 
  bmp.begin();

    // Configurar pines del LED RGB como salida
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // float p = bmp.readPressure();
  // float alt = bmp.readAltitude(1013.25);    SENSOR DE PRESIÓN EN PROCESO DE FUNCIONAMIENTO

  // Enviar mensaje cada 100 ms
  if (millis() >= nextMillis) {
    mySerial.print(t);
    mySerial.print(" ");
    mySerial.println(h);
    
    Serial.print(t);
    Serial.print(" ");
    Serial.println(h);
    nextMillis = millis() + 1000; // Actualiza el tiempo para el próximo envío
  }

  // Si hay datos disponibles en mySerial, léelos y envíalos a Serial
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  // Si hay datos disponibles en Serial, léelos y envíalos a mySerial
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }

  if (t > 27){
      // Encender LED rojo, apagar verde y azul
      analogWrite(ledRojo, 255);  // Rojo al máximo
      analogWrite(ledVerde, 0);   // Apagar verde
      analogWrite(ledAzul, 0);    // Apagar azul
  }
  else{
    // Encender LED rojo, apagar verde y azul
      analogWrite(ledRojo, 0);  // Rojo al máximo
      analogWrite(ledVerde, 255);   // Apagar verde
      analogWrite(ledAzul, 0);    // Apagar azul
  }

}