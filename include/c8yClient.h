
#include <Arduino.h>
#include <PubSubClient.h>

#define C8Y_DEVICE_ID "carrERAstartGrid"
#define C8Y_INIT_TOPIC "tedge_addon/init/" C8Y_DEVICE_ID "/" C8Y_DEVICE_NAME "/" C8Y_DEVICE_TYPE
#define C8Y_EVENTS_TOPIC "tedge/events/"
#define C8Y_DEVICE_NAME "Startgrid"
#define C8Y_DEVICE_TYPE "SmartStartingGrid"
#define C8Y_DEVICE_OPTIONS "c8y_Restart,c8y_Command"
#define C8Y_SUBSCRIBE_TOPIC "tedge_addon/" C8Y_DEVICE_ID "/#"
#define C8Y_COMMAND_TOPIC "tedge_addon/CarERATrack/operation/command"

void initClient(Client& aNetworkClient);
void connectC8Y(String host, int port);
void callbackHandler(char* topic, byte* payload, unsigned int length);
void publishLapEvent(int track, int nrOfLaps, float lapTimeSeconds);
void publishStartRaceEvent(int nrOfLaps);
void publishRaceFinishedEvent();
void reconnectC8Y();
void c8yLoop();