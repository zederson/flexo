#include <SerialRelay.h>

SerialRelay relays(8,9,1); // (data, clock, number of modules)

const int SENS_TEMP = 1;
const int SENS_LUZ  = 0;
boolean state_socket_1 = false;
String stringRead;

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
  Serial.println();
}

void loop() {
  printTemperatura();
  printLuminozidade();
  printSocketState();
  
  delay(2000);
}

void printTemperatura() {
  int value         = analogRead(SENS_TEMP);
  float temperatura = value * 0.1075268817204301;

  String out = "Temperatura[";
  out       += temperatura;
  out       += "]";
  Serial.println(out);
}

void printLuminozidade() {
  int value = analogRead(SENS_LUZ);

  String out = "Luminozidade[";
  out       += value;
  out       += "]";
  Serial.println(out);
}

void printSocketState() {
  String out = "Socket_1[";
  out       += state_socket_1;
  out       += "]";
  Serial.println(out);
}

void serialEvent() {
  while (Serial.available()) {
    stringRead = Serial.readStringUntil('\n');

    if(stringRead == "1") {
      state_socket_1 = true;
      relays.SetRelay(2, SERIAL_RELAY_ON, 1);
    } else if (stringRead == "101") {
      state_socket_1 = false;
      relays.SetRelay(2, SERIAL_RELAY_OFF, 1);
    }
  }
}
