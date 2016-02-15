import joystick as h
import matplotlib.pyplot as plt
import csv
import numpy as np
import time


test = h.hellousb()
# res = []
counter = 0

# while(counter != 3000):
# 	[val1,val2] = test.get_vals()
# 	# res.append(val1)
# 	print val2
# 	counter += 1
# end = time.time()
# print end - start
while(1):
	[val1,val2] = test.get_vals()
	# res.append(val1)
	frac=0
	for i in range(0,7):
		check = (val2 >> i) & 1
		den = 2.0**(7-i)
		# print i, check, bin(den)
		if (check):
			frac += 1.0/den

	# frac = val2 & 127
	whole = val2 >> 7
	valShift = val2
	print whole,frac