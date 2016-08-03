//floodwatch JS App
var getCoord = function (obj) {
    if (Array.isArray(obj) &&
        typeof obj[0] === 'number' &&
        typeof obj[1] === 'number') {
        return obj;
    } else if (obj) {
        if (obj.type === 'Feature' &&
            obj.geometry &&
            obj.geometry.type === 'Point' &&
            Array.isArray(obj.geometry.coordinates)) {
            return obj.geometry.coordinates;
        } else if (obj.type === 'Point' &&
            Array.isArray(obj.coordinates)) {
            return obj.coordinates;
        }
    }
    throw new Error('A coordinate, feature, or point geometry is required');
};

var factors = {
    miles: 3960,
    nauticalmiles: 3441.145,
    degrees: 57.2957795,
    radians: 1,
    inches: 250905600,
    yards: 6969600,
    meters: 6373000,
    metres: 6373000,
    kilometers: 6373,
    kilometres: 6373
};

var radiansToDistance = function (radians, units) {
    var factor = factors[units || 'kilometers'];
    if (factor === undefined) {
        throw new Error('Invalid unit');
    }
    return radians * factor;
};

var turf_distance = function (from, to, units) {
    var degrees2radians = Math.PI / 180;
    var coordinates1 = getCoord(from);
    var coordinates2 = getCoord(to);
    var dLat = degrees2radians * (coordinates2[1] - coordinates1[1]);
    var dLon = degrees2radians * (coordinates2[0] - coordinates1[0]);
    var lat1 = degrees2radians * coordinates1[1];
    var lat2 = degrees2radians * coordinates2[1];

    var a = Math.pow(Math.sin(dLat / 2), 2) +
          Math.pow(Math.sin(dLon / 2), 2) * Math.cos(lat1) * Math.cos(lat2);

    return radiansToDistance(2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a)), units);
};

var processReports = function(reports){
  user_location = {
      "type": "Feature",
      "properties": {},
      "geometry": {
        "type": "Point",
        "coordinates": [106.7932, -6.2196]
      }
  };

  var pkey = [0];
  var text = ["No Flood Reports Available"];
  var time = ["NA"];
  var distance = ["NA"];

  if (reports.features !== null) {

    // Reset stores
    var pkey = [];
    var text = [];
    var time = [];
    var distance = [];

    for (var i = 0; i < reports.features.length -1; i++){
      var dist = turf_distance(user_location, reports.features[i], 'kilometers');
      if (dist <= 5.0) {
        dist = dist.toFixed(1);
        pkey.push(reports.features[i].properties.pkey);
        text.push(reports.features[i].properties.text);
        time.push(reports.features[i].properties.created_at.substring(11,16));
        distance.push(dist);
      }
    }
    console.log(distance.length);
  }
  // Assemble dictionary using our keys
  var dictionary = {
    "KEY_PKEY": pkey.toString(),
    "KEY_DISTANCE":distance.toString(),
    "KEY_TIME": time.toString(),
    "KEY_DESCRIPTION": text.join("|")
  };

    Pebble.sendAppMessage(dictionary,
      function(e) {
        console.log("Flood info sent to Pebble successfully!");
      },
      function(e) {
        console.log("Error sending Flood info to Pebble!");
      }
    );
};

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

var getReports = function () {
  // Construct URL
  var url = "http://192.168.1.7:8081/banjir/data/api/v2/reports/confirmed";

  // Send request to Flood API
  xhrRequest(url, 'GET',
    function(responseText) {
      // responseText contains a JSON object with flood info
      var json = JSON.parse(responseText);
      processReports(json);
      }
    );
}

/*
function FlooddataError(err) {
  console.log("Error requesting Flooddata!");
}*/

/*
function flood() {
  navigator.geolocation.getCurrentPosition(
    FlooddataSuccess,
    FlooddataError,
    {timeout: 150, maximumAge: 600}
  );
  FlooddataSuccess();
}*/

// Listen for when the watchapp is opened
Pebble.addEventListener('ready',
function(e) {
  console.log("PebbleKit JS ready!");
  getReports();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    }
);
