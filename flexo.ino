#include <SerialRelay.h>

const int SENS_TEMP = 1;
const int SENS_LUZ  = 0;
const byte NumModules = 1;
SerialRelay relays(9,8,NumModules); // (data, clock, number of modules)

boolean state_socket_1 = false;

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
  Serial.println("");  
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
  int val = 0;
  while (Serial.available()) {
    val = Serial.read();

    if(val == 1) {
      state_socket_1 = true;
      relays.SetRelay(1, SERIAL_RELAY_ON, 1);
    } else if (val == 101) {
      state_socket_1 = false;
      relays.SetRelay(1, SERIAL_RELAY_OFF, 1);
    }
  }
}
