#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "lapCounter.h"
#include "ledControl.h"
#include "c8yClient.h"
#include "config.h"
#include "button.h"

//connect to a WiFi access point
void connectWifi() {
  WiFi.begin(ssid, wifiPassword);

  Serial.print("Connecting to WiFi");
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (tries++ > 10) {
      WiFi.begin(ssid, wifiPassword);
      tries = 0;
    }

  }
  Serial.println("connected to wifi");
}

void setup() {
  Serial.begin(115200);

  setupButton();

  connectWifi();

  Serial.println(WiFi.localIP());
  connectC8Y(host, 1883);

  init_leds();

  blankLeds();
  
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost");
    connectWifi();
  }
  
  c8yLoop();

  if (isTriggeredLane1()) {
    LapDetails myLap = countLap(0);
    if (myLap.lapNumber < 6) {
      enableLed(myLap.lapNumber -1 );
    }
    publishLapEvent(1, myLap.lapNumber, myLap.lapTime/1000.f);

    if (checkRaceFinished()) {
      raceFinished();
    }
  }

  if (isTriggeredLane2()) {
    LapDetails myLap = countLap(1);

    if (myLap.lapNumber < 6) {
      enableLed(N_LEDS - myLap.lapNumber);
    }
    publishLapEvent(2, myLap.lapNumber, myLap.lapTime/1000.f);

    if (checkRaceFinished()) {
      raceFinished();
    }
  }

  loopButton();

  delay(5);
}