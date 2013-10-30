#!/usr/bin/python

import serial, time, sys

BAUD = 9600
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
for i in range(0,len(data)):
	ser.write(data[i])
	#time.sleep(0.001)
ser.close()
