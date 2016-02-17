import joystick as h
import matplotlib.pyplot as plt
import csv
import numpy as np
import time


test = h.hellousb()
test.set_vals(0)
# while(1):
# 	[val1,val2] = test.get_vals()
# 	# res.append(val1)
# 	frac=0
# 	for i in range(0,7):
# 		check = (val2 >> i) & 1
# 		den = 2.0**(7-i)
# 		# print i, check, bin(den)
# 		if (check):
# 			frac += 1.0/den

	frac = val2 & 127
	whole = val2 >> 7
	valShift = val2
	# print val2,bin(val2),bin(whole)
	print val1, val2

