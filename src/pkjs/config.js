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
                "label": "Date Format",
                "options": [
                    { "label": "Day-Month (DD-MM)", "value": "%d-%m" },
                    { "label": "Month-Day (MM-DD)", "value": "%m-%d" }
                ],
                "defaultValue": "%d-%m"
            },
            {
                "type": "select",
                "messageKey": "TemperatureUnit",
                "label": "Temperature Unit",
                "options": [
                    { "label": "Celsius (°C)", "value": "C" },
                    { "label": "Fahrenheit (°F)", "value": "F" }
                ],
                "defaultValue": "C"
            }
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save"
    }
];