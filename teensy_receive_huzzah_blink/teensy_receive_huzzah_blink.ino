#define PIN 4

void setup() {
  pinMode(PIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  Serial.println(digitalRead(PIN));
  delay(100);
}
