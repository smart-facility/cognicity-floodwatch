
var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function FlooddataSuccess(pos) {
  // Construct URL
  var url = "https://petajakarta.org/banjir/data/api/v2/floodwatch/reports/?area=JAKARTA%20TIMUR";

  // Send request to Flood API
  xhrRequest(url, 'GET',
  function(responseText) {
    // responseText contains a JSON object with flood info
    var json = JSON.parse(responseText);

    if (json.data == null){
      // If no floods prevaltent at update time
      // Flood Area
      var area = "No Flood Reports Available";
      console.log("No Flood Reports Available");
      //Flood Media Source
      var source = "No Flood Reports Available";
      console.log("No Flood Reports Available");
      //Flood Description in indonesian
      var description = "No Flood Reports Available";
      console.log("No Flood Reports Available");

      var dictionary = {

        "KEY_AREA": area,
        "KEY_SOURCE":source,
        "KEY_DESCRIPTION": description


      };
    }

    else{

      var source =[];
      var area = [];
      var description = [];
      var json_length = json.data.length;
      console.log(json_length);

      for (i=0; i< json_length; i++){
        // Flood Area
        area[i] = json.data[i].area_name ;
        console.log(area[i]);
        //Flood Media Source
        source[i]= json.data[i].source;
        console.log(source[i]);
        description[i]= json.data[i].text + "|";
        console.log(description[i]);


      }
      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_AREA": area.toString(),
        "KEY_SOURCE": source.toString(),
        "KEY_DESCRIPTION": description.toString()


      };
    }

    console.log("results are " + area +  ", " + source + "," + description);
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

function FlooddataError(err) {
  console.log("Error requesting Flooddata!");
}

function flood() {
  navigator.geolocation.getCurrentPosition(
    FlooddataSuccess,
    FlooddataError,
    {timeout: 150, maximumAge: 600}
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
