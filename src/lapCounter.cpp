#include <Arduino.h>
#include "lapCounter.h"
#include "ledControl.h"
#include "c8yClient.h"

int laps = 0;
long triggerTime[2] = {0,0};
long previousTriggerTime[2] = {0,0};
long lapTime[2] = {0,0};
boolean triggeredLane1, triggeredLane2, isRaceFinished = false;
int lapCounter[2] = {0,0};

IRAM_ATTR void carLane1Passed() {
  triggerTime[0] = millis();
  long elapsedTime = triggerTime[0] - previousTriggerTime[0];
  if (elapsedTime != triggerTime[0] && elapsedTime > 1000) {
    previousTriggerTime[0] = triggerTime[0];
    lapTime[0] = elapsedTime;
    triggeredLane1 = true;
  }
}

IRAM_ATTR void carLane2Passed() {
  triggerTime[1] = millis();
  long elapsedTime = triggerTime[1] - previousTriggerTime[1];
  if (elapsedTime != triggerTime[1] && elapsedTime > 1000) {
    previousTriggerTime[1] = triggerTime[1];
    lapTime[1] = elapsedTime;
    triggeredLane2 = true;
  }
}

void startRace(int nrOfLaps) {
  
  laps = nrOfLaps;

  countdown();
  //Start Counter
  long myTiming = millis();
  isRaceFinished = false;
  lapCounter[0] = 0;
  lapCounter[1] = 0;
  previousTriggerTime[0] = myTiming;
  previousTriggerTime[1] = myTiming;
  publishStartRaceEvent(laps);

  delay(2000);
  blankLeds();
  delay(1000);
  attachInterrupt(digitalPinToInterrupt(DETECT_PIN_1), carLane1Passed, FALLING);
  attachInterrupt(digitalPinToInterrupt(DETECT_PIN_2), carLane2Passed, FALLING);
}

void track1Dnf() {
  Serial.println("Car 1 did not finish!");
  lapCounter[0] = -1;
  if (lapCounter[1] == laps || lapCounter[1] == -1) {
    raceFinished();
  }
}

void track2Dnf() {
  lapCounter[1] = -1;
  if (lapCounter[0] == laps || lapCounter[0] == -1) {
    raceFinished();
  }
}

boolean isTriggeredLane1() {
  return triggeredLane1;
}

boolean isTriggeredLane2() {
  return triggeredLane2;
}

boolean checkRaceFinished() {
  return isRaceFinished;
}

void raceFinished() {
  Serial.println("Race finished");
  blankLeds();
  publishRaceFinishedEvent();
}

LapDetails countLap(int lane) {
  
  LapDetails myLap;

  if (lapCounter[lane] != -1) {
    lapCounter[lane] += 1;
  }

  myLap.lane = lane;
  myLap.lapNumber = lapCounter[lane];
  myLap.lapTime = lapTime[lane];

  if (lane == 0) {
    triggeredLane1 = false;
    if (myLap.lapNumber == laps) {
      detachInterrupt(digitalPinToInterrupt(DETECT_PIN_1));
      if (lapCounter[1] == laps || lapCounter[1] == -1) {
        //Race finished
        isRaceFinished = true;
      }
    }
  } else {
    triggeredLane2 = false;
    if (myLap.lapNumber == laps) {
      detachInterrupt(digitalPinToInterrupt(DETECT_PIN_2));
      if (lapCounter[0] == laps || lapCounter[0] == -1) {
        //Race finished
        isRaceFinished = true;
      }
    }
  }

  return myLap;
}