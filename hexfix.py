#!/usr/bin/python

# Read in any valid Intel hex file and format it.

# The resultant file will explicitly declare every byte of
# every page up to the last page in the file for a given
# pagesize with lines of a given length

import string, math, os

PAGESIZE = 0x100
LINELENGTH = 0x10
FILE = 'mread.hex'
raw_hex_data = []
last_line_address = 0

f = open(FILE)
hexdata = f.readlines()
f.close()

# returns the data length of a line from an Intel hex file
def read_length(s):
	return int(s[1:3],16)

# returns the byte address of a line from an Intel hex file
def read_address(s):
	return int(s[3:7],16)

# obtains the total byte, page, and line length of the data
for i in range(0,len(hexdata)):
	if (last_line_address < read_address(hexdata[i])):
		last_line_address = read_address(hexdata[i])
		last_line_length = read_length(hexdata[i])
		
number_of_pages = math.ceil((last_line_address + last_line_length)/float(PAGESIZE))
number_of_bytes = int(number_of_pages * PAGESIZE)
number_of_lines = int(number_of_bytes / LINELENGTH)

# Create a blank buffer for the hex data
for i in range(0,number_of_bytes):
	raw_hex_data.append('FF')

# Read the actual hex data into the buffer
for i in range(0,len(hexdata)):
	line_length = read_length(hexdata[i])
	addr = read_address(hexdata[i])
	for j in range(0,line_length):
		raw_hex_data[addr + j] = hexdata[i][9+(2*j):11+(2*j)]

# Re-assemble the data into an Intel hex file and write it to a file
newf = open('new_' + FILE, 'w+')
for i in range(0,number_of_lines):
	checksum = LINELENGTH
	a = hex(i*LINELENGTH)[2:]
	if (len(a) > 4):
		print "address overflow"
	else:
		for j in range(0,4-len(a)):
			a = '0' + a
	checksum += int(a[0:2],0x10) + int(a[2:4],0x10)
	line = ':' + hex(LINELENGTH)[2:] + a + '00'
	for j in range(0,LINELENGTH):
		line += raw_hex_data[(i*LINELENGTH)+j]
		checksum += int(raw_hex_data[(i*LINELENGTH)+j],16)
	
	c = hex((((checksum&0xFF)^0xFF)+1)&0xFF)[2:]
	if (len(c) == 1):
		line += '0'
	line += c + '\r\n'
	newf.write(str.upper(line))

newf.write(':00000001FF\r\n')
