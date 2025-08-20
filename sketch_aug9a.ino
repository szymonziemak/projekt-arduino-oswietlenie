#include <WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

#define WIFI_SSID         ""
#define WIFI_PASS         ""
#define APP_KEY           ""
#define APP_SECRET        ""
#define SWITCH_ID         ""

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
