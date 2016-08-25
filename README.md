[![Build Status](https://travis-ci.org/smart-facility/cognicity-floodwatch.svg?branch=0.4)](https://travis-ci.org/smart-facility/cognicity-floodwatch)

FloodWatch
==========
### On-hand Flood Alerts
FloodWatch is a Pebble smartwatch app that shows nearby reports of flooding.

### Specifications
FloodWatch shows flood reports recorded by the open source flood map [PetaJakarta.org](https://petajakarta.org) that are within five kilometres from the watch's location.

The Pebble app was built using the Pebble SDK in C and contains Javascript code, executed on the users' phone., to pull flood reports from the [PetaJakarta.org API](https://petajakarta.org/banjir/en/data).

### Target Hardware
* Pebble Smartwatch ("Aplite" firmware)
* Pebble Time Smartwatch ("Basalt" firmware)

### Pebble SDK
Pebble Tool v4.3 (active SDK: 3.14)

### Build
`pebble build`

### Install
###### Emulator
`pebble install --emulator [aplite|basalt]`
###### Watch (via developer connection)
`pebble install --phone [phone_ip_address]`

### Testing
#### JSHint
`npm lint`
#### JavaScript unit tests
`npm mocha`
#### Run all tests
`npm test`
#### Git Hooks
There is a git pre-commit hook which will run the 'npm test' command before your commit and will fail the commit if testing fails.

To use this hook, copy the file from 'git-hooks/pre-commit' to '.git/hooks/pre-commit' in your project folder, and make it executable.

```shell
cp git-hooks/pre-commit .git/hooks/
chmod u+x .git/hooks/pre-commit
```

### Dependencies
The following dependencies are required by the app. Note that as Pebble `merge_js` concatenates all JavaScript files stripped down versions of libraries must be created, and placed in `src/js/libs`.
* [TurfJS](https://github.com/Turfjs/turf-distance) v3.3.x

### Development Dependencies
The following packages are required for testing. Note that including these in `package.json` breaks the build, instead they are listed in `scripts/cibuild` or can be installed manually using npm.
* jshint@^2.9.3
* mocha@^3.0.2
* unit.js@^2.0.0

### Message Sizes
Messages are limited to the 10 most recent reports at any one time (within 5 km of user), more than this results in unreasonably long list on the watch. This also helps to reduce memory footprint of the app.

Message descriptions are fixed to 160 characters, as this is a good amount of text to read at font size 18 on the watch. Messages longer than this will be truncated.

For calculation of `INBOX_SIZE` report fields are as follows

| Field/Key   | Number of characters | Notes                         |
| ----------- | -------------------- | ----------------------------- |
| PKEY        | 20                   | PostgreSQL Bigint             |
| DISTANCE    | 3                    | E.g. 3.1                      |
| TIME        | 6                    | E.g. 23:16                    |
| DESCRIPTION | 160                  | Fit watch screen              |
| LENGTH      | 2                    | Length of PKEY array (max 10) |

Thus, including delimiters a message containing a full compliment of 10 flood reports should contain a
maximum of 227 characters.

Using the [Pebble SDK formula](https://developer.pebble.com/docs/c/Foundation/Dictionary/#dict_calc_buffer_size) we estimate that a maximum of 2220 bytes are needed per message.

### Artwork and App Resources
Artwork for the Pebble app store are in the `app_resources` folder. Raw image files are in GIMP .xcf format.

### Contributors
This project was funded by a University of Wollongong, Faculty of Engineering and Information Sciences summer scholarship awarded to [Hasitha Jayanandana](https://github.com/hasithaj) and supervised by [Tomas Holderness](https://github.com/talltom) & [Matthew Berryman](https://github.com/matthewberryman) at the [SMART Open Source Geospatial Lab](http://smart.uow.edu.au/osgeo/index.html).

### Notes on Pebble SDK
#### Cloud Pebble
The most convenient method to run FloodWatch is the online Pebble Development application [CloudPebble](https://cloudpebble.net "CloudPebble").

* CloudPebble requires a pebble account to login and run the pebble application.
* Upon importing the repository from github compile the files using the Compilation tab and set the emulator to *Basalt* or *Aplite*.
* Real time logs for the application can be viewed using the *View App Logs* tab.

#### Local SDK
The Pebble SDK runs on the UNIX terminal. The SDK can be downloaded from https://developer.pebble.com and manually installed or can be directly installed from the terminal using Homebrew. The website contains instructions for the installation of the SDK using both methods.
* After installation download the Zip folder from the repository, extract it and set it as the current directory.
* Build the application using the instructions above.
* Installation of the application can be done using the instructions above.
* Alternatively, to install the app directly using  CloudPebble. use `pebble install --cloudpebble`. This installs the application through the CloudPebble connection established by the Pebble Time App on the connected Android or iOS device.
* To view the app logs on the terminal use `pebble build --emulator [aplite|basalt] --logs`.
