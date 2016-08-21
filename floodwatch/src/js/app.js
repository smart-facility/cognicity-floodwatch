'use strict';
// FloodWatch - Flood alerts from PetaJakarta.org
// app.js - JavaScript app for mobile
// (c) Hasitha Jayanandana, Tomas Holderness & Matthew Berryman 2016
// Released under GNU GPLv3 (see LICENSE.txt)

var FloodWatch = require('./floodwatch.js');

// AJAX Requests
var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.open(type, url, true); // true makes the request asyncronous
  xhr.onload = function () {
    callback(this.responseText, this.status);
    clearTimeout(timeout);
  };
  xhr.send(null);
  var timeout = setTimeout(function(){
    xhr.abort();
    console.log('[Error] Problem communicating with server');
    callback('Request timed out', 418)},
    5000 // timeout in ms
  );
};

// Listen for when the watchapp is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log("PebbleKit JS ready!");
    // Request flood reports from API
    var url = "https://petajakarta.org/banjir/data/api/v2/reports/confirmed";
    xhrRequest(url, 'GET', function(responseText, status){
      FloodWatch.processReports(responseText, status,
        function(pebble_dictionary){
          Pebble.sendAppMessage(pebble_dictionary,
            function() {
              console.log("Flood info sent to Pebble successfully!");
            },
            function(e) {
              console.log("Error sending Flood info to Pebble!");
            }
          );
        }
      );
    });
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    }
);
