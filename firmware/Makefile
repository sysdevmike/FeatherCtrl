all: compile

compile:
	mkdir bin -p
	arduino-cli compile $(shell pwd)/src/firmware/firmware.ino -b adafruit:nrf52:feather52832 -o $(shell pwd)/bin/firmware.ino --build-path $(shell pwd)/bin

flash:
	arduino-cli upload -i $(shell pwd)/bin/firmware.ino.hex -p /dev/cu.SLAB_USBtoUART -b adafruit:nrf52:feather52832
