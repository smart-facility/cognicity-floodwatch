#include <pebble.h>
#include "modules/data.h"
#include "modules/windows.h"

// CogniCity Floodwatch - Flood Alerts to Pebble Smart Watch from PetaJakarta.org

// Main
int main(void) {
  init_windows();   // Start
  app_event_loop(); // App
  deinit_windows(); // Close
}
