The blink code has been modified so that LED1 and LED2 are both flashing, and alternating. To get this code up and working:

1. Compile the blink code by running 'scons' in the terminal from the top-level of the blink repository.
2. Reset the microcontroller by holding down switch one while depressing the red reset button.
3. From the site_scons repository, run the command 'python bootloadergui.py'
4. Using the GUI, upload the blink.hex file.
5. Write the blink.hex file to the microcontroller mode.
6. Switch the microcontroller back into user mode by holding down switch one and pressing the reset button once more.
