'use strict';
/* FloodWatch - Flood alerts from PetaJakarta.org
 * floodwatch.js - Core application logic
 * (c) Hasitha Jayanandana, Tomas Holderness & Matthew Berryman 2016
 * Released under GNU GPLv3 (see LICENSE.txt)
 */

// Turf JS for distance calculation using Haversine
var turf_distance = require('./libs/@turf/distance');

// FloodWatch object containing location-based logic for report filtering
var FloodWatch = {
  // Default configuration
  config : {
    maxPebbleDictionaryLength: 10,
    location:{
      enableHighAccuracy: true,
      maximumAge: 10000,
      timeout: 10000
    }
  }
};

// Get user location using Pebble/navigator API
FloodWatch.getLocation = function(callback){
 var self = this; // Expects config to come from FloodWatch object

 var watch_location = {
     "type": "Feature",
     "properties": {},
     "geometry": {
       "type": "Point",
       "coordinates" : []
     }
   };

 var watch_location_status = 1;

 function success(pos){
   //watch_location.geometry.coordinates.push(106.826355,-6.1763649); // MONAS
   //watch_location.geometry.coordinates.push(106.78839683532715,-6.176224966990518); // Tanjung Duren Utara
   watch_location.geometry.coordinates.push(pos.coords.longitude, pos.coords.latitude);

   watch_location_status = 0;
   callback(watch_location, watch_location_status);
 }

 function error(){
   watch_location_status = 1;
   callback(watch_location, watch_location_status);
 }

 navigator.geolocation.getCurrentPosition(success, error, self.config.location);
};

// Process flood data & user location, returns Pebble dictionary for callback
FloodWatch.processReports = function(report_string, report_status, callback){

    var self = this;
    // Data stores
    var reports_dictionary = {
      pkey : [],
      text : ['[Success] No nearby flood reports'],
      time : [],
      distance : []
    };

    var filterReportsByLocation = function(watch_location, watch_location_status){
        if (report_status !== 200){
          console.log('Error communicating with server ('+report_status+')');
          reports_dictionary.text[0] = '[Error '+report_status+'] Problem communicating with server';
        }
        else if (watch_location_status !== 0) {
          console.log('Error getting watch location');
          reports_dictionary.text[0] = '[Error] Could not detemine\nwatch location';
        }
        else {
          // Filter by location
          var reports = JSON.parse(report_string);
          if (reports.features !== null){
            var j = 0; // dictionary counter
            for (var i = 0; i < reports.features.length; i++){

              var dist = turf_distance(watch_location, reports.features[i], 'kilometers');
              if (dist <= 5.0 && reports_dictionary.pkey.length < self.config.maxPebbleDictionaryLength) {

                dist = dist.toFixed(1); //Round to 1DP
                reports_dictionary.distance[j] = dist;

                reports_dictionary.pkey[j] = reports.features[i].properties.pkey;
                if (reports.features[i].properties.text.length > 160){
                  // Add ellipsis to long text strings
                  reports_dictionary.text[j] = reports.features[i].properties.text.slice(0,156)+'...';
                }
                else {
                  reports_dictionary.text[j] = reports.features[i].properties.text.slice(0,160);
                }
                reports_dictionary.time[j] = reports.features[i].properties.created_at.slice(11,16);
                j += 1; // increment dictionary counter
              }
            }
          }
        }
        // Assemble dictionary using our keys
        var pebble_dictionary = {
          "KEY_PKEY": reports_dictionary.pkey.toString(),
          "KEY_DISTANCE":reports_dictionary.distance.toString(),
          "KEY_TIME": reports_dictionary.time.toString(),
          "KEY_DESCRIPTION": reports_dictionary.text.join("|"),
          "KEY_DATA_LENGTH": (reports_dictionary.pkey.length).toString()
        };
        callback(pebble_dictionary);
      };
      self.getLocation(filterReportsByLocation);
};
module.exports = FloodWatch;
