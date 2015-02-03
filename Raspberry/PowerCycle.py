__author__ = 'Carsten Agerskov'

"""
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
"""

import datetime

class PowerCycle:
    def __init__(self):
        self.time = datetime.datetime.now().isoformat()
        self.startTimeStamp = 0
        self.lowPowerTimeStamp = 0
        self.highPowerTimeStamp = 0
        self.stopTimeStamp = 0

    def getPowerCycle(self):
        dict = {}
        dict['Time'] = self.time
        dict['Start'] = 0
        dict['LowPower'] = self.lowPowerTimeStamp - self.startTimeStamp
        dict['HighPower'] = self.highPowerTimeStamp - self.startTimeStamp
        dict['Stop'] = self.stopTimeStamp - self.startTimeStamp
        return dict


