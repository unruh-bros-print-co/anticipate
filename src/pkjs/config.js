/**
 * Defaults set for global users upon opening Settings page.
 */
module.exports = [
    {
        "type": "heading",
        "defaultValue": "Anticipate"
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Date & Time"
            },
            {
                "type": "select",
                "messageKey": "DateFormat",
                "label": "Date format",
                "options": [
                    { "label": "Day-Month (DD-MM)", "value": "DDMM" },
                    { "label": "Month-Day (MM-DD)", "value": "MMDD" }
                ],
                "defaultValue": "DDMM"
            },
            {
                "type": "toggle",
                "messageKey": "LeadingZero",
                "label": "Use leading zero in 12h time format",
                "defaultValue": true
            },
            {
                "type": "select",
                "messageKey": "DisplaySecondsInterval",
                "label": "Display seconds",
                "options": [
                    { "label": "Off", "value": "0" },
                    { "label": "Always on", "value": "1" },
                    { "label": "5 sec (on motion)", "value": "5" },
                    { "label": "10 sec (on motion)", "value": "10" },
                    { "label": "15 sec (on motion)", "value": "15" },
                    { "label": "30 sec (on motion)", "value": "30" },
                    { "label": "45 sec (on motion)", "value": "45" },
                    { "label": "1 min (on motion)", "value": "60" }
                ],
                "defaultValue": "0"
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Weather"
            },
            {
                "type": "select",
                "messageKey": "TemperatureUnit",
                "label": "Temperature unit",
                "options": [
                    { "label": "Celsius (°C)", "value": "C" },
                    { "label": "Fahrenheit (°F)", "value": "F" }
                ],
                "defaultValue": "C"
            },
            {
                "type": "select",
                "messageKey": "WeatherUpdateInterval",
                "label": "Update weather data every",
                "options": [
                    { "label": "1 min", "value": "1" },
                    { "label": "5 min", "value": "5" },
                    { "label": "10 min", "value": "10" },
                    { "label": "15 min", "value": "15" },
                    { "label": "30 min", "value": "30" },
                    { "label": "45 min", "value": "45" },
                    { "label": "1 hour", "value": "60" },
                    { "label": "3 hours", "value": "180" },
                    { "label": "6 hours", "value": "360" }
                ],
                "defaultValue": "30"
            },
            {
                "type": "toggle",
                "messageKey": "WeatherUpdateOnMotion",
                "label": "Update weather data on motion",
                "defaultValue": false
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Sunrise & Sunset"
            },
            {
                "type": "toggle",
                "messageKey": "LeadingZeroXXS",
                "label": "Use leading zero in 12h time format",
                "defaultValue": true
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Haptics"
            },
            {
                "type": "toggle",
                "messageKey": "VibrateOnMotion",
                "label": "Vibrate on motion",
                "defaultValue": false
            }
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save"
    }
];