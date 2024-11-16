#include <DHT.h>
#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10); // RX, TX (azul, naranja)

int Trig1 = 3;
int Echo1 = 4;

unsigned long tiempoAmb; // Temporizador de ambiente

int step_number = 0;
const int stepsPerRevolution = 2048; // Número de pasos por revolución
const float degreesPerStep = 360.0 / stepsPerRevolution; // Grados por paso
const int stepsPer30Degrees = round(15 / degreesPerStep); // Pasos necesarios para 30 grados

#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  dht.begin();

  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);
}

void loop() {
  if (millis() >= tiempoAmb) {
    // Girar en sentido horario
    for (int i = 0; i < stepsPerRevolution; i++) {
      OneStep(true);
      delay(2); // Ajustar la velocidad del motor

      if (i % stepsPer30Degrees == 0) { 
        long distance = measureDistance(); // Medir la distancia
        mySerial.print("G (h): ");
        mySerial.print(i * degreesPerStep);
        mySerial.print(" | D: ");
        mySerial.print(distance);
        mySerial.println(" cm");

        float h = dht.readHumidity();
        float t = dht.readTemperature();
        mySerial.print("Ambiente - Temp: ");
        mySerial.print(t);
        mySerial.print(" C, Hum: ");
        mySerial.print(h);
        mySerial.println(" %");
      }
    }

    delay(1000); // Esperar un segundo antes de cambiar de dirección

    // Girar en sentido antihorario
    for (int i = 0; i < stepsPerRevolution; i++) {
      OneStep(false);
      delay(2); // Ajustar la velocidad del motor

      if (i % stepsPer30Degrees == 0) { 
        long distance = measureDistance(); // Medir la distancia
        mySerial.print("G (antih): ");
        mySerial.print(360 - (i * degreesPerStep)); // Imprime los grados restantes hacia atrás
        mySerial.print(" | D: ");
        mySerial.print(distance);
        mySerial.println(" cm");

        float h = dht.readHumidity();
        float t = dht.readTemperature();
        mySerial.print("Ambiente - Temp: ");
        mySerial.print(t);
        mySerial.print(" C, Hum: ");
        mySerial.print(h);
        mySerial.println(" %");
      }
    }

    delay(1000); // Esperar un segundo antes de volver a girar
  
    tiempoAmb = millis() + 500;
  }
}

void OneStep(bool dir) {
  if (dir) {
    switch (step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
    }
  } else {
    switch (step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
    }
  }
  
  step_number++;
  if (step_number > 3) {
    step_number = 0;
  }
}

long measureDistance() {
  digitalWrite(Trig1, LOW);
  delayMicroseconds(2); // Asegúrate de que el pin TRIG esté bajo por al menos 2 microsegundos
  digitalWrite(Trig1, HIGH);
  delayMicroseconds(10); // Enviar un pulso de 10 microsegundos
  digitalWrite(Trig1, LOW);

  // Leer el tiempo de retorno del pulso
  long duration = pulseIn(Echo1, HIGH);
  
  // Calcular la distancia (en cm)
  long distance = duration * 0.034 / 2; // Velocidad del sonido: 0.034 cm/µs
  return distance;
}
