__author__ = 'Carsten Agerskov'
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


