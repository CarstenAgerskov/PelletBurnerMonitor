__author__ = 'Carsten Agerskov'

import ConfigParser
from Instapush import Instapush, App
import datetime
from ast import literal_eval

from ArduinoSerial import ArduinoSerial
from PowerCycle import PowerCycle


config = ConfigParser.RawConfigParser()
config.read('PBMonitor.config')

alarmAppId = config.get('Instapush','alarmAppId')
alarmSecret =  config.get('Instapush','alarmSecret')
serialPort = config.get('Arduino','serialPort')
nofSerialPorts = config.get('Arduino','nofSerialPorts')
logFile = config.get('PBMonitor','logFile')

ser = ArduinoSerial(serialPort,nofSerialPorts)

file = open(logFile, 'a+', 0 )
pushApp = App(appid=alarmAppId, secret=alarmSecret)

powerCycle = PowerCycle()

while 1 :
    line = ser.readline()
    file.write(datetime.datetime.now().isoformat() + ":" + line)
    print(datetime.datetime.now().isoformat() + ":" + line)

    try:
        lineTup = literal_eval(line)
    except:
        print("Not a tuple, skipping analysis")
    else:
        if lineTup[0] == "Alarm":
            pushApp.notify(event_name='Alarm', trackers={ 'State': lineTup[1], 'DateTime': datetime.datetime.now().isoformat()})

        if lineTup[0] == "Start":
            powerCycle = PowerCycle()
            powerCycle.startTimeStamp = lineTup[1]

        if lineTup[0] == "LowPower":
            powerCycle.lowPowerTimeStamp = lineTup[1]

        if lineTup[0] == "HighPower":
            powerCycle.highPowerTimeStamp = lineTup[1]

        if lineTup[0] == "Standby":
            powerCycle.stopTimeStamp =  lineTup[1]
            print "Wrote PowerCycle"

        print "PowerCycle: " + str(powerCycle.getPowerCycle())

ser.close()
file.close()

