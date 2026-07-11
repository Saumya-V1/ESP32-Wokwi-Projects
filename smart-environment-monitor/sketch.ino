#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ESP32Servo.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

#define DHTPIN 4
#define DHTTYPE DHT22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
WebServer server(80);
Servo ventServo;

float temperature = 0;
float humidity = 0;
bool manualOverride = false;
bool ventOpen = false;
float tempThreshold = 28.0;

void updateVent() {
  if (ventOpen) {
    ventServo.write(90);  // open
  } else {
    ventServo.write(0);   // closed
  }
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Smart Environment Monitor</h1>";
  html += "<p>Temperature: " + String(temperature) + " C</p>";
  html += "<p>Humidity: " + String(humidity) + " %</p>";
  html += "<p>Vent status: " + String(ventOpen ? "OPEN" : "CLOSED") + "</p>";
  html += "<a href=\"/open\"><button>Force Open Vent</button></a>";
  html += "<a href=\"/close\"><button>Force Close Vent</button></a>";
  html += "<a href=\"/auto\"><button>Return to Auto Mode</button></a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleOpen() {
  manualOverride = true;
  ventOpen = true;
  updateVent();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleClose() {
  manualOverride = true;
  ventOpen = false;
  updateVent();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleAuto() {
  manualOverride = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  ventServo.attach(13);

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

  server.on("/", handleRoot);
  server.on("/open", handleOpen);
  server.on("/close", handleClose);
  server.on("/auto", handleAuto);
  server.begin();
}

void loop() {
  server.handleClient();

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Sensor read failed");
    delay(1000);
    return;
  }

  // Automatic logic only applies if not manually overridden
  if (!manualOverride) {
    ventOpen = (temperature > tempThreshold);
    updateVent();
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temp: "); display.print(temperature); display.println(" C");
  display.print("Hum: "); display.print(humidity); display.println(" %");
  display.print("Vent: "); display.println(ventOpen ? "OPEN" : "CLOSED");
  display.print(manualOverride ? "MANUAL" : "AUTO");
  display.display();

  Serial.print("Temp: "); Serial.print(temperature);
  Serial.print(" Hum: "); Serial.print(humidity);
  Serial.print(" Vent: "); Serial.println(ventOpen ? "OPEN" : "CLOSED");

  delay(2000);
}
