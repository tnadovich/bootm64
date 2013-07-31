#!/usr/bin/python

import serial, string, sys, time

# Can read in an AVR program to memory from a file, then write it out page by page over a serial connection
PAGESIZE = 256
MEMSIZE = 0xF800
BAUD = 9600
SERIALPORT = '/dev/ttyUSB0'
FILE = 'blink.hex'
ser = serial.Serial(SERIALPORT)
program = None

# Takes a hex string, returns a integer corresponding to it. No error checking
def hexstr2int(s):
	out = 0
	for i in range(0,len(s)):
		if (s[i] <= '9' and s[i] >= '0'):
			out += int(s[i]) * (16 ** (len(s) - i - 1))

		elif (str.capitalize(s[i]) <= 'F' and str.capitalize(s[i]) >= 'A'):
			out += (ord(s[i]) - ord('A') + 10) * (16 ** (len(s) - i - 1))

		else:
			return 0

	return out


# Opens the serial port
def open_ser():
	if(BAUD == None):
		print "Baud rate not set. Cannot open serial port"
		return None
	if(SERIALPORT == None):
		print "Serial Port not set. Cannot open serial port"
		return None
	ser.baudrate = BAUD
	ser.open()

def flash(prog_file):
	f = open(prog_file)
	hexdata = f.readlines()
	f.close()

	c = 0
	if(ser.isOpen()):
		print "Programming"
		for i in range(0,len(hexdata)):
			c += hexstr2int(hexdata[i][1:3])
			if (c >= 256):
				c = 0
				print "Writing flash"

			ser.write(hexdata[i])

		print "Done"
	else:
		print "Serial Port is not open"

def close_ser():
	if(ser.isOpen()):
		ser.close()
	else:
		print "Serial Port is not open"

open_ser()
flash(FILE)
