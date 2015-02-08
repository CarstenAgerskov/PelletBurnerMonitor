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

#include <SerialCommand.h> // An Arduino library to tokenize and parse commands received over a serial port. by Steven Cogswell
#include <SoftwareSerial.h>
#include "FilterStatus.h"

#define LED_PIN 13

#define ALARM_PIN 2
#define SHUT_DOWN_PIN 3

#define THRESHOLD_STANDBY 400
#define THRESHOLD_LOW_POWER 400
#define THRESHOLD_HIGH_POWER 400
#define TIME_THRESHOLD 10000
#define BLINK_INTERVAL 500

bool standByOn = false;
bool lowPowerOn = false;
bool highPowerOn = false;
bool alarmOn = false;
int alarmValue;
int shutDownValue;

SerialCommand sCmd;

FilterStatus highPower = FilterStatus(THRESHOLD_HIGH_POWER, 0, A0, TIME_THRESHOLD);
FilterStatus lowPower = FilterStatus(THRESHOLD_LOW_POWER, 0, A1, TIME_THRESHOLD);
FilterStatus standBy = FilterStatus(THRESHOLD_STANDBY, BLINK_INTERVAL, A2, TIME_THRESHOLD);

void setup() {
	Serial.begin(9600);
	pinMode(LED_PIN, OUTPUT);
	pinMode(ALARM_PIN, INPUT_PULLUP);
	pinMode(SHUT_DOWN_PIN, INPUT_PULLUP);
	pinMode(5, OUTPUT);
	digitalWrite(A0, HIGH);
	digitalWrite(A1, HIGH);
	digitalWrite(A2, HIGH);
	Serial.println("Burner monitor restarting");
	standBy.checkStatus();
	lowPower.checkStatus();
	highPower.checkStatus();
	standBy.getStatus();
	lowPower.getStatus();
	highPower.getStatus();
	sCmd.addCommand("setStandByThreshold", setStandByThreshold);
	sCmd.addCommand("setLowPowerThreshold", setLowPowerThreshold);
	sCmd.addCommand("setHighPowerThreshold", setHighPowerThreshold);
	sCmd.addCommand("status", status);
	sCmd.addDefaultHandler(unrecognized);     
	delay(TIME_THRESHOLD+100);
	Serial.println("Burner monitor running");
}

void loop() {
	sCmd.readSerial();
	standBy.checkStatus();
	lowPower.checkStatus();
	highPower.checkStatus();

	if (standByOn) {
		if (standBy.getStatus() == LIGHT_BLINK) {
			Serial.print("('Start', ");
			Serial.print(standBy.getStatusTimeStamp());
			Serial.println(")");
			standByOn = false;
		}
	}
	else {
		if (standBy.getStatus() != LIGHT_BLINK && lowPower.getStatus() != LIGHT_ON) {
			Serial.print("('Standby', ");
			if( lowPower.getStatusTimeStamp() > standBy.getStatusTimeStamp() ) {
				Serial.print(lowPower.getStatusTimeStamp());
			}
			else  {
				Serial.print(standBy.getStatusTimeStamp());
			}
			Serial.println(")");
			standByOn = true;
		}
	}

	if (lowPowerOn) {
		if (lowPower.getStatus() != LIGHT_ON) {
			Serial.print("('NoLowPower', ");
			Serial.print(lowPower.getStatusTimeStamp());
			Serial.println(")");
			lowPowerOn = false;
		}
	}
	else {
		if (lowPower.getStatus() == LIGHT_ON) {
			Serial.print("('LowPower', ");
			Serial.print(lowPower.getStatusTimeStamp());
			Serial.println(")");
			lowPowerOn = true;
		}
	}

	if (highPowerOn) {
		if (highPower.getStatus() != LIGHT_ON) {
			Serial.print("('NoHighPower', ");
			Serial.print(highPower.getStatusTimeStamp());
			Serial.println(")");
			highPowerOn = false;
		}
	}
	else {
		if (highPower.getStatus() == LIGHT_ON) {
			Serial.print("('HighPower', ");
			Serial.print(highPower.getStatusTimeStamp());
			Serial.println(")");
			highPowerOn = true;
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

void setHighPowerThreshold() {
	char *arg;
	arg = sCmd.next();    
	if (arg != NULL) {    
		int t = atoi(arg);
		Serial.print("HighPower threshold: ");
		Serial.println(t);
		highPower.setThreshold(t);
	}
	else {
		Serial.println("You must specify a threshold");
	}
}

void setLowPowerThreshold() {
	char *arg;
	arg = sCmd.next();
	if (arg != NULL) {
		int t = atoi(arg);
		Serial.print("LowPower threshold: ");
		Serial.println(t);
		lowPower.setThreshold(t);
	}
	else {
		Serial.println("You must specify a threshold");
	}
}

void setStandByThreshold() {
	char *arg;
	arg = sCmd.next();
	if (arg != NULL) {
		int t = atoi(arg);
		Serial.print("StandBy threshold: ");
		Serial.println(t);
		standBy.setThreshold(t);
	}
	else {
		Serial.println("You must specify a threshold");
	}
}

void status() {
	Serial.print("StandBy sample value: ");
	Serial.println(standBy.getValue());
	Serial.print("LowPower sample value: ");
	Serial.println(lowPower.getValue());
	Serial.print("HighPower sample value: ");
	Serial.println(highPower.getValue());
	Serial.print("StandBy status: ");
	Serial.println(standBy.getStatus());
	Serial.print("LowPower status: ");
	Serial.println(lowPower.getStatus());
	Serial.print("HighPower status: ");
	Serial.println(highPower.getStatus());
}

void unrecognized() {
	Serial.println("Commands:");
	Serial.println("status - Show input status");
	Serial.println("setStandByThreshold t - Set threshold for stand by");
	Serial.println("setLowPowerThreshold t - Set threshold for low power");
	Serial.println("setHighPowerThreshold t - Set threshold for high power");
}