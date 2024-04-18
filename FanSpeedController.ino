#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char *ssid = "dbnet.net";
const char *password = "dbnet-2024";

char ser_data[65]; // Buffer to store serial data

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(D0, INPUT); // Changed from 0 to D0 for clarity
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);
    delay(150);
  }
  ser_data[0] = '\0'; // Ensure string is properly terminated
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(150);
      digitalWrite(LED_BUILTIN, LOW);
      delay(150);
    }
  }

  if (Serial.available()) {
    int i = 0;
    while (Serial.available()) {
      delay(3); // Ensure all data is read
      ser_data[i++] = Serial.read();
      if (i >= 64) break; // Prevent buffer overflow
    }
    ser_data[i] = '\0'; // Ensure string is properly terminated
    ping_server(ser_data);
  }
}

int ping_server(const char *val) {
  HTTPClient http;
  WiFiClient client;
  char url[90];
  sprintf(url, "http://192.168.4.1/ping?name=dhtc&val=%s", val);
  http.begin(client, url);
  int httpCode = http.GET();
  http.end();
  return httpCode;
}
