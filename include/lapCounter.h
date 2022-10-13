#ifndef LAP_COUNTER_H
#define LAP_COUNTER_H

#define DETECT_PIN_1 13 //D7 
#define DETECT_PIN_2 12 //D6

struct LapDetails {
    int lane;
    int lapNumber;
    long lapTime;
};

IRAM_ATTR void carLane1Passed();
IRAM_ATTR void carLane2Passed();

boolean isTriggeredLane1();
boolean isTriggeredLane2();

LapDetails countLap(int lane);

void startRace(int nrOfLaps);
boolean checkRaceFinished();
void track1Dnf();
void track2Dnf();
void raceFinished();
//void endRace();

#endif