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

print "PBMonitor Copyright (C) 2015  Carsten Agerskov"
print "This program comes with ABSOLUTELY NO WARRANTY"
print "This is free software, and you are welcome to redistribute it"
print "under certain conditions; Refer to the GNU General Public License"
print "as published by the Free Software Foundation, version 3"

import ConfigParser
import datetime
from ast import literal_eval
from multiprocessing import Process, Queue
import subprocess

from ArduinoSerial import ArduinoSerial
from PowerCycle import PowerCycle
from PushNotification import pushNotification
from PowerCycle2GSheet import powerCycle2GSheet

config = ConfigParser.RawConfigParser()
config.read('PBMonitor.config')

serialPort = config.get('Arduino','serialPort')
nofSerialPorts = config.get('Arduino','nofSerialPorts')
logFile = config.get('PBMonitor','logFile')

ser = ArduinoSerial(serialPort,nofSerialPorts)

file = open(logFile, 'a+', 0 )

powerCycle = PowerCycle()

if __name__ == '__main__':
    pushQueue = Queue()
    gSheetQueue = Queue()
    pushNotificationPid = Process(target=pushNotification, args=(pushQueue,))
    powerCycle2GSheetPid = Process(target=powerCycle2GSheet, args=(gSheetQueue,))
    pushNotificationPid.start()
    print "starting pushNotification"
    powerCycle2GSheetPid.start()
    print "starting powerCycle2GSheet"

    lineTup = ('',)
    while lineTup[0] != "PBMonShutDown":
        line = ser.readline()
        file.write(datetime.datetime.now().isoformat() + ":" + line)
        print(datetime.datetime.now().isoformat() + ":" + line)

        try:
            lineTup = literal_eval(line)
        except:
            print("Not a tuple, skipping analysis")
        else:
            if lineTup[0] == "Alarm":
                notification = (lineTup[0],lineTup[1],datetime.datetime.now().isoformat())
                pushQueue.put(notification)

            if lineTup[0] == "Start":
                powerCycle = PowerCycle()
                powerCycle.startTimeStamp = lineTup[1]

            if lineTup[0] == "LowPower":
                powerCycle.lowPowerTimeStamp = lineTup[1]

            if lineTup[0] == "HighPower":
                powerCycle.highPowerTimeStamp = lineTup[1]

            if lineTup[0] == "Standby":
                powerCycle.stopTimeStamp =  lineTup[1]
                gSheetQueue.put(powerCycle.getPowerCycle())
                print "Wrote PowerCycle to queue"


    gSheetQueue.put('PBMonShutDown')

    if lineTup[0] == "PBMonShutDown":
        notification = (lineTup[0],lineTup[1],datetime.datetime.now().isoformat())
        pushQueue.put(notification)
    else:
        notification = ("PBMonError",datetime.datetime.now().isoformat(),"PBMon exited unexpected")
        pushQueue.put(notification)

    powerCycle2GSheetPid.join()
    pushNotificationPid.join()

file.close()

command = "/usr/bin/sudo /sbin/shutdown now"
process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
output = process.communicate()[0]
print output


