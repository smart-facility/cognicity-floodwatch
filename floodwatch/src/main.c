#include <pebble.h>
#include "modules/floodwatch.h"
#include "windows/message.h"
#include "windows/splash.h"
// CogniCity Floodwatch - Flood Alerts to Pebble Smart Watch from PetaJakarta.org
// (c) Hasitha Jayanandana, Tomas Holderness & Matthew Berryman 2016
// Released under GNU GPLv3 (see LICENSE.txt)

// Define the App elemments for the Main Window and Report window
static Window *message_window;

// Constants for data transfer
static int INBOX_SIZE = 2220; //originally defined as const, needs fixing.
static int OUTBOX_SIZE = 1; //originally defined as const
static char message_buffer[50];

// Initialise windows
extern void init_windows(void) {

  splash_window_push();

  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);

  bool app_connection = connection_service_peek_pebble_app_connection();
  if (app_connection == 1){
     app_message_open(INBOX_SIZE, OUTBOX_SIZE);
  }
  else {
    snprintf(message_buffer, sizeof(message_buffer), "%s", "[Error] No bluetooth connection available");
    message_window = window_create();
    window_set_user_data(message_window, message_buffer);
    window_set_window_handlers(message_window, (WindowHandlers){
      .load = message_window_load,
      .unload = message_window_unload
    });
    splash_window_pull();
    window_stack_push(message_window, true);

  }
}

// Deinitialise windows
extern void deinit_windows(void) {
  splash_window_pull();
}

// Main
int main(void) {
  init_windows();   // Start
  app_event_loop(); // App
  deinit_windows(); // Close
}
