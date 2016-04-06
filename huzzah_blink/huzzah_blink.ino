#define PIN 4
#define INDICATOR 0

void setup() {
  pinMode(PIN, OUTPUT);
  pinMode(INDICATOR, OUTPUT);
}

void loop() {
  digitalWrite(PIN, HIGH);
  digitalWrite(INDICATOR, HIGH);
  delay(500);
  digitalWrite(PIN, LOW);
  digitalWrite(INDICATOR, LOW);
  delay(500);
}
