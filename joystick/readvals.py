import joystick as h
import matplotlib.pyplot as plt
import csv
import numpy as np
import time


test = h.hellousb()
res = []
counter = 0
start = time.time()

while(counter != 3000):
	[val1,val2] = test.get_vals()
	res.append(val2)
	counter += 1

end = time.time()
elapsed = end - start
time_vec = np.linspace(0,elapsed,3000)
x = np.array(res)
deriv = np.diff(x)
# plt.plot(time_vec[1:],deriv)
plt.plot(time_vec,x)
plt.ylabel('Position (deg)')
plt.xlabel('Time (s)')
plt.show()
