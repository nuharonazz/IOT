#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Settings
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Weather API Settings
String apiKey = "20ca0ff523294dcdeb424dfc5802e21b";
String city = "Malang";
String units = "metric";
String server = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&units=" + units + "&appid=" + apiKey;

// LCD Settings
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Weather Info:");
  
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
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(server);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);

      // Parsing manual
      int tempIndex = payload.indexOf("temp");
      String temp = payload.substring(tempIndex + 6, payload.indexOf(",", tempIndex));
      
      int humIndex = payload.indexOf("humidity");
      String humidity = payload.substring(humIndex + 9, payload.indexOf(",", humIndex));
      
      int descIndex = payload.indexOf("description");
      String desc = payload.substring(descIndex + 14, payload.indexOf("\"", descIndex + 14));

      // Display suhu
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp: " + temp + " C");
      lcd.setCursor(0, 1);
      lcd.print(desc);
      delay(5000);

      // Display kelembaban dengan scroll kalau panjang
      lcd.clear();
      String humMsg = "Humidity: " + humidity + "%";
      for (int i = 0; i <= humMsg.length() - 16; i++) {
        lcd.setCursor(0, 0);
        lcd.print(humMsg.substring(i, i + 16));
        delay(300);
      }

      lcd.setCursor(0, 1);
      lcd.print(desc);
      delay(5000);

    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  }
  delay(60000); // Update tiap 1 menit
}
