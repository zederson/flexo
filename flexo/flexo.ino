#include <IRremote.h>
#include "DHT.h"

#define DHTTYPE   DHT11
#define IR_PIN    12
#define LED_IR    3
#define GAS_PIN   A0
#define DHTPIN    A1
#define SENS_LUZ  A2

DHT dht(DHTPIN, DHTTYPE);
IRrecv irrecv(IR_PIN);
IRsend irsend;
decode_results results;
String stringRead;

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
  pinMode(LED_IR ,OUTPUT);
  pinMode(GAS_PIN, INPUT);
  irrecv.enableIRIn();
  dht.begin();
}

void loop() {
  processIR();
  printAll();
  delay(1000);
}

void printAll() {
  printTemperatura();
  printLuminozidade();
  printBreathalyzer();
}

String buildPrintTemplate(String key, String value) {
  return key + "[" + value + "]";
}

void printTemperatura() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (!isnan(t) && !isnan(h)) {
    Serial.println(buildPrintTemplate("TEMPERATURA", String(t)));
    Serial.println(buildPrintTemplate("HUMIDADE", String(h)));
  }
}

void printLuminozidade() {
  int value  = analogRead(SENS_LUZ);
  float val  = value / 1023.0 * 100.0;
  String out = buildPrintTemplate("LUMINOZIDADE", String(val));
  Serial.println(out);
}

void printBreathalyzer() {
  int value  = analogRead(GAS_PIN);
  Serial.println(buildPrintTemplate("BAFOMETRO", String(value)));
}

void serialEvent() {
  while (Serial.available()) {
    stringRead = Serial.readStringUntil('\n');

    if(stringRead.indexOf("IR|") > -1) {
      sendIR(String(stringRead));
    }
  }
}

void sendIR(String value) {
  value.replace("IR|", "");
  irsend.sendNEC(strtoul(value.c_str(), 0, 16) ,32);
  irrecv.enableIRIn();
  irrecv.resume();
}

void processIR() {
  if (irrecv.decode(&results)) {
    String out = buildPrintTemplate("IR_RECEIVE", String(results.value, HEX));
    out.toUpperCase();
    irrecv.resume();
    Serial.println(out);
  }
}
