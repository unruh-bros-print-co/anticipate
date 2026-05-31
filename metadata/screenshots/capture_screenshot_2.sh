#!/bin/bash

# Before running:

# These are set automatically by the script:
# Date: Current Date? Hopefully something without a '1'
# Time: 20:45 (24h)
# Time Format: 24h (Set by this script)

# Make these manual changes:
# anticipate.c:
    # Steps: 10372
    # Conditions: 'CLEAR' (Moon)
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
faketime '2026-05-30 20:45:00' pebble install --emulator $1
# pebble install --emulator $1

# Set time format
pebble emu-time-format --format 24h

# Use this command if the emulator is showing in UTC time.
pebble emu-set-time 15:45:00 && pebble screenshot ${1}_2.png

# Troubleshooting note: if app install gets stuck, try 'pebble wipe' before running this script.