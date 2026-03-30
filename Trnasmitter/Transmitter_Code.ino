#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Receiver MAC Address
uint8_t receiverMAC[] = {0x88, 0x57, 0x21, 0xB6, 0x55, 0x38};

typedef struct struct_message {
  char command;
} struct_message;

struct_message dataToSend;
esp_now_peer_info_t peerInfo;

float ax, ay;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  // ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  // MPU FIX
  Wire.begin(21, 22);
  mpu.initialize();

  Serial.println("MPU6050 Ready!");
}

void loop() {

  int16_t rawAx, rawAy, rawAz;
  mpu.getAcceleration(&rawAx, &rawAy, &rawAz);

  ax = rawAx / 16384.0;
  ay = rawAy / 16384.0;

  char command = 'S';

  // Dead zone (no movement)
  if (abs(ax) < 0.15 && abs(ay) < 0.15) {
   command = 'S';
  }
  else if (ay > 0.3) command = 'F';
  else if (ay < -0.3) command = 'B';
  else if (ax > 0.3) command = 'R';
  else if (ax < -0.3) command = 'L';

  sendCommand(command);

  Serial.print("AX: ");
  Serial.print(ax);
  Serial.print(" AY: ");
  Serial.print(ay);
  Serial.print(" -> ");
  Serial.println(command);

  delay(100);
}

void sendCommand(char cmd) {
  dataToSend.command = cmd;
  esp_now_send(receiverMAC, (uint8_t *)&dataToSend, sizeof(dataToSend));
}
