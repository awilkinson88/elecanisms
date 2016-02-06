import encodertest as et
import struct

avg = range(10)

while (1):
	e = et.encodertest()
	value = e.enc_readReg(address=0x3FFF)
	avg = avg[1:]
	temp = struct.unpack('<h',value)
	avg.append(int(temp[0]))
	print 720*(sum(avg)/len(avg))/(2.0**14)
	temp2=bin(temp[0]);
	# print (temp2[2:].zfill(16))
	# print value
