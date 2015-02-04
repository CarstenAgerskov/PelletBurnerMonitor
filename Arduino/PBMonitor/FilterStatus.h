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

#ifndef _FILTER_STATUS_H
#define _FILTER_STATUS_H

#include <Arduino.h>
#include "LightStatus.h"

class FilterStatus : public LightStatus
{
	unsigned long confirmTimeThreshold = 0;
	unsigned long stateTimeStamp = 0;
	unsigned long tentativeStateTimeStamp = 0;
	int state = 0;
	int tentativeState = 0;
public:
	FilterStatus(int, int, int, unsigned long);
	unsigned long getStatusTimeStamp();
	int getStatus();
};

#endif
