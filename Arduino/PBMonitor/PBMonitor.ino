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


#include "FilterStatus.h"

#define LED_PIN 13

#define ALARM_PIN 2
#define SHUT_DOWN_PIN 3

#define THRESHOLD_STANDBY 440
#define THRESHOLD_START 420
#define THRESHOLD_RUN 450
#define TIME_THRESHOLD 10000

bool standByOn = false;
bool startupOn = false;
bool runOn = false;
bool alarmOn = false;
int alarmValue;
int shutDownValue;

FilterStatus run = FilterStatus(THRESHOLD_RUN, 0, A0, TIME_THRESHOLD);
FilterStatus startup = FilterStatus(THRESHOLD_START, 0, A1, TIME_THRESHOLD);
FilterStatus standBy = FilterStatus(THRESHOLD_STANDBY, 500, A2, TIME_THRESHOLD);

void setup() {
	Serial.begin(9600);
	pinMode(LED_PIN, OUTPUT);
	pinMode(ALARM_PIN, INPUT_PULLUP);
	pinMode(SHUT_DOWN_PIN, INPUT_PULLUP);
	Serial.println("Burner monitor restarting");
	standBy.checkStatus();
	startup.checkStatus();
	run.checkStatus();
	standBy.getStatus();
	startup.getStatus();
	run.getStatus();
	delay(TIME_THRESHOLD+100);
	Serial.println("Burner monitor running");
}

void loop() {
	standBy.checkStatus();
	startup.checkStatus();
	run.checkStatus();

	if (standByOn) {
		if (standBy.getStatus() == LIGHT_BLINK) {
			Serial.print("('Start', ");
			Serial.print(standBy.getStatusTimeStamp());
			Serial.println(")");
			standByOn = false;
		}
	}
	else {
		if (standBy.getStatus() != LIGHT_BLINK && startup.getStatus() != LIGHT_ON) {
			Serial.print("('Standby', ");
			if( startup.getStatusTimeStamp() > standBy.getStatusTimeStamp() ) {
				Serial.print(startup.getStatusTimeStamp());
			}
			else  {
				Serial.print(standBy.getStatusTimeStamp());
			}
			Serial.println(")");
			standByOn = true;
		}
	}

	if (startupOn) {
		if (startup.getStatus() != LIGHT_ON) {
			Serial.print("('NoLowPower', ");
			Serial.print(startup.getStatusTimeStamp());
			Serial.println(")");
			startupOn = false;
		}
	}
	else {
		if (startup.getStatus() == LIGHT_ON) {
			Serial.print("('LowPower', ");
			Serial.print(startup.getStatusTimeStamp());
			Serial.println(")");
			startupOn = true;
		}
	}

	if (runOn) {
		if (run.getStatus() != LIGHT_ON) {
			Serial.print("('NoHighPower', ");
			Serial.print(run.getStatusTimeStamp());
			Serial.println(")");
			runOn = false;
		}
	}
	else {
		if (run.getStatus() == LIGHT_ON) {
			Serial.print("('HighPower', ");
			Serial.print(run.getStatusTimeStamp());
			Serial.println(")");
			runOn = true;
		}
	}

	alarmValue = digitalRead(ALARM_PIN);
	if (alarmOn) {
		if (alarmValue == LOW) {
			digitalWrite(LED_PIN, LOW);
			Serial.print("('Alarm', 'off', ");
			Serial.print(millis());
			Serial.println(")");
			alarmOn = false;
		}
	}
	else {
		if (alarmValue == HIGH) {
			digitalWrite(LED_PIN, HIGH);
			Serial.print("('Alarm', 'on', ");
			Serial.print(millis());
			Serial.println(")");
			alarmOn = true;
		}
	}

	shutDownValue = digitalRead(SHUT_DOWN_PIN);
	if (shutDownValue == LOW) {
		Serial.print("('PBMonShutDown', 'now', ");
		Serial.print(millis());
		Serial.println(")");
	}

}

