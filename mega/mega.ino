#include <SerialRelay.h>
#include <Stepper.h>
#include <Ultrasonic.h>

#define RELE_SIZE 4
#define STEPS 200
#define ULTRASONIC_TRIGGER 52
#define ULTRASONIC_ECHO 51
#define RAIN_DIG 41
#define RAIN_ANA A15

Stepper stepMotor(STEPS, 22,24,23,25);
SerialRelay relays(53,52,1); // (data, clock, number of modules)
Ultrasonic ultrasonic(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO);

int releState[] = {0, 0, 0, 0};

void setup() {
  pinMode(RAIN_DIG, INPUT);
  pinMode(RAIN_ANA, INPUT);

  Serial.begin(9600);
}

void loop() {
  printValues();
  delay(1000);
}

void serialEvent() {
  String stringRead;
  while (Serial.available()) {
    stringRead = Serial.readStringUntil('\n');
    if (stringRead.indexOf("RELE") > -1) {
      processRele(String(stringRead));
    } else if(stringRead.indexOf("STEP") > -1) {
      processStepMotor(String(stringRead));
    }
  }
}

void processRele(String values) {
  values.replace("RELE", "");
  int index  = values.indexOf(",");
  int port   = values.substring(0, index).toInt();
  int action = values.substring(index+1).toInt();
  relays.SetRelay(port, action, 1);
  setReleState(port, action);
}

void setReleState(int port, int value) {
  releState[port-1] = value;
}

void processStepMotor(String value) {
  if(value.indexOf("STEP_SPEED|") > -1) {
    value.replace("STEP_SPEED|", "");
    stepMotor.setSpeed(value.toInt());
  } else if(value.indexOf("STEP_ON|") > -1) {
    value.replace("STEP_ON|", "");
    stepMotor.step(value.toInt());
    for(int i = 22; i <= 25 ; i++) {
      digitalWrite(i,LOW);
    }
  }
}

void printValues() {
  printRele();
  printUltrasonic();
  printRain();
}

void printRele() {
  String value;
  String key;
  for(int i = 0; i < RELE_SIZE; i++) {
    key = "RELE_" + String(i + 1);
    value = buildPrintTemplate(key, String(releState[i]));
    Serial.println(value);
  }
}

void printUltrasonic() {
  long microsec = ultrasonic.timing();
  float cmMsec  = ultrasonic.convert(microsec, Ultrasonic::CM);
  if (cmMsec > 0) {
    Serial.println(buildPrintTemplate("ULTRASONIC", String(cmMsec)));
  }
}

void printRain() {
  int digital = digitalRead(RAIN_DIG);
  int analog  = analogRead(RAIN_ANA);
  String val  = (digital == 0) ? "true" : "false";
  Serial.println(buildPrintTemplate("RAIN", val));

  if (analog > 600 && analog < 900) {
    Serial.println(buildPrintTemplate("RAIN_INTENSITY", "low"));
  } else if (analog > 400 && analog < 600) {
    Serial.println(buildPrintTemplate("RAIN_INTENSITY", "moderate"));
  } else if (analog < 400) {
    Serial.println(buildPrintTemplate("RAIN_INTENSITY", "hard"));
  }
}

String buildPrintTemplate(String key, String value) {
  return key + "[" + value + "]";
}
