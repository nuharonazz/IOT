#define BLYNK_TEMPLATE_ID "TMPL6rvAUy0un"
#define BLYNK_TEMPLATE_NAME "ESP32 Smart Lamp"
#define BLYNK_AUTH_TOKEN "amPtC1k8Jzd3jH_dOdmB4giT16cjcVc2"

#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define PIR_PIN 15
#define LDR_PIN 33
#define LED_PIN 13

bool ledState = false;
bool manualControl = false;  // Untuk membedakan kontrol manual dan otomatis

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

BlynkTimer timer;

// Fungsi membaca sensor dan kontrol otomatis
void checkSensors() {
  if (manualControl) return;  // Jika mode manual aktif, sensor tidak mengontrol LED

  int pirState = digitalRead(PIR_PIN);
  int ldrValue = analogRead(LDR_PIN);

  Serial.print("📈 PIR: ");
  Serial.print(pirState);
  Serial.print(" | ☀️ LDR (lux proxy): ");
  Serial.println(ldrValue);

  // nyalakan lampu jika gelap (LDR > 2000) atau ada gerakan
  bool newLedState = (pirState == HIGH || ldrValue > 2000);

  if (newLedState != ledState) {
    Serial.print("💡 Status Lampu: ");
    Serial.println(newLedState ? "MENYALA" : "MATI");

    if (newLedState) {
      Serial.println("🔆 Lampu MENYALA");
      if (pirState == HIGH) Serial.println("➡️ Penyebab: Gerakan terdeteksi oleh PIR!");
      if (ldrValue > 2000) Serial.println("➡️ Penyebab: Cahaya redup, sensor LDR aktif!");
    } else {
      Serial.println("🌙 Lampu MATI");
    }

    ledState = newLedState;
    digitalWrite(LED_PIN, newLedState ? HIGH : LOW);
    Blynk.virtualWrite(V1, newLedState);  // Kirim status LED ke Blynk (V1)
  }
}

// Kontrol LED dari Blynk (V0)
BLYNK_WRITE(V0) {
  int ledControl = param.asInt();
  manualControl = (ledControl == 0 || ledControl == 1);  // Aktifkan mode manual jika tombol ditekan

  digitalWrite(LED_PIN, ledControl);
  ledState = ledControl;

  Serial.print("🖲️ LED dikontrol manual dari Blynk: ");
  Serial.println(ledControl ? "ON" : "OFF");

  Blynk.virtualWrite(V1, ledControl);  // Kirim status LED ke Blynk
}

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid, pass);
  Serial.print("📡 Menghubungkan ke WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" Terhubung!");

  Blynk.config(auth);
  Blynk.connect();  // Hubungkan ke Blynk

  timer.setInterval(1000L, checkSensors);  // Cek sensor tiap 1 detik
}

void loop() {
  Blynk.run();
  timer.run();
}
