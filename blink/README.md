<<<<<<< HEAD
The blink code has been modified so that LED1 and LED2 are both flashing, and alternating. To get this code up and working:

1. Compile the blink code by running 'scons' in the terminal from the top-level of the blink repository.
2. Reset the microcontroller by holding down switch one while depressing the red reset button.
3. From the site_scons repository, run the command 'python bootloadergui.py'
4. Using the GUI, upload the blink.hex file.
5. Write the blink.hex file to the microcontroller mode.
6. Switch the microcontroller back into user mode by holding down switch one and pressing the reset button once more.
=======
# Blink Program

These directions are for Mac OS X. Works on OS X 10.11.x (El Capitan).

## Dependencies

- [XC16 Microchip compiler for Mac](http://www.microchip.com/mplabxc16osx)
- [Homebrew](http://brew.sh)
	+ `python` (Homebrew version)
		* `pyusb`
	+ `libusb`
	+ `scons`

The Homebrew version of `python` provides `pip`, which we will use to install `pyusb` (depends on `libusb`).

### Installing dependencies

Install the Microchip compiler and Homebrew from the links above. Then:

```bash
$ brew install python scons libusb
$ pip install pyusb
```

## Building and Running

Plug the board in and put it in bootloader mode (Press the `RST` button while holding down the `SW1` button next to it). Then, from the root of your clone of this repository:

```bash
$ cd blink
$ scons
$ ../site_scons/bootloadercmd.py -i blink.hex -w
```

Reset the board (press the `RST` button) and LEDs 1-3 should start blinking!
>>>>>>> 4dd3851d0a27e51bc2f0abaa05a14a41831d8576
