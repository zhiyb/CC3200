#!/bin/sh
if lsmod | fgrep -q ftdi-sio; then
	:
else
	modprobe ftdi-sio
	echo 0451 c32a > /sys/bus/usb-serial/drivers/ftdi_sio/new_id
	sleep 1
fi
microcom -s 115200 -p $(ls /dev/ttyUSB* | tail -n 1)
