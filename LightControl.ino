#define Trigger 9
#define PIR 6
#define LDR A2

const byte Interupt = 2;
int delayTime =  0; // DelayTime values are 8500, 1050, 0

char data[33];
unsigned long lastSendTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(Trigger, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(LDR, INPUT);
  pinMode(Interupt, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Interupt), DELAY, FALLING);
}

void loop() {
  lightEffect();
  sendData();
}

void sendData() {
  if (millis() - lastSendTime >= 1000) {
    sprintf(data, "{\"lux\":%d,\"mot\":%d}", analogRead(LDR), digitalRead(PIR));
    Serial.println(data);
    lastSendTime = millis();
  }
}

void DELAY() {
  digitalWrite(Trigger, delayTime != 0);
  if (delayTime > 0 && delayTime != 1050) {
    delayMicroseconds(delayTime);
    digitalWrite(Trigger, LOW);
  }
}

void lightEffect() {
  if (analogRead(LDR) <  1000) {
    delayTime = 8500;
    if (digitalRead(PIR) == 1) {
      delayTime = 1050;
    }
  } else {
    delayTime = 0;
  }
}
