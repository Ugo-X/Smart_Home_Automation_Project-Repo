#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char *ssid = "dbnet.net";
const char *password = "dbnet-2024";

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(D0, INPUT);
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);
    delay(150);
  }
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

  if (digitalRead(D0)) { // Triggering security event
    while (digitalRead(D0)) {
      ping_server(1);
      delay(1000); // Sending active signal every second
    }
    ping_server(0); // Sending deactivation signal
  }
}

int ping_server(int val) {
  HTTPClient http;
  WiFiClient client;
  char url[90];
  sprintf(url, "http://192.168.4.1/ping?name=pirs&val=%d", val);
  http.begin(client, url);
  int httpCode = http.GET();
  http.end();
  return httpCode;
}
