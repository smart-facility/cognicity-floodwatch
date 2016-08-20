[![Build Status](https://travis-ci.org/smart-facility/cognicity-floodwatch.svg?branch=0.4)](https://travis-ci.org/smart-facility/cognicity-floodwatch)

FloodWatch
==========
### Wearable flood alerts from PetaJakarta.org
FloodWatch is a Pebble SmartWatch Application which shows flood reports recorded by (https://petajakarta.org)[PetaJakarta.org] within 5 km of the user's location.

The Pebble app was built using the Pebble SDK in C and contains Javascript code to pull flood reports from the PetaJakarta.org API.

### Target Platforms
* Pebble Smartwatch ("Aplite" firmware)
* Pebble Time Smartwatch ("Basalt" firmware)

### Build
`pebble build`

### Install (emulator - Pebble SDK)
`pebble install --emulator basalt`

### Pebble SDK
* Pebble Tool v4.3 (active SDK: 3.14)

### Dependencies
Pebble `merge_js` concatenates all JavaScript files, this stripped down versions of libraries must be created, and placed in `src/js/libs`.
* [TurfJS](https://github.com/Turfjs/turf-distance) v3.3.x

### Development Dependencies
Including these in package.json breaks Pebble build. Instead they are listed in scripts/cibuild
* jshint@^2.9.3
* mocha@^3.0.2
* unit.js@^2.0.0

#### Cloud Pebble
The most convenient method to run the flood watch application would be to use the Online Pebble Development application [**CloudPebble**](https://cloudpebble.net "CloudPebble").

1. *CloudPebble* requires a pebble account to login and run the pebble application.
2. Upon importing the repository from github compile the files using the Compilation tab and set the emulator to *Basalt* or *Chalk* since these two emulators represent the newer Pebble Time and Pebble Time Round watches which support colour display.
3. Real time logs for the application can be viewed using the *View App Logs* tab.

#### SDK
The Pebble SDK runs on the UNIX terminal. The SDK can be downloaded from https://developer.pebble.com and manually installed or can be directly installed from the terminal using Homebrew. The website contains instructions for the installation of the SDK using both methods.
* After installation download the Zip folder from the repository, extract it and set it as the current directory.
* Build the application using the `pebble build` function.
* Installation of the application can be done on either an emulator or on the PebbleWatch.
* To install the application using the Pebble Emulator run `pebble install --emulator basalt` or `pebble install --emulator chalk` for the Basalt and Chalk emulators respectively.
* To view the app logs on the terminal use `pebble build --emulator version --logs`.
* To install the app directly onto the pebble watch use `pebble install --cloudpebble`. This installs the application through the cloudpebble connection established by the Pebble Time App on the Android or iOS device the pebble watch is connected to.

#### A Note on Message Sizes
Messages are limited to 10 most recent reports at any one time (within 5 km of user), more than this results in unreasonably long list on the watch. This also helps to reduce memory footprint of the app.

Message descriptions are fixed to 160 characters, as this is a reasonable amount of text to read at font size 18 on the watch. Messages longer than this, such as those from the Pasangmata.com and Qlue data sources will be truncated.

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

## Contributors
This project was funded by a University of Wollongong, Faculty of Engineering and Information Sciences summer scholarship awarded to Hasitha Jayanandana (hasithaj) and led by Tomas Holderness & Matthew Berryman at the Open Source Geospatial Lab (SMART Infrastructure Facility).
