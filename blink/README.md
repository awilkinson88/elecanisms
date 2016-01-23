# Blink Program

These directions are for Mac OS X. Works on OS X 10.11.x (El Capitan).

## Dependencies

- [XC16 Microchip compiler for Mac](http://www.microchip.com/mplabxc16osxp)
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