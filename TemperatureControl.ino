#include "DHT.h"

#define Trigger 6
#define DHTPIN 12
#define DHTTYPE DHT11

const byte Interupt = 2;
int delayTime =  0;
DHT dht(DHTPIN, DHTTYPE);

char data[33];
unsigned long lastSendTime = 0;
int temp = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(Trigger, OUTPUT);
  pinMode(Interupt, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Interupt), DELAY, FALLING);
  lastSendTime = millis();
}

void loop() {
  if (millis() - lastSendTime >= 1000) {
    temp = dht.readTemperature();
    if (isnan(temp)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      adjustFanSpeedBasedOnTemp(temp);
      sendData();
    }
    lastSendTime = millis();
  }
}

void adjustFanSpeedBasedOnTemp(int temperature) {
  if (temperature >= 40) {
    delayTime = 1050; // Fastest speed
  } else if (temperature >= 35) {
    delayTime = 4000; // Medium speed
  } else if (temperature >= 34) {
    delayTime = 5000; // Low speed
  } else {
    delayTime = 0; // Off
  }
}

void sendData() {
  sprintf(data, "{\"temp\":%d,\"delayTime\":%d}", temp, delayTime);
  Serial.println(data);
}

void DELAY() {
  if (delayTime != 1050 && delayTime != 0) {
    delayMicroseconds(delayTime);
    digitalWrite(Trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trigger, LOW);
  } else if (delayTime == 0) {
    digitalWrite(Trigger, LOW);
  } else {
    digitalWrite(Trigger, HIGH);
  }
}
