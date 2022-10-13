#include "button.h"
#include "lapCounter.h"
#include "ledControl.h"

#define ROTARY_CLK_PIN 5 // D1
#define ROTARY_DT_PIN  4 // D2
#define ROTARY_BUTTON  3 // RX D0

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

void setupButton() {
    // Set encoder pins as inputs
	pinMode(ROTARY_CLK_PIN, INPUT);
	pinMode(ROTARY_DT_PIN, INPUT);
	pinMode(ROTARY_BUTTON, INPUT_PULLUP);

    lastStateCLK = digitalRead(ROTARY_CLK_PIN);
}

void loopButton() {

    // Read the current state of CLK
	currentStateCLK = digitalRead(ROTARY_CLK_PIN);

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(ROTARY_DT_PIN) != currentStateCLK && counter >= 1) {
            disableLed(--counter);
			//counter --;
			currentDir ="CCW";
		} else {
			// Encoder is rotating CW so increment
			//counter ++;
            enableLed(counter++);
			currentDir ="CW";
		}
        Serial.printf("turn the knob %s, count is now %i\n", currentDir, counter);
	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;

	// Read the button state
	int btnState = digitalRead(ROTARY_BUTTON);

	//If we detect LOW signal, button is pressed
	if (btnState == LOW) {
		//if 50ms have passed since last LOW pulse, it means that the
		//button has been pressed, released and pressed again
		if (millis() - lastButtonPress > 50) {
			blankLeds();
            startRace(counter);
            counter = 0;
            lastStateCLK = digitalRead(ROTARY_CLK_PIN);
        }
        // Remember last button press event
        lastButtonPress = millis();
	}
}