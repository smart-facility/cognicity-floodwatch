'use strict';

var test = require('unit.js');

global.navigator = {
  userAgent: 'node.js',
  geolocation:{
    getCurrentPosition: function(success, error, location){
      console.log('spoof');
    }
  }
};
var FloodWatch = require('../src/js/floodwatch.js');

describe("processReports error status handling", function(){

  var oldGetLocation;
  // Mock location function
  before( function(){
    oldGetLocation = FloodWatch.getLocation;
    FloodWatch.getLocation = function(callback){
      callback('location', 1);
    }
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
    })
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
    }
  });
  // Test null data handling
  it ('Catches empty reports data', function(){
    FloodWatch.processReports('{"features": null}', 200, function(data){
      test.value( data.KEY_DESCRIPTION ).is('[Error] Could not detemine\nwatch location');
    })
  });
  // Return location function to normal
  after ( function(){
    FloodWatch.getLocation = oldGetLocation;
  });
});
