import joystick as h

test = h.hellousb()
while(1):
	[val1,val2] = test.get_vals()
	print val1