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

#ifndef _LIGHT_STATUS
#define _LIGHT_STATUS


#include "LightStatus.h"


LightStatus::LightStatus(int threshold, int blinkInterval, int analogPin)
{
	lightThreshold = threshold;
	lightBlinkInterval = blinkInterval;
	lightAnalogPin = analogPin;
}

int LightStatus::getStatus() 
{
	return lightStatus;
}

void LightStatus::checkStatus()
{
	int value = analogRead(lightAnalogPin);
	unsigned long timeStamp = millis();
	if (!lastState && value >= lightThreshold) {
		float interval = 0;
		if (lightBlinkInterval != 0) {
			interval = (float)(timeStamp - timeStampStateChange) / (float)lightBlinkInterval;
#ifdef DEBUG
            		Serial.println("----------------");
                        Serial.print("interval ");
                        Serial.print(interval);
#endif
		}
		if (interval < 1.05 && interval > 0.95) {
			lightStatus = LIGHT_BLINK;
		}
		else
		{
			lightStatus = LIGHT_ON;
		}
#ifdef DEBUG
		Serial.println("----------------");
		Serial.print("Status for ");
		Serial.println(lightAnalogPin);
		Serial.print("Status ");
		Serial.println(lightStatus);
		Serial.print("timestamp ");
		Serial.println(timeStamp);
		Serial.print("last timestamp ");
		Serial.println(timeStampStateChange);
		Serial.print("analog value ");
		Serial.println(value);
#endif
		timeStampStateChange = timeStamp;
		lastState = true;
	}

	if (lastState && value < lightThreshold) {
		float interval = 0;
		if (lightBlinkInterval != 0) {
			interval = (float)(timeStamp - timeStampStateChange) / (float)lightBlinkInterval;
#ifdef DEBUG
            		Serial.println("----------------");
                        Serial.print("interval ");
                        Serial.print(interval);
#endif
		}
		if (interval < 1.05 && interval > 0.95) {
			lightStatus = LIGHT_BLINK;
		}
		else
		{
			lightStatus = LIGHT_OFF;
		}
#ifdef DEBUG
		Serial.println("----------------");
		Serial.print("Status for ");
		Serial.println(lightAnalogPin);
		Serial.print("Status ");
		Serial.println(lightStatus);
		Serial.print("timestamp ");
		Serial.println(timeStamp);
		Serial.print("last timestamp ");
		Serial.println(timeStampStateChange);
		Serial.print("analog value ");
		Serial.println(value);
#endif
		timeStampStateChange = timeStamp;
		lastState = false;
	}

	if (LIGHT_BLINK && timeStamp > timeStampStateChange + (float)lightBlinkInterval * 1.1)
	{
		if (value >= lightThreshold) {
			lightStatus = LIGHT_ON;
			lastState = true;
		}
		else
		{
			lightStatus = LIGHT_OFF;
			lastState = false;
		}
		timeStampStateChange = timeStamp;
	}
}
#endif
