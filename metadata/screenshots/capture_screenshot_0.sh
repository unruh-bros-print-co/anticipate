#!/bin/bash

# Before running:

# These are set automatically by the script:
# Date: Current Date? Hopefully something without a '1'
# Time: 12:34am
# Time Format: 12h (Set by this script)

# Make these manual changes:
# anticipate.c:
    # Steps: these
    # Conditions: 'CLEAR'
# index.js:
    # Temp High: 25 C
    # Temp Current: 23 C
    # Temp Low: 17 C
    # "DateFormat": "DDMM",
    # "DisplaySecondsInterval": "1", (Always On) (Capture Seconds at "00")
    # "TemperatureUnit": "C",

# Run "pebble build"

echo "Preparing Screenshot for platform:[$1]..."

# Kill any running emulators
pebble kill

# Buid any changes
pebble build

# Set to just before desired time to capture correct second.
# --> Update the Date to the current day!
faketime '2026-05-30 12:33:40' pebble install --emulator $1
# pebble install --emulator $1

# Some emulators use this command to set the time.
# pebble emu-set-time 12:33:40

# pebble install --emulator basalt
pebble emu-time-format --format 12h

# Use this command if the emulator is showing in UTC time.
pebble emu-set-time 07:34:00 && pebble screenshot ${1}_0.png

# Troubleshooting note: if app install gets stuck, try 'pebble wipe' before running this script.