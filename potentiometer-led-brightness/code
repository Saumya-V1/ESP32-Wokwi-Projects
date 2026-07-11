int potPin = 34;
int ledPin = 2;
int pwmFreq = 5000;
int pwmResolution = 8;

void setup() {
  Serial.begin(115200);
  ledcAttach(ledPin, pwmFreq, pwmResolution);
}

void loop() {
  int rawValue = analogRead(potPin);
  int brightness = map(rawValue, 0, 4095, 0, 255);

  ledcWrite(ledPin, brightness);

  Serial.print("Pot: ");
  Serial.print(rawValue);
  Serial.print("  Brightness: ");
  Serial.println(brightness);

  delay(50);
}
