#!/usr/bin/python

import serial, string, sys

PAGESIZE = 256
MEMSIZE = 0xF800
BAUD = 9600
SERIALPORT = '/dev/ttyUSB0'

# Can read in an AVR program to memory from a file, then write it out page by page over a serial connection
class AvrSerialProgrammer:
	pagesize = None
	memsize = None
	baud = None
	serialport = None
	ser = None
	program = None

	def __init__(ps,ms,b,sp):
		pagesize = ps
		memsize = ms
		baud = b
		serialport = sp
	
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
		if(baud == None):
			print "Baud rate not set. Cannot open serial port"
			return None
		if(serialport == None):
			print "Serial Port not set. Cannot open serial port"
			return None
		ser = serial.Serial(serialport)
		ser.baudrate = baud
		ser.open()
	
	def flash(prog_file):
		f = open(prog_file)
		hexdata = f.readlines()
		f.close()

		if(ser.isOpen()):
			print "Programming"
			for i in range(0,len(hexdata)):
				ser.write(hexdata[i])
			print "Done"
		else:
			print "Serial Port is not open"

	def close_ser():
		if(ser.isOpen()):
			ser.close()
		else:
			print "Serial Port is not open"

asp = AvrSerialProgrammer(PAGESIZE,MEMSIZE,BAUD,SERIALPORT)
asp.open_ser()
asp.flash('blink.hex')
