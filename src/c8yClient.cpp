#include "c8yClient.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "lapCounter.h"

WiFiClient wifiClient;
PubSubClient client = PubSubClient(wifiClient);

void connectC8Y(String aHost, int aPort) {

    client.setServer(aHost.c_str(), aPort);
    client.setCallback(callbackHandler);

    if (!client.connected()) {
        
        client.connect(C8Y_DEVICE_ID);
    }

    Serial.printf("subscribing on topic %s\n", C8Y_COMMAND_TOPIC);
    boolean succeeded = client.subscribe(C8Y_COMMAND_TOPIC);
    Serial.printf("Did subscription succeed? %d", succeeded);

    client.publish(C8Y_INIT_TOPIC, C8Y_DEVICE_OPTIONS);
}

void reconnectC8Y() {
    if (!client.connected()) {
        Serial.println("Connection to MQTT broker lost, reconnecting....");
        client.connect(C8Y_DEVICE_ID);
    }
}

void publishLapEvent(int track, int nrOfLaps, float lapTimeSeconds) {

    DynamicJsonDocument event(1024);

    event["text"] = "Lap timed";
    event["track"] = track;
    event["lap"] = nrOfLaps;
    event["lapTime"] = lapTimeSeconds;

    String eventJson;
    serializeJson(event, eventJson);

    String topic = "tedge/events/lap_timing/carrERAstartGrid";
    //topic += C8Y_DEVICE_ID;
    //String topic = C8Y_EVENTS_TOPIC + String("lap_timing/") + C8Y_DEVICE_ID;

    Serial.println("publishing to " + topic);

    reconnectC8Y();
    client.publish(topic.c_str(), eventJson.c_str());
}

void publishStartRaceEvent(int nrOfLaps) {

    DynamicJsonDocument event(1024);

    event["text"] = "Race started";
    event["Number of laps"] = nrOfLaps;

    String eventJson;
    serializeJson(event, eventJson);
    
    
    String topic = "tedge/events/race_started/carrERAstartGrid";
    //topic += C8Y_DEVICE_ID;
    //String topic = C8Y_EVENTS_TOPIC + String("race_started/") + C8Y_DEVICE_ID;
    Serial.println("publishing to " + topic);

    reconnectC8Y();
    client.publish(topic.c_str(), eventJson.c_str());
}

void publishRaceFinishedEvent() {

    DynamicJsonDocument event(1024);

    event["text"] = "Race ended";

    String eventJson;
    serializeJson(event, eventJson);
   //String topic = C8Y_EVENTS_TOPIC + String("race_finished/") + C8Y_DEVICE_ID;
    String topic = C8Y_EVENTS_TOPIC;
    topic += "race_finished/";
    topic += C8Y_DEVICE_ID;
    Serial.println("publishing to " + topic);

    reconnectC8Y();
    client.publish(topic.c_str(), eventJson.c_str());
}

void callbackHandler(char* topic, byte* payload, unsigned int length) {

    Serial.printf("message received on topic %s\n", topic);
    if (strcmp(topic, "tedge_addon/CarERATrack/operation/command")==0) {
        Serial.println("Checking if command is powerbudget off");
        char* myPayload = (char*)payload;
        if (strncmp(myPayload, "Track1Power;0",12) ==0) {
            Serial.println("track 1 budget depleted");
            track1Dnf();
        } else if (strncmp(myPayload, "Track2Power;0",12) == 0 ) {
            Serial.println("track 2 budget depleted");
            track2Dnf();
        }
    }
}

void c8yLoop() {
    client.loop();
}