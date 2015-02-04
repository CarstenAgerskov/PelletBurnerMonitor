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

