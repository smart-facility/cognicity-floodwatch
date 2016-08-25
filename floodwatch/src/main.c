#include <pebble.h>
#include "modules/floodwatch.h"
#include "windows/message.h"
#include "windows/splash.h"
// CogniCity Floodwatch - Flood Alerts to Pebble Smart Watch from PetaJakarta.org
// (c) Hasitha Jayanandana, Tomas Holderness & Matthew Berryman 2016
// Released under GNU GPLv3 (see LICENSE.txt)

// Define the App elemments for the Main Window and Report window
// Constants for data transfer
static int INBOX_SIZE = 2220; //originally defined as const, needs fixing.
static int OUTBOX_SIZE = 1; //originally defined as const
static char message_buffer[50];

// Initialise windows
extern void init_windows(void) {
  splash_window_push(); // Load splash window
  // Prepare for data
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  // Check Bluetooth connectivity
  // *TO DO* there is latency in bluetooth peek function from Pebble
  if (connection_service_peek_pebble_app_connection()){
    app_message_open(INBOX_SIZE, OUTBOX_SIZE);
  }
  else {
     snprintf(message_buffer, sizeof(message_buffer), "%s", "[Error] No bluetooth connection available");
     splash_window_pull();
     message_window_push(message_buffer);
  }
}

// Deinitialise windows
extern void deinit_windows(void) {
  // *TO DO* need to pull all windows / check what loaded?
  splash_window_pull();
}

// Main
int main(void) {
  init_windows();   // Start
  app_event_loop(); // App
  deinit_windows(); // Close
}
