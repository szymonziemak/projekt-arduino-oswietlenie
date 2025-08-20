#include <WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

#define WIFI_SSID         "TP-Link_54CF"
#define WIFI_PASS         "75216627"
#define APP_KEY           "3e1fe396-32cf-4584-a4e1-27beeedf0cc7"
#define APP_SECRET        "24065007-9cd1-4b5e-8480-519dddc6df3b-8d206304-af48-4131-8319-1ceb3bbe2348"
#define SWITCH_ID         "68975b82678c5bc9ab28118c"

#define RELAY_PIN         14 // Przykładowy pin, dostosuj do swojego połączenia

bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("Urządzenie %s - stan: %s\r\n", deviceId.c_str(), state ? "WŁĄCZONE" : "WYŁĄCZONE");
  digitalWrite(RELAY_PIN, state ? HIGH : LOW);
  return true;
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Upewnij się, że lampka jest wyłączona na start

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Połączono z WiFi!");

  SinricPro.onConnected([](){ Serial.println("Połączono z Sinric Pro"); });
  SinricPro.onDisconnected([](){ Serial.println("Rozłączono z Sinric Pro"); });

  SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];
  mySwitch.onPowerState(onPowerState);
  
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void loop() {
  SinricPro.handle();
}