# FloodWatch README
##What is FloodWatch?
This repository contains the code for a Pebble SmartWatch Application, FloodWatch, which displays the current flood report status in the city of Jakarta as represented by the Peta Jakarta servers. The Pebble app was built using the Pebble SDK in C and contains Javascript code to pull data from the PetaJakarta API for the flood reports.
##Motivation
##Install
###Cloud Pebble
The most convinient method to run the flood watch application would be to use the Online Pebble Development application [**CloudPebble**](https://cloudpebble.net "CloudPebble").

1. *CloudPebble* requires a pebble accout to login and run the pebble application.
2. Upon importing the repository from github compile the files using the Compilation tab and set the emulator to *Basalt* or *Chalk* since these two emulators represent the newer Pebble Time and Pebble Time Round watches which support colour display.
3. Real time logs for the application can be viewed using the *View App Logs* tab.

###Pebble SDK
The Pebble SDK runs on the UNIX terminal. The SDK can be downloaded from https://developer.pebble.com and manually installed or can be directly installed from the terminal using Homebrew. The website contains instructions for the installation of the SDK using both methods.
* After installation download the Zip folder from the repository, extract it and set it as the current directory.
* Build the application using the `pebble build` function.
* Installation of the application can be done on either an emulator or on the PebbleWatch.
* To install the application using the Pebble Emulator run `pebble install --emulator basalt` or `pebble install --emulator chalk` for the Basalt and Chalk emulators respectively.
* To view the app logs on the terminal use `pebble build --emulator version --logs`.
* To install the app directly onto the pebble watch use `pebble install --cloudpebble`. This installs the application through the cloudpebble conection established by the Pebble Time App on the Android or iOS device the pebble watch is connected to.

##CODE
##Testing
##Contributors
