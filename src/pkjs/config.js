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
                "defaultValue": "Date"
            },
            {
                "type": "select",
                "messageKey": "DateFormat",
                "label": "Date format",
                "options": [
                    { "label": "Day-Month (DD-MM)", "value": "%d-%m" },
                    { "label": "Month-Day (MM-DD)", "value": "%m-%d" }
                ],
                "defaultValue": "%d-%m"
            }
        ]
    },
    {
        "type": "section",
        "items": [
            {
                "type": "heading",
                "defaultValue": "Time"
            },
            {
                "type": "toggle",
                "messageKey": "LeadingZero",
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
                    { "label": "1 hour", "value": "60" }
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
                "defaultValue": "Sunrise and Sunset"
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
        "type": "submit",
        "defaultValue": "Save"
    }
];