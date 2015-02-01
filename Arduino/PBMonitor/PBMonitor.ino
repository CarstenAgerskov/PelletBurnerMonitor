#include "FilterStatus.h"

#define LED_PIN 13

#define ALARM_PIN 2

#define THRESHOLD_STANDBY 440
#define THRESHOLD_START 420
#define THRESHOLD_RUN 450
#define TIME_THRESHOLD 10000

bool standByOn = false;
bool startupOn = false;
bool runOn = false;
bool alarmOn = false;
int alarmValue;

FilterStatus run = FilterStatus(THRESHOLD_RUN, 0, A0, TIME_THRESHOLD);
FilterStatus startup = FilterStatus(THRESHOLD_START, 0, A1, TIME_THRESHOLD);
FilterStatus standBy = FilterStatus(THRESHOLD_STANDBY, 500, A2, TIME_THRESHOLD);

void setup() {
	Serial.begin(9600);
	pinMode(LED_PIN, OUTPUT);
	pinMode(ALARM_PIN, INPUT_PULLUP);
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
			Serial.print("Starting: ");
			Serial.println(standBy.getStatusTimeStamp());
			standByOn = false;
		}
	}
	else {
		if (standBy.getStatus() != LIGHT_BLINK && startup.getStatus() != LIGHT_ON) {
			Serial.print("Standby: ");
			Serial.println(standBy.getStatusTimeStamp());
			standByOn = true;
		}
	}

	if (startupOn) {
		if (startup.getStatus() != LIGHT_ON) {
			Serial.print("No low power: ");
			Serial.println(startup.getStatusTimeStamp());
			startupOn = false;
		}
	}
	else {
		if (startup.getStatus() == LIGHT_ON) {
			Serial.print("Low power: ");
			Serial.println(startup.getStatusTimeStamp());
			startupOn = true;
		}
	}

	if (runOn) {
		if (run.getStatus() != LIGHT_ON) {
			Serial.print("No high power: ");
			Serial.println(run.getStatusTimeStamp());
            runOn = false;
		}
	}
	else {
		if (run.getStatus() == LIGHT_ON) {
			Serial.print("High power: ");
			Serial.println(run.getStatusTimeStamp());
			runOn = true;
		}
	}

	alarmValue = digitalRead(ALARM_PIN);
	if (alarmOn) {
		if (alarmValue == LOW) {
			digitalWrite(LED_PIN, LOW);
			Serial.print("Alarm off: ");
			Serial.println(millis());
			alarmOn = false;
		}
	}
	else {
		if (alarmValue == HIGH) {
			digitalWrite(LED_PIN, HIGH);
			Serial.print("Alarm on: ");
			Serial.println(millis());
			alarmOn = true;
		}
	}
}

