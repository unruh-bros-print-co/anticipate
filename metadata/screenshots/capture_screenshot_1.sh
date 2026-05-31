#!/bin/bash

# Before running:

# These are set automatically by the script:
# Date: Current Date? Hopefully something without a '1'
# Time: 08:37 (12h)
# Time Format: 12h (Set by this script)

# Make these manual changes:
# anticipate.c:
    # Steps: 10372
    # Conditions: 'RAIN'
# index.js:
    # Temp High: 25 C (use F to convert)
    # Temp Current: 23 C (use F to convert)
    # Temp Low: 17 C (use F to convert)
    # "DateFormat": "MMDD",
    # "DisplaySecondsInterval": "0",
    # "TemperatureUnit": "F",

# Run "pebble build"

echo "Preparing Screenshot for platform:[$1]..."

# Kill any running emulators
pebble kill

# Buid any changes
pebble build

# Set to just before desired time to capture correct second
# --> Update the Date to the current day!
faketime '2026-05-30 08:36:40' pebble install --emulator $1
# pebble install --emulator $1

# Set time format
pebble emu-time-format --format 12h

# Use this command if the emulator is showing in UTC time.
pebble emu-set-time 03:37:00 && pebble screenshot ${1}_1.png

# Troubleshooting note: if app install gets stuck, try 'pebble wipe' before running this script.