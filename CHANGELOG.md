# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.2.0] - 2026-05-30

### Added

- App Settings
  - Date format (DD-MM or MM-DD)
  - Leading zero on main time (show or hide)
  - Seconds display (off, on, or duration on motion)
  - Temperature unit (Celsius or Fahrenheit)
  - Update weather data interval
  - Update weather data on motion toggle
  - Leading zero on sunrise/sunset time labels (show or hide)
  - Vibrate on motion toggle
- Seconds display (off by default - configure in app settings)

### Changed

- Improved UI elements shown while loading.
- Updated some sunrise/sunset time digits to align with larger digit style.
- Show leading zeros in 12h times by default. App settings include show/hide toggle.
- Acknowledging that most of the world uses Celsius and DD-MM format.
  - Now, during the first install, the watch face will attempt to derive the user's preferred date format and temperature unit based on the phone locale and set them as defaults.
  - There is a possibility that existing users will see a change from MM-DD to DD-MM and from Fahrenheit to Celsius.
  - The app settings can be used to switch back to the original MM-DD date format and Fahrenheit temperature unit if desired.
  - Thanks to everyone who tried out the 1.0.1 version!

## [1.0.1] - 2026-04-18

### Added

- Initial application
- Supported platforms:
  - basalt
  - diorite
  - flint
  - emery
  - gabbro
