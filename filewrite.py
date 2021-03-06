#!/usr/bin/python

import serial

BAUD = 4800
SERIALPORT = '/dev/ttyUSB0'
FILE = 'new_sysctrl.hex'

ser = serial.Serial(SERIALPORT)
program = None
ser.baudrate = BAUD
ser.xonxoff = True
ser.open()
f = open(FILE)
data = f.read()
f.close()
ser.write(data)
ser.close()
