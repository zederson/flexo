const int SENS_TEMP = 1;
const int SENS_LUZ  = 0;
const int LED = 8;

boolean state_socket_1 = false;

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
  pinMode(LED, OUTPUT);
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
      digitalWrite(LED, HIGH);
    } else if (val == 101) {
      state_socket_1 = false;
      digitalWrite(LED, LOW);
    }
  }
}
