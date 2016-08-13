'use strict';
// Floodwatch - Flood alerts from PetaJakarta.org
// app.js - JavaScript app for mobile
// (c) Hasitha Jayanandana, Tomas Holderness & Matthew Berryman 2016
// Released under GNU GPLv3 (see LICENSE.txt)

var turf_distance = require('./libs/@turf/distance');

// Get user location
var getUserLocation = function(callback){

  function success(pos){
    var user_location = {
        "type": "Feature",
        "properties": {},
        "geometry": {
          "type": "Point",
          //"coordinates": [pos.coords.longitude, pos.coords.latitude]
          "coordinates":[106.826355,-6.1763649] /*for testing - @MONAS*/
        }
    }
    callback(user_location);
  }

  function error(err){
    callback(err);
  }

  var options = {
    enableHighAccuracy: true,
    maximumAge: 10000,
    timeout: 10000
  };

  navigator.geolocation.getCurrentPosition(success, error, options);
}

// Process incoming flood data from PetaJakarta.org
var processReports = function(report_string, status){

  // Data stores
  var pkey = [];
  var text = ['[Success] No nearby flood reports'];
  var time = [];
  var distance = [];

  getUserLocation(function(user_location){

    if (status !== 200){
      console.log('Error communicating with server ('+status+')');
      text[0] = '[Error '+status+'] Problem communicating with server';
    }
    else {
      var reports = JSON.parse(report_string);
      // check for user location
      if (user_location.type == "Feature"){
        // check that there are some reports
        if (reports.features !== null) {
          // loop reports and check if nearby
          for (var i = 0; i < Math.min(reports.features.length, 10); i++){
            var dist = turf_distance(user_location, reports.features[i], 'kilometers');
            if (dist <= 5.0) {
              dist = dist.toFixed(1);
              pkey[i] = reports.features[i].properties.pkey;
              if (reports.features[i].properties.text.length > 160){
                text[i] = reports.features[i].properties.text.slice(0,156)+'...'
              }
              else {
                text[i] = reports.features[i].properties.text.slice(0,160);
              }
              time[i] = reports.features[i].properties.created_at.slice(11,16);
              distance[i] = dist;
            }
          }
        }
      }
      else {
        text[0] = '[Error] Could not detemine user location';
      }
    }
    // Assemble dictionary using our keys
    var dictionary = {
      "KEY_PKEY": pkey.toString(),
      "KEY_DISTANCE":distance.toString(),
      "KEY_TIME": time.toString(),
      "KEY_DESCRIPTION": text.join("|"),
      "KEY_DATA_LENGTH": (pkey.length).toString()
    };

    Pebble.sendAppMessage(dictionary,
      function(e) {
        console.log("Flood info sent to Pebble successfully!");
      },
      function(e) {
        console.log("Error sending Flood info to Pebble!");
      }
    );
  });
};

// AJAX Requests
var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText, this.status);
  };
  xhr.open(type, url);
  xhr.send();
};

// Listen for when the watchapp is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log("PebbleKit JS ready!");
    // Request flood reports from API
    //var url = "https://petajakarta.org/banjir/data/api/v2/reports/confirmed";
    var url = "http://127.0.0.1:8080/sample_reports.json"
    xhrRequest(url, 'GET', processReports);
    }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    }
);
