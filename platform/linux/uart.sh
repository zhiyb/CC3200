#!/bin/bash

com="/dev/ttyUSB*"

case "$1" in
"load")
	[[ $UID == 0 ]] || (echo "Run as root!" >&2 && exit 1)
	modprobe ftdi_sio
	echo 0451 c32a > /sys/bus/usb-serial/drivers/ftdi_sio/new_id
	exit 0;;
"unload")
	if [[ $UID != 0 ]]; then
		sudo modprobe -r ftdi_sio
	else
		modprobe -r ftdi_sio
	fi
	exit;;
esac

(lsmod | fgrep -q ftdi_sio) || (
	echo "Loading ftdi_sio module..." >&2
	if [[ $UID != 0 ]]; then
		sudo $0 load
	else
		$0 load
	fi || exit 1
	sleep 1
)

tty=$(ls $com | tail -n 1)
[ -z "$tty" ] && echo "Cannot find COM port from $com!" >&2 && exit 1
echo "Opening COM port $tty..."
microcom -s 115200 -p "$tty"
