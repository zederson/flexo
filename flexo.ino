#include <SerialRelay.h>
#include <IRremote.h>
#include "DHT.h"
#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);
SerialRelay relays(6,7,1); // (data, clock, number of modules)

const int SENS_LUZ     = 0;
const int IR_PIN       = 4;
const int LED_IR       = 3;

boolean state_socket_1 = false;
String stringRead;

IRrecv irrecv(IR_PIN);
IRsend irsend;
decode_results results;

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
  pinMode(LED_IR ,OUTPUT);
  irrecv.enableIRIn();
  dht.begin();
}

void loop() {
  printTemperatura();
  printLuminozidade();
  printSocketState();
  processIR();
  delay(1000);
}

void printTemperatura() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (!isnan(t) && !isnan(h)) {
    String out = "Temperatura[";
    out       += t;
    out       += "]";
    Serial.println(out);

    out = "Humidade[";
    out       += h;
    out       += "]";
    Serial.println(out);
  }
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
    } else if(stringRead.indexOf("IR|") > -1) {
      stringRead.replace("IR|", "");
      sendIR(stringRead);
    }
  }
}

void sendIR(String value) {
  irsend.sendNEC(strtoul(value.c_str(), 0, 16) ,32);
  irrecv.enableIRIn();
  irrecv.resume();
}

void processIR() {
  if (irrecv.decode(&results)) {
    String out = "IR_RECEIVE[";
    out       += String(results.value, HEX);
    out       += "]";
    out.toUpperCase();
    irrecv.resume();
    Serial.println(out);
  }
}
