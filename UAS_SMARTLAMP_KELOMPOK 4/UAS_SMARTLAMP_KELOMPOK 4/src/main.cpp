#define BLYNK_TEMPLATE_ID "TMPL6rvAUy0un"
#define BLYNK_TEMPLATE_NAME "ESP32 Smart Lamp"
#define BLYNK_AUTH_TOKEN "amPtC1k8Jzd3jH_dOdmB4giT16cjcVc2"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define PIR_PIN 17
#define LED_PIN 21

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST";  // WiFi bawaan wokwi
char pass[] = "";             // Wokwi tidak perlu password

BlynkTimer timer;
bool manualLampState = false;
bool motionDetected = false;
unsigned long lastMotionTime = 0;
const unsigned long lampDelay = 5000; // 5 detik nyala setelah gerakan

// Fungsi: Update status lampu
void updateLampStatus() {
  bool lampuNyala = manualLampState || (millis() - lastMotionTime < lampDelay);

  // Fisik: LED
  digitalWrite(LED_PIN, lampuNyala ? HIGH : LOW);

  // Widget LED (V1)
  Blynk.virtualWrite(V1, lampuNyala ? 255 : 0);

  // Label V3
  if (lampuNyala) {
    if (manualLampState) {
      Blynk.virtualWrite(V3, "Lampu NYALA (Manual)");
    } else {
      Blynk.virtualWrite(V3, "Lampu NYALA (Gerakan)");
    }
  } else {
    Blynk.virtualWrite(V3, "Lampu MATI");
  }
}

// Fungsi: Cek sensor PIR
void checkMotion() {
  int pirState = digitalRead(PIR_PIN);

  if (pirState == HIGH) {
    lastMotionTime = millis(); // Simpan waktu terakhir gerakan
    motionDetected = true;
  } else {
    motionDetected = false;
  }

  Serial.print("PIR: ");
  Serial.print(pirState);
  Serial.print(" | Gerakan? ");
  Serial.println(motionDetected ? "YA" : "TIDAK");

  updateLampStatus();
}

// Event: Switch manual Blynk (V0)
BLYNK_WRITE(V0) {
  int value = param.asInt();
  manualLampState = (value == 1);

  Serial.print("Kontrol manual dari Blynk: ");
  Serial.println(manualLampState ? "Lampu NYALA (Manual)" : "Lampu MATI (Manual)");

  updateLampStatus();
}

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(500L, checkMotion); // Cek lebih sering, tiap 0.5 detik

  Serial.println("Monitoring PIR dimulai...");
}

void loop() {
  Blynk.run();
  timer.run();
}
