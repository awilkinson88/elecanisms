import joystick as h
import matplotlib.pyplot as plt
import csv
import numpy as np
import time


test = h.hellousb()
# res = []
counter = 0

start = time.time()
while(counter != 3000):
	[val1,val2] = test.get_vals()
	# res.append(val1)
	print val2
	counter += 1
end = time.time()
print end - start