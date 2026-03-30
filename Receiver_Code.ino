#include <WiFi.h>
#include <esp_now.h>

// Structure
typedef struct struct_message {
  char command;
} struct_message;

struct_message incomingData;

// Speed
int Speed = 230;

// PWM channels
#define R 0
#define L 1

// PWM pins
int enA = 5;
int enB = 23;

// Motor pins
int IN1 = 22;
int IN2 = 21;
int IN3 = 19;
int IN4 = 18;

// Callback
void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  char cmd = incomingData.command;

  Serial.print("Received: ");
  Serial.println(cmd);

  if (cmd == 'F') forward();
  else if (cmd == 'B') backward();
  else if (cmd == 'L') left();
  else if (cmd == 'R') right();
  else if (cmd == 'S') stop();
  else if (cmd == 'G') forwardLeft();
  else if (cmd == 'I') forwardRight();
  else if (cmd == 'H') backwardLeft();
  else if (cmd == 'J') backwardRight();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  ledcSetup(R, 5000, 8);
  ledcAttachPin(enA, R);

  ledcSetup(L, 5000, 8);
  ledcAttachPin(enB, L);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stop();
}

void loop() {}


// ===== MOTOR FUNCTIONS =====

void forward() {
  ledcWrite(R, Speed);
  ledcWrite(L, Speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void backward() {
  ledcWrite(R, Speed);
  ledcWrite(L, Speed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void left() {
  ledcWrite(R, Speed);
  ledcWrite(L, Speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right() {
  ledcWrite(R, Speed);
  ledcWrite(L, Speed);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Diagonal

void forwardLeft() {
  ledcWrite(R, 0);
  ledcWrite(L, Speed);
  forward();
}

void forwardRight() {
  ledcWrite(R, Speed);
  ledcWrite(L, 0);
  forward();
}

void backwardLeft() {
  ledcWrite(R, 0);
  ledcWrite(L, Speed);
  backward();
}

void backwardRight() {
  ledcWrite(R, Speed);
  ledcWrite(L, 0);
  backward();
}

void stop() {
  ledcWrite(R, 0);
  ledcWrite(L, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}