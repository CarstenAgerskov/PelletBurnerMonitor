/*
    PBMonitor: Monitor a Pellet Burner
    Copyright (C) 2015  Carsten Agerskov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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