#!/usr/bin/python

import serial, string, sys

SPM_PAGESIZE = 256

def hexstr2int(s):
	out = 0
	for i in range(0,len(s)):
		if (s[i] <= '9' and s[i] >= '0'):
			out += int(s[i]) * (16 ** (len(s) - i - 1))

		elif (str.capitalize(s[i]) <= 'F' and str.capitalize(s[i]) >= 'A'):
			out += (ord(s[i]) - ord('A') + 10) * (16 ** (len(s) - i - 1))
			
		else:
			return 0;

	return out

def get_data(n,lines):
	return lines[n][9:-4]

def get_checksum(n,lines):
	return hexstr2int(lines[n][-4:-2]) 

ser = serial.Serial('/dev/ttyUSB0')
ser.baudrate = 9600
f = open('blink.hex')
lines = f.readlines()
f.close

i = 0
j = 0
no_more_data = False
buf = []
# until the buffer is filled...
while (i < 256):
	# keep getting lines until you hit the end
	print 'getting line ' + str(j)
	data = get_data(j,lines)
	print data
	j += 1
	if (data == ''):
		no_more_data = True
	
	# if you still have a line...
	if(not no_more_data):
		# read a hex byte into an 8 bit char in the buffer
		k = 0
		while(k<len(data)):
			buf.append(chr(hexstr2int(data[k:k+1])))
			i += 1
			k += 2

	# if you've run out of data...
	else:
		# write all the remaining bits to 1s
		while (i < 256):
			buf.append(chr(0xFF))
			i += 1

for ii in range(0,256):
	sys.stdout.write(str(ord(buf[ii])) + ' ')
"""
ser.open()
if (ser.isOpen()):
	print "whee"
	ser.write("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")

ser.close()
if (not ser.isOpen()):
	print "whoo"
"""
