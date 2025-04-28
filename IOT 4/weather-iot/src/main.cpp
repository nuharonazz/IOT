#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
String apiKey = "20ca0ff523294dcdeb424dfc5802e21b";
String city = "Malang";
String units = "metric";
String server = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&units=" + units + "&appid=" + apiKey;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Tombol
const int buttonViewPin = 12; // Tombol untuk ganti tampilan
const int buttonRefreshPin = 14; // Tombol untuk refresh data

bool showHumidity = false;
unsigned long lastUpdateTime = 0;
const long updateInterval = 60000; // 1 menit

// Data cuaca
String temp = "";
String humidity = "";
String desc = "";

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Weather Info:");

  pinMode(buttonViewPin, INPUT_PULLUP);
  pinMode(buttonRefreshPin, INPUT_PULLUP);
  
  WiFi.begin(ssid, password);
  lcd.setCursor(0, 1);
  lcd.print("Connecting...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected!");
  delay(2000);
  lcd.clear();

  getWeatherData();
  displayWeather();
}

void loop() {
  if (digitalRead(buttonViewPin) == LOW) {
    showHumidity = !showHumidity;
    displayWeather();
    delay(300); // Debounce
  }

  if (digitalRead(buttonRefreshPin) == LOW) {
    getWeatherData();
    displayWeather();
    delay(300); // Debounce
  }

  if (millis() - lastUpdateTime > updateInterval) {
    getWeatherData();
    displayWeather();
    lastUpdateTime = millis();
  }
}

void getWeatherData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(server);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);

      int tempIndex = payload.indexOf("temp");
      temp = payload.substring(tempIndex + 6, payload.indexOf(",", tempIndex));

      int humIndex = payload.indexOf("humidity");
      humidity = payload.substring(humIndex + 9, payload.indexOf(",", humIndex));

      int descIndex = payload.indexOf("description");
      desc = payload.substring(descIndex + 14, payload.indexOf("\"", descIndex + 14));
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  }
}

void displayWeather() {
  lcd.clear();
  if (showHumidity) {
    String humMsg = "Humidity: " + humidity + "%";
    if (humMsg.length() <= 16) {
      lcd.setCursor(0, 0);
      lcd.print(humMsg);
    } else {
      for (int i = 0; i <= humMsg.length() - 16; i++) {
        lcd.setCursor(0, 0);
        lcd.print(humMsg.substring(i, i + 16));
        delay(300);
      }
    }
    lcd.setCursor(0, 1);
    lcd.print(desc);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Temp: " + temp + " C");
    lcd.setCursor(0, 1);
    lcd.print(desc);
  }
}
