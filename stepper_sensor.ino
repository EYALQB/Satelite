
#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12

#define TRIG_PIN 7 // Pin para el TRIG del sensor
#define ECHO_PIN 8 // Pin para el ECHO del sensor

int step_number = 0;
const int stepsPerRevolution = 2048; // Número de pasos por revolución
const float degreesPerStep = 360.0 / stepsPerRevolution; // Grados por paso
const int stepsPer10Degrees = 10 / degreesPerStep; // Pasos necesarios para 10 grados

void setup() {
  Serial.begin(9600); // Iniciar la comunicación serial
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  
  pinMode(TRIG_PIN, OUTPUT); // Configurar el pin TRIG como salida
  pinMode(ECHO_PIN, INPUT);   // Configurar el pin ECHO como entrada
}

void loop() {
  // Girar en sentido horario
  for (int i = 0; i < stepsPerRevolution; i++) {
    OneStep(true);
    delay(2); // Ajustar la velocidad

    if (i % stepsPer10Degrees == 0 && i != 0) { // Imprimir cada 10 grados
      //long distance = measureDistance(); // Medir la distancia en cada paso
      Serial.print("Grados recorridos (horario): ");
      Serial.print(i * degreesPerStep);
      Serial.print(" | Distancia: ");
      //Serial.print(distance);
      Serial.println(" cm");
    }
  }

  delay(1000); // Esperar un segundo antes de cambiar de dirección

  // Girar en sentido antihorario
  for (int i = 0; i < stepsPerRevolution; i++) {
    OneStep(false);
    delay(2); // Ajustar la velocidad

    if (i % stepsPer10Degrees == 0 && i != 0) { // Imprimir cada 10 grados
      //long distance = measureDistance(); // Medir la distancia en cada paso
      Serial.print("Grados recorridos (antihorario): ");
      Serial.print(360 - (i * degreesPerStep)); // Imprime los grados restantes hacia atrás
      Serial.print(" | Distancia: ");
      //Serial.print(distance);
      Serial.println(" cm"); // Distancia actual
    }
  }
  
  delay(1000); // Esperar un segundo antes de volver a girar
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

