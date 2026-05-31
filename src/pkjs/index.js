var Clay = require('@rebble/clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);

var xhrRequest = function (url, type, callback) {
    var xhr = new XMLHttpRequest();
    xhr.onload = function () {
        callback(this.responseText);
    };
    xhr.open(type, url);
    xhr.send();
}

function locationSuccess(pos) {
    var url = 'https://api.open-meteo.com/v1/forecast?' +
    'latitude=' + pos.coords.latitude + 
    '&longitude=' + pos.coords.longitude + 
    '&daily=sunrise,sunset,temperature_2m_max,temperature_2m_min' +
    '&current=temperature_2m,is_day,weather_code' +
    '&timezone=auto' +
    '&forecast_days=1' +
    '&timeformat=unixtime';

    xhrRequest(url, 'GET',
        function(responseText) {
            console.log(responseText);

            // for testing
            // responseText = `{
            //     "latitude": 52.52,
            //     "longitude": 13.419998,
            //     "generationtime_ms": 0.095367431640625,
            //     "utc_offset_seconds": 7200,
            //     "timezone": "Europe/Berlin",
            //     "timezone_abbreviation": "GMT+2",
            //     "elevation": 38.0,
            //     "current_units": {
            //         "time": "unixtime",
            //         "interval": "seconds",
            //         "temperature_2m": "°F",
            //         "is_day": "",
            //         "weather_code": "wmo code"
            //     },
            //     "current": {
            //         "time": 1776307500,
            //         "interval": 900,
            //         "temperature_2m": 49.0,
            //         "is_day": 0,
            //         "weather_code": 3
            //     },
            //     "daily_units": {
            //         "time": "unixtime",
            //         "sunrise": "unixtime",
            //         "sunset": "unixtime",
            //         "temperature_2m_max": "°F",
            //         "temperature_2m_min": "°F"
            //     },
            //     "daily": {
            //         "time": [
            //             1776290400
            //         ],
            //         "sunrise": [
            //             1776312366
            //         ],
            //         "sunset": [
            //             1776362769
            //         ],
            //         "temperature_2m_max": [
            //             67.0
            //         ],
            //         "temperature_2m_min": [
            //             48.0
            //         ]
            //     }
            // }`;

            var json = JSON.parse(responseText);

            var temp_max = Math.round(json.daily.temperature_2m_max[0]);
            console.log('Temp High: ' + temp_max);

            var temp = Math.round(json.current.temperature_2m);
            console.log('Temp (Current): ' + temp);

            var temp_min = Math.round(json.daily.temperature_2m_min[0]);
            console.log('Temp Low:' + temp_min);

            // Conditions
            var conditions = null;
            var conditionId = json.current.weather_code;
            console.log('Condition ID: ' + conditionId);
            switch(conditionId) {
                case 0: // Clear Sky
                    conditions = 'CLEAR';
                    break;
                case 1: // Mainly clear
                case 2: // Partly Cloudy
                    conditions = 'PARTLY_CLOUDY';
                    break;
                case 3: // Overcast
                    conditions = 'CLOUDS';
                    break;
                case 45: // Fog
                case 48: // Depositing rime fog
                    conditions = 'ATMOSPHERE';
                    break;
                case 51: // Drizzle: Light intensity
                case 53: // Drizzle: Moderate intensity
                case 55: // Drizzle: Dense intensity
                    conditions = 'DRIZZLE'; // TODO - split this out with different icons.
                    break;
                case 56: // Freezing Drizzle: light intensity
                case 57: // Freezing Drizzle: dense intensity
                    conditions = 'DRIZZLE'; // TODO - make these 'FREEZING_DRIZZLE' icons.
                    break;
                case 61: // Rain: slight intensity
                case 63: // Rain: moderate intensity
                case 65: // Rain: heavy intensity
                    conditions = 'RAIN'; // TODO - split this out with different icons.
                    break;
                case 66: // Freezing rain: light
                case 67: // Freezing rain: heavy
                    conditions = 'RAIN' // TODO - make these 'FREEZING_RAIN' icons.
                    break;
                case 71: // Snow fall: slight intensity
                case 73: // Snow fall: moderate intensity
                case 75: // Snow fall: heavy intensity
                case 77: // Snow grains
                    conditions = 'SNOW';
                    break;
                case 80: // Rain showers: slight
                case 81: // Rain showers: moderate
                case 82: // Rain showers: violent
                    conditions = 'RAIN'; // TODO - split this out with different icons.
                    break;
                case 85: // Snow showers: slight
                case 86: // Snow showers: heavy
                    conditions = 'SNOW'; // TODO - make these 'SNOW_SHOWERS' icons.
                    break;
                case 95: // Thunderstorm: slight or moderate
                case 96: // Thunderstorm: with slight hail (Central Europe only)
                case 99: // Thunderstorm: with heavy hail (Central Europe only)
                    conditions = 'THUNDERSTORM';
                    break;
                default:
                    conditions = null;
                    break;                
            }
            console.log('Conditions: ' + conditions);

            // Sunrise / Sunset
            var sunrise = json.daily.sunrise[0]; // seconds!
            console.log('Sunrise: ' + new Date(sunrise * 1000).toString());

            var sunset = json.daily.sunset[0]; // seconds!
            console.log('Sunset: ' + new Date(sunset * 1000).toString());

            var dictionary = {
                'TEMP_HI': temp_max,
                'TEMP_CUR': temp,
                'TEMP_LO': temp_min,
                'CONDITIONS': conditions,
                'SUNRISE': sunrise,
                'SUNSET': sunset
            };

            Pebble.sendAppMessage(dictionary,
                function(e) {
                    console.log('Weather info sent to Pebble successfully!');
                },
                function(e) {
                    console.log('Error sending weather info to Pebble!');
                }
            );
        }
    )
}

function locationError(err) {
    console.log('Error requesting location!');
}

function getWeather() {
    navigator.geolocation.getCurrentPosition(
        locationSuccess,
        locationError,
        {timeout: 15000, maximumAge: 60000}
    );
}

Pebble.addEventListener('ready',
    function(e) {
        console.log('PebbleKit JS ready!');

        var claySettingsJSON = localStorage.getItem('clay-settings');
        var claySettings = null;
        
        // Fresh install: No 'clay-settings' found on phone.
        //      -> Set initial settings defaults based on the user's locale.
        if (claySettingsJSON === null) {
            console.log('V2 INSTALL detected! Assigning locale-based settings defaults...');

            var locale = (navigator.language || 'en-US').toLowerCase();

            var fahrenheitCountries = ['-us', '-bs', '-ky', '-lr', '-pw', '-mh', '-fm'];
            var mmddCountries = ['-us', '-ca', '-ph', '-bz'];

            var useFahrenheit = fahrenheitCountries.some(function(suffix) {
                return locale.endsWith(suffix);
            });

            var useMMDD = mmddCountries.some(function(suffix) {
                return locale.endsWith(suffix);
            });

            // Logs for debugging purposes
            console.log("=============================================");
            console.log("V2 BOOTSTRAP LOG");
            console.log("Detected Phone Locale: " + locale);
            console.log("Matches Fahrenheit Country? " + (useFahrenheit ? "YES (F)" : "NO (C)"));
            console.log("Matches MM-DD Country?       " + (useMMDD ? "YES (MM-DD)" : "NO (DD-MM)"));
            console.log("=============================================");

            // Programatically write locale-based settings before the user opens them.
            claySettings = {
                "DateFormat": useMMDD ? "MMDD" : "DDMM",
                "LeadingZero": true,
                "DisplaySecondsInterval": "0",
                "TemperatureUnit": useFahrenheit ? "F" : "C",
                "WeatherUpdateInterval": "30",
                "WeatherUpdateOnMotion": false,
                "LeadingZeroXXS": true,
                "VibrateOnMotion": false
            };

            // Set these locale-based default settings to the localStorage on the phone.
            localStorage.setItem('clay-settings', JSON.stringify(claySettings));
        }
        else {
            console.log('V2 RETURNING USER detected! Loading saved profile choices');
            claySettings = JSON.parse(claySettingsJSON);
        }

        // Send the settings message to the phone in EVERY case to pass settings to watch.
        // Using a separate object because some fields types need to be converted.
        var directWatchPayload = {
            "DateFormat": claySettings.DateFormat, // keep as string
            "LeadingZero": claySettings.LeadingZero ? 1 : 0, // convert bool to int
            "DisplaySecondsInterval": claySettings.DisplaySecondsInterval, // keep as string
            "TemperatureUnit": claySettings.TemperatureUnit, // keep as string
            "WeatherUpdateInterval": claySettings.WeatherUpdateInterval, // keep as string
            "WeatherUpdateOnMotion": claySettings.WeatherUpdateOnMotion ? 1 : 0, // convert bool to int
            "LeadingZeroXXS": claySettings.LeadingZeroXXS ? 1 : 0, // convert bool to int
            "VibrateOnMotion": claySettings.VibrateOnMotion ? 1: 0 // convert bool to int
        };

        Pebble.sendAppMessage(directWatchPayload);

        // Get the initial weather
        getWeather();
    }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
    function(e) {
        console.log('AppMessage received!');
        if (e.payload['REQUEST_WEATHER']) {
            getWeather();
        }
    }
);