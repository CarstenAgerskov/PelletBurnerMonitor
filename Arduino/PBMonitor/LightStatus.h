#ifndef _LIGHT_STATUS_H
#define _LIGHT_STATUS_H

#include <Arduino.h>

#define LIGHT_OFF 0
#define LIGHT_BLINK 1
#define LIGHT_ON 2

class LightStatus
{
	int lightStatus;
	int lightThreshold;
	int lightBlinkInterval;
	int lightAnalogPin;
	unsigned long timeStampStateChange = 0;
	bool lastState = false;
public:
	LightStatus(int,int,int);
	int getStatus();
	void checkStatus();
};

#endif

