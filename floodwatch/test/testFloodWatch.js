'use strict';
// Required modules
var test = require('unit.js');
var fs = require('fs');
// Reports JSON
var eleven_reports = fs.readFileSync('./test/sample_reports.json');
var single_report = fs.readFileSync('./test/one_sample_report.json');
var faraway_report = fs.readFileSync('./test/one_faraway_sample_report.json');
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
  it ('Truncates description longer than 160 chars', function(){
    FloodWatch.processReports(single_report, 200, function(data){
      test.value( data.KEY_DESCRIPTION).is("1 Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla odio elit, semper nec purus et, facilisis dignissim arcu. In urna ante, sollicitudin ut sod...");
    });
  });
  it ('Truncates timestamps to HH:MM', function(){
    FloodWatch.processReports(single_report, 200, function(data){
      test.value( data.KEY_TIME).is("13:16");
    });
  });
  it ('Only returns a maximum of 10 reports', function(){
    FloodWatch.processReports(eleven_reports, 200,  function(data){
      test.value( JSON.parse(eleven_reports).features.length).is(11);
      test.value( data.KEY_DATA_LENGTH).is("10");
    });
  });
  it('Excludes reports which are more than 5 km away', function(){
    FloodWatch.processReports(faraway_report, 200, function(data){
      test.value( data.KEY_DESCRIPTION).is("[Success] No nearby flood reports");
    });
  });
  // Return location function to normal
  after ( function(){
    FloodWatch.getLocation = oldGetLocation;
  });
});
