
var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  // Construct URL
  var url = "https://petajakarta.org/banjir/data/api/v2/floodwatch/reports/?area=JAKARTA%20TIMUR";

  // Send request to Flood API
  xhrRequest(url, 'GET',
    function(responseText) {
      // responseText contains a JSON object with flood info
      var json = JSON.parse(responseText);

      if (json.data == null){
        // If no floods prevaltent at update time
        var pkey = "No Flood Reports Available";
        console.log("No Flood Reports Available");
        // Flood Area
        var area = "No Flood Reports Available";
        console.log("No Flood Reports Available");
        //Flood Media Source
        var source = "No Flood Reports Available";
        console.log("No Flood Reports Available");
        //Flood Description in indonesian
        var text = "No Flood Reports Available";
        console.log("No Flood Reports Available");

        var dictionary = {
          "KEY_NUMBER": pkey,
          "KEY_AREA": area,
          "KEY_SOURCE":source,
          "KEY_DESCRIPTION":text


        };
      }
      else{

      // Flood Number
      var pkey = json.data[0].pkey;
      console.log("keys are " + pkey);
      // Flood Area
      var area = json.data[0].area_name;
      console.log("Area is " + area);
      //Flood Media Source
      var source = json.data[0].source;
      console.log("Source is " + source);
      //Flood Description in indonesian
      var text = json.data[0].text;
      console.log("Description is " + text);

      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_NUMBER": pkey,
        "KEY_AREA": area,
        "KEY_SOURCE":source,
        "KEY_DESCRIPTION":text


      };
    }

console.log("results are " + pkey  +  ", " + area +  ", " + source + ", " + text);
      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Flood info sent to Pebble successfully!");

        },
        function(e) {
          console.log("Error sending Flood info to Pebble! No Flood Data Available");
        }
      );
    }
  );
}

function locationError(err) {
  console.log("Error requesting location!");
}

function flood() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

// Listen for when the watchapp is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial flood details
    flood();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    flood();
  }
);
