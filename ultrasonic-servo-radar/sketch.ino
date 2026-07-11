#include <ESP32Servo.h>

int trigPin = 5;
int echoPin = 18;
int servoPin = 13;

Servo myServo;
int angle = 0;
int step = 5;

long readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;  // speed of sound conversion
  return distance;
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);
}

void loop() {
  myServo.write(angle);
  delay(200); // let servo settle before measuring

  long distance = readDistanceCM();
  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print("  Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 15) {
    Serial.println("  --> Obstacle detected!");
  }

  angle += step;
  if (angle >= 180 || angle <= 0) {
    step = -step; // reverse sweep direction
  }
}
