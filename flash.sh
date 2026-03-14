#!/bin/bash

is_available() {
	local T="$1"

	echo -n "* $1: "
	if ! command -v "$1" > /dev/null 2>&1; then
		echo "FAIL"
	else
		echo "OK"
	fi
}

check_requirements() {
# Make sure we have needed base tools
	is_available "openocd"
}

flash() {
# Flash the first argument
openocd -f interface/cmsis-dap.cfg \
		-f target/rp2040.cfg \
		-c "adapter speed 5000" \
		-c "program $1 verify reset exit"
}

if [[ "${BASH_SOURCE[0]}" == "$0" ]]; then
	BIN_PATH="./build/src/VerdantAbode.elf"

	echo "[FLASH]"
	echo -e "\n> TOOLS" && check_requirements
	echo -e "\n> FLASH" && flash $BIN_PATH

	echo "DONE"
fi
