
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

//simulador de orbita
const double G = 6.67430e-11; // Gravitational constant (m^3 kg^-1 s^-2)
const double M = 5.97219e24; // Mass of Earth (kg)
const double R_EARTH = 6371000; // Radius of Earth (meters)
const double ALTITUDE = 400000; // Altitude of satellite above Earth's surface (meters)
const double EARTH_ROTATION_RATE = 7.2921159e-5; // Earth's rotational rate (radians/second)
const unsigned long MILLIS_BETWEEN_UPDATES = 1000; // Time in milliseconds between each orbit simulation update
const double TIME_COMPRESSION = 90.0; // Time compression factor (90x) Variables
unsigned long nextUpdate; // Time in milliseconds when the next orbit simulation update should occur
double real_orbital_period; // Real orbital period of the satellite (seconds)
double r; // Total distance from Earth's center to satellite (meters)


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

  nextUpdate = MILLIS_BETWEEN_UPDATES;
  r = R_EARTH + ALTITUDE;
  real_orbital_period = 2 * PI * sqrt(pow(r, 3) / (G * M));
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

  //orbita
  unsigned long currentTime = millis();
  if(currentTime>nextUpdate) {
    simulate_orbit(currentTime, 1, 1);
    nextUpdate = currentTime + MILLIS_BETWEEN_UPDATES;
  }
}

//simulador de orbitas
void simulate_orbit(unsigned long millis, double inclination, int ecef) {
  
  double time = (millis / 1000) * TIME_COMPRESSION; // Real orbital time
  double angle = 2 * PI * (time / real_orbital_period); // Angle in radians
  double x = r * cos(angle); // X-coordinate (meters)
  double y = r * sin(angle) * cos(inclination); // Y-coordinate (meters)
  double z = r * sin(angle) * sin(inclination); // Z-coordinate (meters)

  if (ecef) {
    double theta = EARTH_ROTATION_RATE * time;
    double x_ecef = x * cos(theta) - y * sin(theta);
    double y_ecef = x * sin(theta) + y * cos(theta);
    x = x_ecef;
    y = y_ecef;
  }

// Send the data to the serial port
  mySerial.print("Time: ");
  mySerial.print(time);
  mySerial.print(" s | Pos: (X: ");
  mySerial.print(x);
  mySerial.print(" m, Y: ");
  mySerial.print(y);
  mySerial.print(" m, Z: ");
  mySerial.print(z);
  mySerial.println(" m)");
}