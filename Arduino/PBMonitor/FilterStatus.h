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
