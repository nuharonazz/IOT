#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// Pin definitions
#define DHTPIN 4         // DHT22 data pin
#define LIGHT_PIN 34     // Photoresistor pin
#define SCREEN_WIDTH 128 // OLED display width in pixels
#define SCREEN_HEIGHT 64 // OLED display height in pixels
#define OLED_RESET -1    // Reset pin (or -1 if sharing Arduino reset pin)

// Initialize DHT sensor
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Initialize OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  // Clear the display buffer
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  
  Serial.println("System initialized");
}

void loop() {
  // Read temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Read light intensity
  int lightIntensity = analogRead(LIGHT_PIN);
  int lightPercentage = map(lightIntensity, 0, 4095, 0, 100);
  
  // Check if any reads failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  } else {
    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.print(F("%  Temperature: "));
    Serial.print(temperature);
    Serial.println(F("°C"));
  }
  
  Serial.print(F("Light: "));
  Serial.print(lightPercentage);
  Serial.println(F("%"));
  
  // Clear display
  display.clearDisplay();
  display.setCursor(0,0);
  
  // Display data on OLED
  display.println("Monitoring Sensor:");
  
  display.print("Suhu: ");
  display.print(temperature, 1);
  display.println(" C");
  
  display.print("Kelembapan: ");
  display.print(humidity, 1);
  display.println(" %");
  
  display.print("Cahaya: ");
  display.print(lightPercentage);
  display.println(" %");
  
  // Update display
  display.display();
  
  // Wait before next reading
  delay(2000);
}