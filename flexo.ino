#include <SerialRelay.h>
#include <IRremote.h>

SerialRelay relays(6,7,1); // (data, clock, number of modules)

const int SENS_TEMP    = 1;
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
  Serial.println();
}

void loop() {
  printTemperatura();
  printLuminozidade();
  printSocketState();
  processIR();
  delay(1000);
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

