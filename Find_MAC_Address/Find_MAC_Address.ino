#include <WiFi.h>

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);   // VERY IMPORTANT
  delay(1000);           // give time to init WiFi

  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {}
