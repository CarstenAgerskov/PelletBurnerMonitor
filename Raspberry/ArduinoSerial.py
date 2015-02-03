__author__ = 'Carsten Agerskov'

"""
    ArduinoSerial: For managing a serial connection to an Arduino
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

import serial

class ArduinoSerial:
    def __init__(self,serialPort,nofSerialPorts):
        i = 0
        while i < nofSerialPorts:
            sp = serialPort + str(i)
            try:
                print "Trying to connect to serial port: ",  sp
                self.ser = serial.Serial(sp)
            except:
                print "Failed to connect to serial port: ",  sp
                i = i + 1
            else:
                print "Connect to serial port: ",  sp
                i = nofSerialPorts

    def readline(self):
        return self.ser.readline()


