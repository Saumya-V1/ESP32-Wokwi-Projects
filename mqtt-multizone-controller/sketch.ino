#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ESP32Servo.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "test.mosquitto.org";  // public test broker

WiFiClient espClient;
PubSubClient mqttClient(espClient);

#define DHTPIN 4
#define DHTTYPE DHT22
#define LDR_PIN 34
#define SERVO_PIN 13
#define BUZZER_PIN 15
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Servo ventServo;

enum SystemState { IDLE, COOLING, ALERT };
SystemState currentState = IDLE;

float temperature = 0;
float humidity = 0;
int lightLevel = 0;

const float TEMP_ON_THRESHOLD = 28.0;
const float TEMP_OFF_THRESHOLD = 25.0;
const float TEMP_ALERT_THRESHOLD = 32.0;

unsigned long lastPublish = 0;
const unsigned long publishInterval = 3000;

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "esp32-multizone-" + String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
      mqttClient.subscribe("multizone/command");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 2s");
      delay(2000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) message += (char)payload[i];
  Serial.print("MQTT message on [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  if (message == "FORCE_COOL") {
    currentState = COOLING;
  } else if (message == "RESET") {
    currentState = IDLE;
  }
}

void updateStateMachine() {
  switch (currentState) {
    case IDLE:
      if (temperature >= TEMP_ALERT_THRESHOLD) {
        currentState = ALERT;
      } else if (temperature >= TEMP_ON_THRESHOLD) {
        currentState = COOLING;
      }
      break;

    case COOLING:
      if (temperature >= TEMP_ALERT_THRESHOLD) {
        currentState = ALERT;
      } else if (temperature <= TEMP_OFF_THRESHOLD) {
        currentState = IDLE;
      }
      break;

    case ALERT:
      if (temperature <= TEMP_OFF_THRESHOLD) {
        currentState = IDLE;
      }
      break;
  }
}

void applyActuators() {
  switch (currentState) {
    case IDLE:
      ventServo.write(0);
      noTone(BUZZER_PIN);
      break;
    case COOLING:
      ventServo.write(90);
      noTone(BUZZER_PIN);
      break;
    case ALERT:
      ventServo.write(90);
      tone(BUZZER_PIN, 1000);
      break;
  }
}

String stateToString(SystemState s) {
  switch (s) {
    case IDLE: return "IDLE";
    case COOLING: return "COOLING";
    case ALERT: return "ALERT";
  }
  return "UNKNOWN";
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  ventServo.attach(SERVO_PIN);
  pinMode(BUZZER_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }
  display.clearDisplay();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(mqttCallback);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  lightLevel = analogRead(LDR_PIN);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Sensor read failed");
    delay(1000);
    return;
  }

  updateStateMachine();
  applyActuators();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temp: "); display.print(temperature); display.println(" C");
  display.print("Hum: "); display.print(humidity); display.println(" %");
  display.print("Light: "); display.println(lightLevel);
  display.print("State: "); display.println(stateToString(currentState));
  display.display();

  unsigned long now = millis();
  if (now - lastPublish > publishInterval) {
    lastPublish = now;

    String payload = "{";
    payload += "\"temperature\":" + String(temperature) + ",";
    payload += "\"humidity\":" + String(humidity) + ",";
    payload += "\"light\":" + String(lightLevel) + ",";
    payload += "\"state\":\"" + stateToString(currentState) + "\"";
    payload += "}";

    mqttClient.publish("multizone/data", payload.c_str());
    Serial.println("Published: " + payload);
  }
}
