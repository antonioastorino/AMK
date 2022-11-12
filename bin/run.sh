#!/bin/zsh

set -ue
set -o pipefail

arduino-cli lib install Keyboard
arduino-cli board list
local ARDUINO_DETAILS="$(arduino-cli board list)"
local SERIAL_DEVICE=$(echo ${ARDUINO_DETAILS} | grep arduino | awk '{print $1}')
local ARDUINO_BOARD=$(echo ${ARDUINO_DETAILS} | grep arduino | awk '{print $(NF-1)}')

function f_log_info {
    echo "\e[0mINFO:\e[0m ${1}"
}

function f_log_err {
    echo "\e[31mERROR:\e[0m ${1}"
}

if [ "${SERIAL_DEVICE}" = "" ]; then
    f_log_err "Arduino board not found"
    exit 1
fi

f_log_info "${SERIAL_DEVICE} found."

arduino-cli board attach -p "${SERIAL_DEVICE}" keyboard
arduino-cli -v compile -b "${ARDUINO_BOARD}" -p "${SERIAL_DEVICE}" keyboard
arduino-cli -v upload keyboard
