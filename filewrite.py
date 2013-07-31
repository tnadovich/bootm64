#!/usr/bin/python

import serial

BAUD = 9600
SERIALPORT = '/dev/ttyUSB0'
FILE = 'new_blink.hex'

ser = serial.Serial(SERIALPORT)
program = None
ser.baudrate = BAUD
ser.open()
f = open(FILE)
data = f.read()
f.close()
ser.write(data)
ser.close()
