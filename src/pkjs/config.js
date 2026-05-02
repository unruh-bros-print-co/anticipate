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
                "defaultValue": "Display"
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
                "type": "toggle",
                "messageKey": "LeadingZero",
                "label": "Leading zero in 12-hr format (current time)",
                "defaultValue": true
            },
            {
                "type": "toggle",
                "messageKey": "LeadingZeroXXS",
                "label": "Leading zero in 12-hr format (sunset/sunrise times)",
                "defaultValue": false
            }
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save"
    }
];