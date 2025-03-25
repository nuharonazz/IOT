#define BLYNK_TEMPLATE_ID "TEMPLESTBPXX4"
#define BLYNK_TEMPLATE_NAME "Template1"
#define BLYNK_AUTH_TOKEN "dobaBayz1984AlphaEiw818fiziusoWaaP"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHTesp.h>

// WiFi credentials
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// Pin definitions
const int DHT_PIN = 15;
const int LED_R = 26;
const int LED_Y = 27;
const int LED_G = 14;
const int LED_B = 12;

DHTesp dht;
BlynkTimer timer;

void sendSensor() {
  TempAndHumidity data = dht.getTempAndHumidity();
  
  // Check if readings are valid
  if (isnan(data.temperature) || isnan(data.humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    Blynk.virtualWrite(V0, 0);  // Send 0 if reading fails
    Blynk.virtualWrite(V1, 0);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(data.temperature, 1);  // 1 decimal place
  Serial.println(" °C");
  
  Serial.print("Humidity: ");
  Serial.print(data.humidity, 1);  // 1 decimal place
  Serial.println(" %");

  Blynk.virtualWrite(V0, data.temperature);
  Blynk.virtualWrite(V1, data.humidity);
}

// LED Control Handlers
BLYNK_WRITE(V2) {  // Red LED control
  digitalWrite(LED_R, param.asInt());
  Blynk.virtualWrite(V3, param.asInt());
}

BLYNK_WRITE(V4) {  // Yellow LED control
  digitalWrite(LED_Y, param.asInt());
}

BLYNK_WRITE(V5) {  // Green LED control
  digitalWrite(LED_G, param.asInt());
}

BLYNK_WRITE(V6) {  // Blue LED control
  digitalWrite(LED_B, param.asInt());
}

void setup() {
  Serial.begin(115200);
  delay(100);  // Short delay for serial monitor
  
  // Initialize DHT sensor
  dht.setup(DHT_PIN, DHTesp::DHT22);
  Serial.println("DHT22 sensor initialized");
  
  // Initialize LED pins
  pinMode(LED_R, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  
  // Start with LEDs off
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_Y, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);

  // Connect to Blynk
  Serial.print("Connecting to Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  while (Blynk.connected() == false) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to Blynk!");
  
  // Setup timer for sensor readings (2000ms recommended for DHT22)
  timer.setInterval(2000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}