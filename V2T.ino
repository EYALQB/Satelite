#include <DHT.h>
#include <Servo.h>

Servo servoMotor;
 
unsigned long t1;
int inc=0;
int cambio=0;
// Configuramos los pines del sensor Trigger y Echo
int Trig = 3;
int Echo = 4;
// Constante velocidad sonido en cm/s
const float VelSon = 34000.0;
float d;

#define DHTPIN 2
#define DHTTYPE DHT11
unsigned long T;

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(9600);
  dht.begin();
  // Ponemos el pin Trig en modo salida
  pinMode(Trig, OUTPUT);
  // Ponemos el pin Echo en modo entrada
  pinMode(Echo, INPUT);
  // Iniciamos el servo para que empiece a trabajar con el pin 9
  servoMotor.attach(9);
  servoMotor.write(0);
  }
void loop() {
  if (millis()>=T){
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.print(t);
    Serial.print(" ");
    Serial.print(h);
    Serial.print(" ");
    Serial.print(h);
    Serial.print(" ");
    Serial.println(h);
    T=millis()+500;
  }
  if (millis()>=t1){
    if (inc==180){
      cambio = -1;
    }
    if (inc==0){
      cambio = 1;
    }
    
    servoMotor.write(inc);
    digitalWrite(Trig, LOW);
    digitalWrite(Trig, HIGH);
    // La función pulseIn obtiene el tiempo que tarda en cambiar entre estados, en este caso a HIGH
    unsigned long tiempo = pulseIn(Echo, HIGH);
  
    // Obtenemos la distancia en cm, hay que convertir el tiempo en segudos ya que está en microsegundos
    // por eso se multiplica por 0.000001
    d = tiempo * 0.000001 * VelSon / 2;
    Serial.print(d);
    Serial.print(" ");
    Serial.println(inc);
    inc=inc+10*cambio;
    t1=millis()+100;
  }
}