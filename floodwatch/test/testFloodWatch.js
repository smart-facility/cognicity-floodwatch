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

describe("processReports", function(){
  it( 'Catches error value in reports data != 200', function(){
    FloodWatch.processReports('test', 0, function(data){
      text.value( data.KEY_DESCRIPTION ).is('[Error 0] Problem communicating with server');
    });
  });
  /*it ('Catches error value in watch location data != 0', function(){
    FloodWatch.processReports
  })*/
});
