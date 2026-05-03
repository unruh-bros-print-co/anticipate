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