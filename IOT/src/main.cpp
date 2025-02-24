#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

  void setup() {
    // Inisialisasi komunikasi serial
    Serial.begin(115200);
    Serial.println("Hello, ESP32!");
  
    // Atur pin sebagai output
    pinMode(23, OUTPUT);
    pinMode(22, OUTPUT);
    pinMode(21, OUTPUT);
  }
  
  void loop() {
    // Nyalakan pin 23 selama 30 detik
    digitalWrite(23, HIGH);
    delay(30000);
    digitalWrite(23, LOW);
  
    // Nyalakan pin 22 selama 5 detik
    digitalWrite(22, HIGH);
    delay(5000);
    digitalWrite(22, LOW);
  
    // Nyalakan pin 21 selama 20 detik
    digitalWrite(21, HIGH);
    delay(20000);
    digitalWrite(21, LOW);
  }