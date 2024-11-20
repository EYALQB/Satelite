#include <DHT.h>
#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10); // RX, TX (azul, naranja)
Servo servoMotor;

//Pines
#define DHTPIN 13
int Trig1 = 3;
int Echo1 = 4;
int Trig2 = 5;
int Echo2 = 6;

//Variables Ambiente
#define DHTTYPE DHT11
unsigned long tiempoAmb=0; //Temporizador de ambiente

//Variables ditsancia
int inc=0;
int cambio=1;//Nos servirá para rotar el servo en los dos sentidos
const float VelSon = 34000.0;// Constante velocidad sonido en cm/s
float d1, d2;
unsigned long tiempoDist=0; //Temporizador del servo




DHT dht(DHTPIN, DHTTYPE);
void setup() {
  //Serial
  Serial.begin(9600);
  mySerial.begin(9600);
  //Ambiente
  dht.begin();
  //Distancia
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);
  servoMotor.attach(9);
  servoMotor.write(0);
}
void ambiente(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  mySerial.print("Amb ");
  mySerial.print(t);
  mySerial.print(" ");
  mySerial.println(h);
}
void radar(int *inc){
  //En qué sentido gira el servo
  if (*inc==180){
    cambio = -1;
  }
  if (*inc==0){
    cambio = 1;
  }
  
  servoMotor.write(*inc);
  digitalWrite(Trig1, LOW);
  digitalWrite(Trig1, HIGH);
  unsigned long tiempo1 = pulseIn(Echo1, HIGH);
  d1 = tiempo1 * 0.000001 * VelSon / 2; //Fórmula para calcular la distancia en cm
  
  digitalWrite(Trig2, LOW);
  digitalWrite(Trig2, HIGH);
  unsigned long tiempo2 = pulseIn(Echo2, HIGH);
  d2 = tiempo2 * 0.000001 * VelSon / 2; //Fórmula para calcular la distancia en cm
  mySerial.print("Dis ");
  mySerial.print(d1);
  mySerial.print(" ");
  mySerial.print(*inc);
  mySerial.print(" ");
  mySerial.println(d2);
  *inc=*inc+10*cambio; //Cambiamos la inclinación en 10 grados en el sentido que nos indique el cambio
}

void loop() {
  //Ambiente
  if (millis()-tiempoAmb>=1000){
    ambiente();
    tiempoAmb=millis();
  }
  //Servo
  if (millis()-tiempoDist>=500){
    radar(&inc);
    tiempoDist=millis();
  }
}
