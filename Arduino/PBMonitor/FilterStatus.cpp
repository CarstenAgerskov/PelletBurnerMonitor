#ifndef _FILTER_STATUS
#define _FILTER_STATUS
#include <Arduino.h>
#include "FilterStatus.h"


FilterStatus::FilterStatus(int threshold, int blinkInterval, int analogPin, unsigned long t) : LightStatus(threshold, blinkInterval, analogPin)
{
	confirmTimeThreshold = t;
}

unsigned long FilterStatus::getStatusTimeStamp() {
	return stateTimeStamp;
}


int FilterStatus::getStatus() {
	unsigned long timeStamp = millis();
	int tmpState = LightStatus::getStatus();
	if (tentativeState != tmpState) {
		tentativeStateTimeStamp = timeStamp;
		tentativeState = tmpState;
	}

	if (state != tentativeState && timeStamp - tentativeStateTimeStamp >= confirmTimeThreshold) {
		state = tentativeState;
		stateTimeStamp = tentativeStateTimeStamp;
#ifdef DEBUG
		Serial.print("FilterStatus state: ");
		Serial.println(state);
		Serial.print("FilterStatus time: ");
		Serial.println(stateTimeStamp);
#endif
	}

	return state;
}

#endif