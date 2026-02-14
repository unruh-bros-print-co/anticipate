var apiKey = 'b9adedce948f46ade4253555561f2d43';

var xhrRequest = function (url, type, callback) {
    var xhr = new XMLHttpRequest();
    xhr.onload = function () {
        callback(this.responseText);
    };
    xhr.open(type, url);
    xhr.send();
}

function locationSuccess(pos) {
    var url = 'https://api.openweathermap.org/data/2.5/weather?lat=' +
    pos.coords.latitude + '&lon=' + pos.coords.longitude + '&appid=' + apiKey + '&units=imperial';

    xhrRequest(url, 'GET',
        function(responseText) {
            console.log(responseText);

            // for testing
            // responseText = `{
            //     "weather": [
            //         {
            //             "id": 800,
            //             "main": "Clear",
            //             "description": "clear sky",
            //             "icon": "01d"
            //         }
            //     ],
            //     "main": {
            //         "temp": 24,
            //         "temp_max": 24,
            //         "temp_min": 7
            //     },
            //     "sys": {
            //         "sunrise": 1765718998,
            //         "sunset": 1765753031
            //     }
            // }`;

            var json = JSON.parse(responseText);

            var temp_max = Math.round(json.main.temp_max);
            console.log('Temp High: ' + temp_max);

            var temp = Math.round(json.main.temp);
            console.log('Temp (Current): ' + temp);

            var temp_min = Math.round(json.main.temp_min);
            console.log('Temp Low:' + temp_min);

            // Conditions
            var conditions = null;
            var conditionId = json.weather[0].id;
            console.log('Condition ID: ' + conditionId);
            var conditionIndex = conditionId / 100;
            console.log('Condition Index: ' + conditionIndex);
            switch(conditionIndex) {
                case 2:
                    conditions = 'THUNDERSTORM';
                    break;
                case 3:
                    conditions = 'DRIZZLE';
                    break;
                case 5:
                    conditions = 'RAIN';
                    break;
                case 6:
                    conditions = 'SNOW';
                    break;
                case 7:
                    conditions = 'ATMOSPHERE';
                    break;
                case 8:
                    conditions = 
                        conditionId === 800 ? 'CLEAR' : 
                        conditionId === 801 ? 'PARTLY_CLOUDY' : 'CLOUDS';
                    break;
                default:
                    conditions = null;
                    break;                
            }
            console.log('Conditions: ' + conditions);

            // Sunrise / Sunset
            var sunrise = json.sys.sunrise; // seconds!
            console.log('Sunrise: ' + new Date(sunrise * 1000).toString());

            var sunset = json.sys.sunset; // seconds!
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