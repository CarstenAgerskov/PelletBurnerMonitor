__author__ = 'Carsten Agerskov'

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


