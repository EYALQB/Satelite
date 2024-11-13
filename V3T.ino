#include <DHT.h>
#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10); // RX, TX (azul, naranja)

Servo servoMotor;
 
int inc=0;
int cambio=0;//Nos servirá para rotar el servo en los dos sentidos
int Trig1 = 3;
int Echo1 = 4;
int Trig2 = 5;
int Echo2 = 6;
// Constante velocidad sonido en cm/s
const float VelSon = 34000.0;
float d1, d2;
unsigned long tiempoRadar; //Temporizador del servo

#define DHTPIN 2
#define DHTTYPE DHT22
unsigned long tiempoAmb; //Temporizador de ambiente

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  dht.begin();
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);
  servoMotor.attach(9);
  servoMotor.write(0);
  }
void loop() {
  if (millis()>=tiempoAmb){
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.print("Amb ");
    Serial.print(t);
    Serial.print(" ");
    Serial.print(h);
    Serial.print(" ");
    Serial.print(h);
    Serial.print(" ");
    Serial.println(h);
    tiempoAmb=millis()+500;
  }
  if (millis()>=tiempoRadar){
    //En qué sentido gira el servo
    if (inc==180){
      cambio = -1;
    }
    if (inc==0){
      cambio = 1;
    }
    
    servoMotor.write(inc);
    digitalWrite(Trig1, LOW);
    digitalWrite(Trig1, HIGH);
    unsigned long tiempo1 = pulseIn(Echo1, HIGH);
    d1 = tiempo1 * 0.000001 * VelSon / 2; //Fórmula para calcular la distancia en cm
    
    digitalWrite(Trig2, LOW);
    digitalWrite(Trig2, HIGH);
    unsigned long tiempo2 = pulseIn(Echo2, HIGH);
    d2 = tiempo2 * 0.000001 * VelSon / 2; //Fórmula para calcular la distancia en cm
    Serial.print("dist ");
    Serial.print(d1);
    Serial.print(" ");
    Serial.print(inc);
    Serial.print(" ");
    Serial.println(d2);
    inc=inc+10*cambio; //Cambiamos la inclinación en 10 grados en el sentido que nos indique el cambio
    tiempoRadar=millis()+100;
  }
}
