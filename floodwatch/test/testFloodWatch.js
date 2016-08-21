'use strict';
// Required modules
var test = require('unit.js');
var fs = require('fs');
// Reports JSON
//var report_data = fs.readFileSync('./test/sample_reports.json');
var single_report = fs.readFileSync('./test/one_sample_report.json');
// Spoof Pebble navigator function
global.navigator = {
  userAgent: 'node.js',
  geolocation:{
    getCurrentPosition: function(success, error, location){
      return(0);
    }
  }
};
var FloodWatch = require('../src/js/floodwatch.js');

var location = {
   "type": "Feature",
   "properties": {},
   "geometry": {
     "type": "Point",
     "coordinates" : [106.826355,-6.1763649] //MONAS
   }
};

describe("processReports error status handling", function(){

  var oldGetLocation;
  // Mock location function
  before( function(){
    oldGetLocation = FloodWatch.getLocation;
    FloodWatch.getLocation = function(callback){
      callback('location', 1);
    };
  });
  // Test reports status handling
  it( 'Catches error value in reports data status != 200', function(){
    FloodWatch.processReports('test', 404, function(data){
      test.value( data.KEY_DESCRIPTION ).is('[Error 404] Problem communicating with server');
    });
  });
  // Test location status handling
  it ('Catches error value in watch location data != 0', function(){
    FloodWatch.processReports('test', 200, function(data){
      test.value( data.KEY_DESCRIPTION ).is('[Error] Could not detemine\nwatch location');
    });
  });
  // Return location function to normal
  after ( function(){
    FloodWatch.getLocation = oldGetLocation;
  });
});

describe("processReports null data handling", function(){
  var oldGetLocation;
  // Mock location function
  before( function(){
    oldGetLocation = FloodWatch.getLocation;
    FloodWatch.getLocation = function(callback){
      callback('location', 1);
    };
  });
  // Test null data handling
  it ('Catches empty reports data', function(){
    FloodWatch.processReports('{"features": null}', 200, function(data){
      test.value( data.KEY_DESCRIPTION ).is('[Error] Could not detemine\nwatch location');
    });
  });
  // Return location function to normal
  after ( function(){
    FloodWatch.getLocation = oldGetLocation;
  });
});

describe("processReport data handling and filtering", function(){
  var oldGetLocation;
  // Mock location function with real data
  before( function(){
    oldGetLocation = FloodWatch.getLocation;
    FloodWatch.getLocation = function(callback){
      callback(location, 0);
    };
  });
  // Test real data handling
  it ('Finds one report within 5 km of the watch location', function(){
    FloodWatch.processReports(single_report, 200, function(data){
      test.value( data.KEY_PKEY ).is("1");
    });
  });
  // Return location function to normal
  after ( function(){
    FloodWatch.getLocation = oldGetLocation;
  });
});
