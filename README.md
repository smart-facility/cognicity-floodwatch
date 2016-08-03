FloodWatch
==========
### Wearable flood alerts from PetaJakarta.org
This repository contains the code for a Pebble SmartWatch Application, FloodWatch, which displays the current flood report status in the city of Jakarta as recorded by (https://petajakarta.org)[PetaJakarta.org]. The Pebble app was built using the Pebble SDK in C and contains Javascript code to pull flood reports from the PetaJakarta.org API.

### Target Platforms
* Pebble Smartwatch ("Aplite" firmware)
* Pebble Time Smartwatch ("Basalt" firmware)

### Build
`pebble build`

### Install (emulator - Pebble SDK)
`pebble install --emulator basalt`

### Pebble SDK
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

## Contributors
This project was funded by a University of Wollongong, Faculty of Engineering and Information Sciences summer scholarship awarded to Hasitha Jayanandana (hasithaj) and led by Tomas Holderness & Matthew Berryman at the Open Source Geospatial Lab (SMART Infrastructure Facility).
