#!/bin/bash

source_venv() {

	local PATH_VENV="./.venv"
	echo -n "* Finding venv: "
	if [[ ! -d "$PATH_VENV" ]]; then
		echo "FAIL!"
		echo -e "\nMake sure python venv is up-to-date in '$PATH_VENV'!"
		exit 1
	else
		echo "OK"

		echo -n "* Sourcing venv: "
		source "$PATH_VENV/bin/activate" > /dev/null 2>&1
		if (( $? )); then
			echo "FAIL!"
			echo -e "\nSomething went wrong, can't continue!"
			exit 1
		else
			echo "OK"
		fi
	fi
}

check_requirement() {
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
	check_requirement "pyocd"
}

flash() {
	local TARGET_DEV="rp2040"
	pyocd flash -t $TARGET_DEV $1
}

if [[ "${BASH_SOURCE[0]}" == "$0" ]]; then
	BIN_PATH="./build/src/VerdantAbode.elf"

	echo "[FLASH]"
	echo -e "\n> VENV" && source_venv
	echo -e "\n> TOOLS" && check_requirements
	echo -e "\n> FLASH" && flash $BIN_PATH

	echo "DONE"
fi
