var apiKey = 'ca27d59194a2495788a201710251512';

var xhrRequest = function (url, type, callback) {
    var xhr = new XMLHttpRequest();
    xhr.onload = function () {
        callback(this.responseText);
    };
    xhr.open(type, url);
    xhr.send();
}

function convert12HTimeTo24H(dateStr, timeString12h) {
    console.log('convert12HTimeTo24H - Date: ' + dateStr + ' Time: ' + timeString12h);

    let[year, month, day] = dateStr.split('-').map(Number);

    const timePartsRegex = /(\d{1,2}):(\d\d) ([AP]M)/;
    const match = timeString12h.match(timePartsRegex);

    if (!match) {
        console.error("Could not parse time string: " + timeString12h);
        return null;
    }

    let [, hourStr, minuteStr, meridiem] = match;

    let hours = Number(hourStr);
    let minutes = Number(minuteStr);

    if (meridiem === 'PM' && hours !== 12) {
        hours += 12;
    } else if (meridiem === 'AM' && hours === 12) {
        hours = 0;
    }

    const localDate = new Date(year, month - 1, day, hours, minutes, 0, 0);

    if (isNaN(localDate.getTime())) {
        console.error("Invalid resulting date components.");
        return null;
    }

    return localDate;
}

function locationSuccess(pos) {
    var url = 'http://api.weatherapi.com/v1/forecast.json?key=' + apiKey + '&q=' + pos.coords.latitude + ',' + pos.coords.longitude + '&days=1&aqi=no&alerts=no';

    xhrRequest(url, 'GET',
        function(responseText) {
            console.log(responseText);

            // for testing
            // {
            //     "location": {
            //         "name": "Overland Park",
            //         "region": "Kansas",
            //         "country": "United States of America",
            //         "lat": 38.982,
            //         "lon": -94.671,
            //         ...
            //     },
            //     "current": {
            //         "last_updated_epoch": 1765858500,
            //         "last_updated": "2025-12-15 22:15",
            //         "temp_c": -1.7,
            //         "temp_f": 28.9,
            //         "is_day": 0,
            //         "condition": {
            //             "text": "Clear",
            //             "icon": "//cdn.weatherapi.com/weather/64x64/night/113.png",
            //             "code": 1000
            //         },
            //         ...
            //     },
            //     "forecast": {
            //         "forecastday": [
            //             {
            //                 "date": "2025-12-15",
            //                 "date_epoch": 1765756800,
            //                 "day": {
            //                     "maxtemp_c": 10.2,
            //                     "maxtemp_f": 50.4,
            //                     "mintemp_c": -7.6,
            //                     "mintemp_f": 18.3,
            //                     "condition": {
            //                         "text": "Sunny",
            //                         "icon": "//cdn.weatherapi.com/weather/64x64/day/113.png",
            //                         "code": 1000
            //                     },
            //                 },
            //                 "astro": {
            //                     "sunrise": "07:31 AM",
            //                     "sunset": "04:57 PM",
            //                      ...
            //                 }
            //             }
            //         ]
            //     }
            // }

            var json = JSON.parse(responseText);

            var temp_max = Math.round(json.forecast.forecastday[0].day.maxtemp_f);
            console.log('Temp High: ' + temp_max);

            var temp = Math.round(json.current.temp_f);
            console.log('Temp (Current): ' + temp);

            var temp_min = Math.round(json.forecast.forecastday[0].day.mintemp_f);
            console.log('Temp Low: ' + temp_min);

            // Conditions
            var conditions = json.current.condition.text;
            console.log('Conditions: ' + conditions);

            // Sunrise / Sunset
            var dateString = json.forecast.forecastday[0].date;

            var sunriseTimeString = json.forecast.forecastday[0].astro.sunrise;
            var sunriseDate = convert12HTimeTo24H(dateString, sunriseTimeString);
            var sunrise = sunriseDate.getTime();
            console.log('Sunrise: ' + new Date(sunrise).toString());

            var sunsetTimeString = json.forecast.forecastday[0].astro.sunset;
            var sunsetDate = convert12HTimeTo24H(dateString, sunsetTimeString);
            var sunset = sunsetDate.getTime();
            console.log('Sunset: ' + new Date(sunset).toString());

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

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
    function(e) {
        console.log('PebbleKit JS ready!');

        // Get the initial weather
        getWeather();
    }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
    function(e) {
        console.log('AppMessage received!');
        getWeather();
    }
);