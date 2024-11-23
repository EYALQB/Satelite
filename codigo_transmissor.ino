#include <Servo.h>
#include <DHT.h>
#include <SoftwareSerial.h>

Servo miServo;  // Crear objeto Servo
SoftwareSerial mySerial(10, 11); // RX, TX (azul, naranja)

// Pines
const int pinServo = 9; // Pin al que está conectado el servo
#define DHTPIN 2
int trig1 = 3;
int echo1 = 4;
int trig2 = 5;
int echo2 = 6;

//                                   VARIABLES
// Intervalos de tiempo
long intervaloAmb = 1000;
long tiempoAmb = 0;
long intervaloDis = 1000;
long tiempoDis = 0;
long intervaloLeer = 1000;
long tiempoLeer = 0;

// Variables servo
int angulo = 0;           // Variable para almacenar el ángulo actual del servo
int direccion = 1;        // Controla la dirección del movimiento: 1 para adelante, -1 para atrás
const int anguloMax = 180; // Ángulo máximo
const int anguloMin = 0;   // Ángulo mínimo
const int delayMovimiento = 20;  // Tiempo de espera entre cada paso (ms)
int estadoAuto = 1;
char comando;
int paso = 5;
float d1, d2;

// Variables ambiente
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float t, h;
float temperaturas[10];
float humedades[10];
int i = 0;
float sumaTemperaturas = 0;
float sumaHumedades = 0;
int estadoMedia = 0;
float mediaTemperatura = 0;
float mediaHumedad = 0;

//                                  SETUP
void setup(){
  Serial.begin(9600);
  mySerial.begin(9600);
  miServo.attach(pinServo); // Conectar el servo al pin 
  dht.begin();

  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
}

//                                  LOOP
void loop(){
  moverServo();  // Llamada a la función para mover el servo continuamente
  if (millis() - tiempoAmb >= intervaloAmb){
    leerAmbiente();
    calcularMedia();
    transmitirAmb(t,h);
    tiempoAmb = millis();
  }
  if (millis() - tiempoDis >= intervaloDis){
    leerDistancia();
    transmitirDis();
    tiempoDis = millis();
  }
  if (millis() - tiempoLeer >= intervaloLeer){
    leerDatos();
    tiempoLeer = millis();
  }
}

//                                  FUNCIONES
void moverServo(){
  if (estadoAuto){
    miServo.write(angulo); // Mover el servo al ángulo actual
    delay(delayMovimiento); // Esperar un poco para que el servo tenga tiempo de moverse
    angulo += direccion;     // Cambiar el ángulo

    // Cambiar de dirección
    if (angulo >= anguloMax || angulo <= anguloMin) {
      direccion = -direccion;  // Invertir la dirección del movimiento
    }
  }
  else{
    if (comando == 'd' && angulo < anguloMax) {  // Comando para mover a la derecha
      angulo += paso;
      miServo.write(angulo);
    }
    if (comando == 'i' && angulo > anguloMin) {  // Comando para mover a la izquierda
      angulo -= paso;
      miServo.write(angulo);
    }
  }

}
void leerAmbiente(){
  h = dht.readHumidity();
  t = dht.readTemperature();
}
void leerDistancia(){
  // Sensor 1
  digitalWrite(trig1, LOW);  // Asegurar que el pin está en LOW al principio
  delayMicroseconds(2);        // Esperar 2 microsegundos
  digitalWrite(trig1, HIGH); // Enviar pulso HIGH de 10 microsegundos
  delayMicroseconds(10);
  digitalWrite(trig1, LOW);  // Volver a LOW
  long duracion1 = pulseIn(echo1, HIGH);
  float d1 = duracion1 * 0.0343 / 2;

  // Sensor 2
  digitalWrite(trig2, LOW);  // Asegurar que el pin está en LOW al principio
  delayMicroseconds(2);        // Esperar 2 microsegundos
  digitalWrite(trig2, HIGH); // Enviar pulso HIGH de 10 microsegundos
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);  // Volver a LOW
  long duracion2 = pulseIn(echo2, HIGH);
  float d2 = duracion2 * 0.0343 / 2;
}
void calcularMedia(){
  // Temperatura
  sumaTemperaturas -= temperaturas[i];
  temperaturas[i] = t;
  sumaTemperaturas += t;
  i = (i + 1) % 10;
  float mediaTemperatura = sumaTemperaturas / 10;

  // Humedad
  sumaHumedades -= humedades[i];
  humedades[i] = h;
  sumaHumedades += h;
  i = (i + 1) % 10;
  float mediaHumedad = sumaHumedades / 10;
}
void transmitirAmb(float t, float h){
  mySerial.print("Amb ");
  mySerial.print(t);
  mySerial.print(" ");
  mySerial.println(h);
  if (estadoMedia){
    mySerial.print(mediaTemperatura);
    mySerial.print(" ");
    mySerial.println(mediaHumedad);
  }
}
void transmitirDis(){
  mySerial.print("Dis ");
  mySerial.print(d1);
  mySerial.print(" ");
  mySerial.print(d2);
  mySerial.print(" ");
  mySerial.println(angulo);
}
void leerDatos(){
  if (mySerial.available()) {
    String data = mySerial.readString();
    int estadoAuto = data[0] - '0';
    int estadoMedia = data[1] - '0';
    char comando = data[2];
  }
}